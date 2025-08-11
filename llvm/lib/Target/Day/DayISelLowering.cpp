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



SDValue
DayTargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv,
                                 bool IsVarArg,
                                 const SmallVectorImpl<ISD::OutputArg> &Outs,
                                 const SmallVectorImpl<SDValue> &OutVals,
                                 const SDLoc &DL, SelectionDAG &DAG) const {
  MachineFunction &MF = DAG.getMachineFunction();
  const DaySubtarget &STI = MF.getSubtarget<DaySubtarget>();

  // Stores the assignment of the return value to a location.
  SmallVector<CCValAssign, 16> RVLocs;

  // Info about the registers and stack slot.
  CCState CCInfo(CallConv, IsVarArg, DAG.getMachineFunction(), RVLocs,
                 *DAG.getContext());

  analyzeOutputArgs(DAG.getMachineFunction(), CCInfo, Outs, /*IsRet=*/true,
                    nullptr, CC_Day);

  if (CallConv == CallingConv::GHC && !RVLocs.empty())
    report_fatal_error("GHC functions return void only");

  SDValue Glue;
  SmallVector<SDValue, 4> RetOps(1, Chain);

  // Copy the result values into the output registers.
  for (unsigned i = 0, e = RVLocs.size(), OutIdx = 0; i < e; ++i, ++OutIdx) {
    SDValue Val = OutVals[OutIdx];
    CCValAssign &VA = RVLocs[i];
    assert(VA.isRegLoc() && "Can only return in registers!");

    // Handle a 'normal' return.
    Val = convertValVTToLocVT(DAG, Val, VA, DL, Subtarget);
    Chain = DAG.getCopyToReg(Chain, DL, VA.getLocReg(), Val, Glue);

    if (STI.isRegisterReservedByUser(VA.getLocReg()))
      MF.getFunction().getContext().diagnose(DiagnosticInfoUnsupported{
          MF.getFunction(),
          "Return value register required, but has been reserved."});

    // Guarantee that all emitted copies are stuck together.
    Glue = Chain.getValue(1);
    RetOps.push_back(DAG.getRegister(VA.getLocReg(), VA.getLocVT()));
    
  }

  RetOps[0] = Chain; // Update chain.

  // Add the glue node if we have it.
  if (Glue.getNode()) {
    RetOps.push_back(Glue);
  }

  // if (any_of(RVLocs,
  //            [](CCValAssign &VA) { return VA.getLocVT().isScalableVector(); }))
  //   MF.getInfo<DayMachineFunctionInfo>()->setIsVectorCall();

  unsigned RetOpc = DayISD::RET_GLUE;
  // Interrupt service routines use different return instructions.
  // const Function &Func = DAG.getMachineFunction().getFunction();
  // if (Func.hasFnAttribute("interrupt")) {
  //   if (!Func.getReturnType()->isVoidTy())
  //     report_fatal_error(
  //         "Functions with the interrupt attribute must have void return type!");

  //   MachineFunction &MF = DAG.getMachineFunction();
  //   StringRef Kind =
  //     MF.getFunction().getFnAttribute("interrupt").getValueAsString();

  //   if (Kind == "supervisor")
  //     RetOpc = DayISD::SRET_GLUE;
  //   else
  //     RetOpc = DayISD::MRET_GLUE;
  // }

  return DAG.getNode(RetOpc, DL, MVT::Other, RetOps);
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
