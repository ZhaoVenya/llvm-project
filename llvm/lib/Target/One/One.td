// One.td
include "llvm/Target/Target.td"  // 这个可能已经包含了 TargetSelectionDAG.td
include "OneRegisterInfo.td"
include "OneInstrInfo.td"       // 如果 InstrInfo 需要额外的东西，可以在这里加
include "OneCallingConv.td"


def ProcessorModel : ProcessorModel<"one", NoSchedModel, []>;

def OneInstrInfo : InstrInfo;

def One : Target{
    let InstructionSet = OneInstrInfo;
}

 

