#include "DayISelLowering.h"
#include "MCTargetDesc/DayMCTargetDesc.h"
#include "DaySubtarget.h"
#include "DayMachineFunctionInfo.h"
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
  return Chain;
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
