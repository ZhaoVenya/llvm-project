#include "DayISelLowering.h"
#include "MCTargetDesc/DayMCTargetDesc.h"
#include "DaySubtarget.h"
#include "DayMachineFunctionInfo.h"
#include "DayCallingConv.h"

#include "MCTargetDesc/DayMCExpr.h"

#include "llvm/CodeGen/CallingConvLower.h"

// #include "llvm/CodeGen/SelectionDAGAddressAnalysis.h"
// #include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
// #include "llvm/CodeGen/ValueTypes.h"
// #include "llvm/CodeGen/MachineFrameInfo.h"
// #include "llvm/CodeGen/MachineFunction.h"
// #include "llvm/CodeGen/MachineInstrBuilder.h"
// #include "llvm/CodeGen/MachineJumpTableInfo.h"
// #include "llvm/CodeGen/MachineRegisterInfo.h"

using namespace llvm;

#define DEBUG_TYPE "day-lower"

#include "DayGenCallingConv.inc"

DayTargetLowering::DayTargetLowering(const TargetMachine &TM,
                                           const DaySubtarget &ST)
    : TargetLowering(TM), STI(&ST) {
  // Set up the register classes.
  addRegisterClass(MVT::i32, &Day::GPRRegClass);

  // Compute derived properties from the register classes
  computeRegisterProperties(ST.getRegisterInfo());
}



void DayTargetLowering::analyzeOutputArgs(
    MachineFunction &MF, CCState &CCInfo,
    const SmallVectorImpl<ISD::OutputArg> &Outs, bool IsRet,
    CallLoweringInfo *CLI, DayCCAssignFn Fn) const {
  unsigned NumArgs = Outs.size();

  for (unsigned i = 0; i != NumArgs; i++) {
    MVT ArgVT = Outs[i].VT;
    ISD::ArgFlagsTy ArgFlags = Outs[i].Flags;
    Type *OrigTy = CLI ? CLI->getArgs()[Outs[i].OrigArgIndex].Ty : nullptr;

    if (Fn(i, ArgVT, ArgVT, CCValAssign::Full, ArgFlags, CCInfo,
           Outs[i].IsFixed, IsRet, OrigTy)) {
      LLVM_DEBUG(dbgs() << "OutputArg #" << i << " has unhandled type "
                        << ArgVT << "\n");
      llvm_unreachable(nullptr);
    }
  }
}


SDValue DayTargetLowering::LowerFormalArguments(
    SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, const SDLoc &DL,
    SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const {

    /// 1. 分析ins的存储
    /// 2. 产生节点
    ///
    /// ir type, MVT, EVT
    /// ir type: i1 ~i128, f32, f64, struct, array
    /// MVT : machine value type, 寄存器的类型，架构所具体支持的类型，一般都是整型
    /// i8~i32 EVT : 扩展的value type，包含了架构所不支持的类型，比如 i3, i99

    MachineFunction &MF = DAG.getMachineFunction();
    MachineFrameInfo &MFI = MF.getFrameInfo();

    SmallVector<CCValAssign, 16> ArgLocs;
    CCState CCInfo(CallConv, IsVarArg, MF, ArgLocs, *DAG.getContext());
    
    CCInfo.AnalyzeFormalArguments(Ins, CC_Day);

    SDValue ArgValue;
    for (unsigned i = 0, e = ArgLocs.size(); i != e; ++i) {
      CCValAssign &VA = ArgLocs[i];
      if (VA.isRegLoc()) {
        MVT RegVT = VA.getLocVT();
        Register Reg = MF.addLiveIn(VA.getLocReg(), &Day::GPRRegClass);
        ArgValue = DAG.getCopyFromReg(Chain, DL, Reg, RegVT);
        InVals.push_back(ArgValue);
      } else {
        assert(VA.isMemLoc());
        MVT ValVT = VA.getValVT();
        int Offset = VA.getLocMemOffset();
        int FI = MFI.CreateFixedObject(ValVT.getSizeInBits() / 8, Offset, true);
        SDValue FIN = DAG.getFrameIndex(FI, getPointerTy(DAG.getDataLayout()));
        SDValue Val = DAG.getLoad(
            ValVT, DL, Chain, FIN,
            MachinePointerInfo::getFixedStack(DAG.getMachineFunction(), FI));
        InVals.push_back(Val);
      }
    }
    
    return Chain;
}


SDValue DayTargetLowering::LowerCall(CallLoweringInfo &CLI,
                                     SmallVectorImpl<SDValue> &InVals) const {
  SelectionDAG &DAG = CLI.DAG;
  SDLoc &DL = CLI.DL;
  SmallVectorImpl<ISD::OutputArg> &Outs = CLI.Outs;
  SmallVectorImpl<SDValue> &OutVals = CLI.OutVals;
  SmallVectorImpl<ISD::InputArg> &Ins = CLI.Ins;
  SDValue Chain = CLI.Chain;
  SDValue Callee = CLI.Callee;
  CallingConv::ID CallConv = CLI.CallConv;
  bool IsVarArg = CLI.IsVarArg;

  MachineFunction &MF = DAG.getMachineFunction();
  
  // 第一步：分析函数的参数
  SmallVector<CCValAssign, 16> ArgLocs;
  CCState CCInfo(CallConv, IsVarArg, MF, ArgLocs, *DAG.getContext());
  CCInfo.AnalyzeCallOperands(Outs, CC_Day); // 使用特定于调用的规则

  // 第二步：将参数值从虚拟寄存器复制到物理寄存器或栈上
  SmallVector<std::pair<unsigned, SDValue>, 8> RegsToPass;
  SDValue StackPtr;

  for (unsigned i = 0, e = ArgLocs.size(); i != e; ++i) {
    CCValAssign &VA = ArgLocs[i];
    SDValue Arg = OutVals[i];

    if (VA.isRegLoc()) {
      // 参数通过寄存器传递
      RegsToPass.push_back(std::make_pair(VA.getLocReg(), Arg));
    } else {
      // 参数通过栈传递
      assert(VA.isMemLoc() && "Passed argument not in a register or on the stack!");
      
      // 如果栈指针节点还不存在，先获取它
      if (!StackPtr.getNode()) {
        StackPtr = DAG.getCopyFromReg(Chain, DL, Day::SP,
                                      getPointerTy(DAG.getDataLayout()));
      }
      unsigned LocMemOffset = VA.getLocMemOffset();
      
      // 计算栈地址
      SDValue PtrOff = DAG.getIntPtrConstant(LocMemOffset, DL);
      PtrOff = DAG.getNode(ISD::ADD, DL, getPointerTy(DAG.getDataLayout()),
                           StackPtr, PtrOff);

      // 生成 Store 节点，将参数值存到栈上
      Chain = DAG.getStore(Chain, DL, Arg, PtrOff,
                           MachinePointerInfo::getStack(MF, LocMemOffset));
    }
  }

  // 第三步：处理被调用的函数地址 (Callee)
  // 这是最关键的修正点。我们不在这里生成LUI/ADDI机器指令！
  // 而是创建抽象的节点，让指令选择器去处理。
  if (GlobalAddressSDNode *N = dyn_cast<GlobalAddressSDNode>(Callee)) {
    MVT Ty = getPointerTy(DAG.getDataLayout());
    // 创建 TargetGlobalAddress 节点，它代表了 @add 的地址
    // 这个节点会带有重定位信息，指令选择器会识别它
    // 并将其展开为 auipc + jalr 序列
    Callee = DAG.getTargetGlobalAddress(N->getGlobal(), DL, Ty, N->getOffset());
  } else if (ExternalSymbolSDNode *S = dyn_cast<ExternalSymbolSDNode>(Callee)) {
    MVT Ty = getPointerTy(DAG.getDataLayout());
    // 对于外部符号也一样
    Callee = DAG.getTargetExternalSymbol(S->getSymbol(), Ty);
  } else {
      // 对于间接调用 (如通过函数指针)，Calley 已经是有效的SDValue
      // 无需额外操作
  }
  
  // 第四步：生成抽象的 Call 节点
  SmallVector<SDValue, 8> Ops(1, Chain);
  Ops.push_back(Callee); // 将抽象的 Callee 节点作为操作数
  
  SDValue Glue;
  for (const auto &[Reg, Val] : RegsToPass) {
    // 创建 CopyToReg 节点，将参数从虚拟寄存器复制到物理寄存器
    Chain = DAG.getCopyToReg(Chain, DL, Reg, Val, Glue);
    Glue = Chain.getValue(1); // 获取胶水值，用于连接后续操作
    Ops.push_back(DAG.getRegister(Reg, Val.getValueType()));
  }

  // 获取调用保存寄存器掩码
  const TargetRegisterInfo *TRI = STI->getRegisterInfo();
  const uint32_t *Mask = TRI->getCallPreservedMask(MF, CallConv);
  Ops.push_back(DAG.getRegisterMask(Mask));
  if (Glue.getNode()) {
    Ops.push_back(Glue);
  }

  // 创建一个抽象的 DayISD::Call 节点
  SDVTList NodeTys = DAG.getVTList(MVT::Other, MVT::Glue);
  Chain = DAG.getNode(DayISD::Call, DL, NodeTys, Ops);

  // 第五步：处理返回值
  SDValue CallChain = Chain;
  SDValue CallGlue = Chain.getValue(1);
  SmallVector<CCValAssign, 2> RVLos;
  CCState RVInfo(CallConv, IsVarArg, DAG.getMachineFunction(), RVLos, *DAG.getContext());
  RVInfo.AnalyzeCallResult(Ins, RetCC_Day);

  for (unsigned i = 0, e = RVLos.size(); i != e; ++i) {
    CCValAssign &VA = RVLos[i];
    assert(VA.isRegLoc());
    unsigned RVReg = VA.getLocReg();
    EVT VT = VA.getLocVT();
    
    // 生成 CopyFromReg 节点，从物理寄存器中获取返回值
    SDValue Val = DAG.getCopyFromReg(CallChain, DL, RVReg, VT, CallGlue);
    CallChain = Val.getValue(1);
    CallGlue = Val.getValue(2);
    InVals.push_back(Val);
  }

  return CallChain;
}


/// GlobalAddress -> HI / LO
SDValue DayTargetLowering::LowerGlobalAddress(SDValue Op,
                                              SelectionDAG &DAG) const {
  EVT VT = Op.getValueType();
  GlobalAddressSDNode *N = dyn_cast<GlobalAddressSDNode>(Op);
  int64_t Offset = N->getOffset();
  SDLoc DL(N);
  SDValue Hi =
      DAG.getTargetGlobalAddress(N->getGlobal(), DL, VT, 0, DayMCExpr::HI);
  SDValue Lo =
      DAG.getTargetGlobalAddress(N->getGlobal(), DL, VT, 0, DayMCExpr::LO);

  SDValue MHiNode = SDValue(DAG.getMachineNode(Day::LUI, DL, VT, Hi), 0);
  SDValue BaseAddr =
      SDValue(DAG.getMachineNode(Day::ADDI, DL, VT, MHiNode, Lo), 0);
  if (Offset) {
    return DAG.getNode(ISD::ADD, DL, VT, BaseAddr,
                       DAG.getConstant(Offset, DL, VT));
  }
  return BaseAddr;
}



// 核心：重写LowerReturn，将通用RET转换为自定义RET_GLUE
SDValue DayTargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv,
                                      bool IsVarArg,
                                      const SmallVectorImpl<ISD::OutputArg> &Outs,
                                      const SmallVectorImpl<SDValue> &OutVals,
                                      const SDLoc &DL, SelectionDAG &DAG) const {
// 1. 处理返回值。
  // 遍历所有返回值，将其从 OutVals 复制到约定的返回寄存器中。
  // 比如，如果返回值在 OutVals[0] 中，你需要将其复制到寄存器 A0。
  // 可以使用 CopyToReg 节点来完成这个操作。
  //
  // 这里简化处理，假设只有一个返回值，并且已经存在于 OutVals[0] 中。
  if (!Outs.empty()) {
    Chain = DAG.getCopyToReg(Chain, DL, Day::A0, OutVals[0], SDValue());
  }

  // 2. 将返回值寄存器和 Chain 节点打包成一个 Glue 节点。
  // 这是为了确保 retglue 节点在所有 CopyToReg 完成后执行。
  SDValue Glue = Chain.getValue(1);
  SmallVector<SDValue, 1> RetOps;
  RetOps.push_back(Chain);

  // 如果有返回值，需要将返回值寄存器也作为操作数传递给 RET_GLUE。
  if (!Outs.empty()) {
      RetOps.push_back(DAG.getRegister(Day::A0, MVT::i32));
  }

  // 3. 创建 retglue 节点。
  // 注意，这里创建的是你自定义的 retglue 节点。
  SDValue RetNode = DAG.getNode(DayISD::RET_GLUE, DL, MVT::Other, RetOps);

  // 4. 将 retglue 节点标记为终结节点。
  // 这会确保 retglue 节点是 SelectionDAG 的根。
  DAG.setRoot(RetNode);
  
  return RetNode;
}




const char *DayTargetLowering::getTargetNodeName(unsigned Opcode) const {
  switch ((DayISD::NodeType)Opcode) {
  case DayISD::RET_GLUE:
    return "DayISD::RET_GLUE";
  case DayISD::Call:
    return "DayISD::Call";
  default:
    return nullptr;
  }
}


bool DayTargetLowering::isLegalICmpImmediate(int64_t Imm) const {
  return isInt<14>(Imm);
}

bool DayTargetLowering::isLegalAddImmediate(int64_t Imm) const {
  return isInt<14>(Imm);
}



static SDValue lowerConstant(SDValue Op, SelectionDAG &DAG){
  assert(Op.getValueType() == MVT::i32 && "Unexpected VT");

  int32_t Imm = cast<ConstantSDNode>(Op)->getSExtValue();
  
  if(isInt<32>(Imm))
    return Op;

}


SDValue DayTargetLowering::LowerOperation(SDValue Op, SelectionDAG &DAG) const {
  
  switch(Op.getOpcode()){
    default:
      report_fatal_error("unimplemented operand");
    
    case ISD::Constant:
      return lowerConstant(Op,DAG);

  }

  return SDValue();

}
