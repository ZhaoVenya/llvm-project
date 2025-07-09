include "llvm/Target/Target.td"
include "DayRegisterInfo.td"
include "DayInstrInfo.td"
include "DayCallingConv.td"

def DayInstrInfo : InstrInfo {
    let isLittleEndianEncoding = true;
    let guessInstructionProperties = 0;
}

def DayAsmParser : AsmParser {
  let ShouldEmitMatchRegisterAltName = 1;
  let AllowDuplicateRegisterNames = 0;
}

def DayAsmWriter : AsmWriter {
  int PassSubtarget = 1;
}

def Day : Target {
  let InstructionSet = DayInstrInfo;
  let AssemblyParsers = [DayAsmParser];
  let AssemblyWriters = [DayAsmWriter];
  let AllowRegisterRenaming = 1;
}



def DayTarget : Target{
    let InstructionSet = DayInstrInfo;
}

