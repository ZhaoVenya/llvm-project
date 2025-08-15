#ifndef LLVM_LIB_TARGET_DAY_DAYISELLOWERING_H
#define LLVM_LIB_TARGET_DAY_DAYISELLOWERING_H

#include "Day.h"
#include "DayCallingConv.h"

#include "llvm/CodeGen/TargetLowering.h"



namespace llvm {
class DaySubtarget;
namespace DayISD {
enum NodeType : unsigned { FIRST_NUMBER = ISD::BUILTIN_OP_END, RET_GLUE, Call};
}

class DayTargetLowering : public TargetLowering {
  const DaySubtarget *STI;

public:
  explicit DayTargetLowering(const TargetMachine &TM,
                                const DaySubtarget &ST);

  const char *getTargetNodeName(unsigned Opcode) const override;
  
  bool isLegalICmpImmediate(int64_t Imm) const override;
  bool isLegalAddImmediate(int64_t Imm) const override;
  SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const override;

private:
  SDValue LowerFormalArguments(SDValue Chain, CallingConv::ID CallConv,
                               bool IsVarArg,
                               const SmallVectorImpl<ISD::InputArg> &Ins,
                               const SDLoc &DL, SelectionDAG &DAG,
                               SmallVectorImpl<SDValue> &InVals) const override;

  SDValue LowerReturn(SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
                      const SmallVectorImpl<ISD::OutputArg> &Outs,
                      const SmallVectorImpl<SDValue> &OutVals, const SDLoc &DL,
                      SelectionDAG &DAG) const override;
  
  void analyzeOutputArgs(MachineFunction &MF, CCState &CCInfo,
                         const SmallVectorImpl<ISD::OutputArg> &Outs,
                         bool IsRet, CallLoweringInfo *CLI,
                         DayCCAssignFn Fn) const;
  
  SDValue LowerCall(CallLoweringInfo &CLI, SmallVectorImpl<SDValue> &InVals) const override;
  SDValue LowerGlobalAddress(SDValue Op, SelectionDAG &DAG) const;

};
} // namespace llvm

#endif
