#include "DayISelLowering.h"
#include "MCTargetDesc/DayMCTargetDesc.h"
#include "DaySubtarget.h"
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
                                           const DaySubtarget &STI)
    : TargetLowering(TM), Subtarget(&STI) {
  // Set up the register classes.
  addRegisterClass(MVT::i32, &Day::GPRRegClass);

  // Compute derived properties from the register classes
  computeRegisterProperties(STI.getRegisterInfo());
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
  SmallVector<CCValAssign, 16> RVLocs;

  CCState CCInfo(CallConv, IsVarArg, DAG.getMachineFunction(), RVLocs,
                 *DAG.getContext());
  CCInfo.AnalyzeReturn(Outs, RetCC_Day);

  SDValue Glue;
  SmallVector<SDValue, 4> RetOps(1, Chain);

  for (unsigned i = 0, e = RVLocs.size(); i < e; ++i) {
    CCValAssign &VA = RVLocs[i];
    assert(VA.isRegLoc() && "Can only return in registers!");
    Chain = DAG.getCopyToReg(Chain, DL, VA.getLocReg(), OutVals[i], Glue);
    Glue = Chain.getValue(1);
    RetOps.push_back(DAG.getRegister(VA.getLocReg(), VA.getLocVT()));
  }

  RetOps[0] = Chain;

  if (Glue.getNode()) {
    RetOps.push_back(Glue);
  }

  return DAG.getNode(DayISD::RET_GLUE, DL, MVT::Other, RetOps);
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
