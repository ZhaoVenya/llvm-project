// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 这个文件是整个新加架构Day芯片的一个集合，包含了关于新架构相关所有的td信息，是顶层。
// td文件只能是顶层调用次顶层，次顶层调用次次顶层。不能反过来。
// 

include "llvm/Target/Target.td"

//===----------------------------------------------------------------------===//
// Target-dependent interfaces
//===----------------------------------------------------------------------===//

// Register Infomation
include "DayRegisterInfo.td"

// Calling Conversion
include "DaySchedule.td"

// Instruction Description
include "DayInstrInfo.td"

def DayInstrInfo : InstrInfo;

// Whill generate DayGenAsmWrite.inc included by DayInstPrinter.cpp, contents
// as follows,
// void DayInstPrinter::printInstruction(const MCInst *MI, raw_ostream &O) {...}
// const char* DayInstPrinter::getRegisterName(unsigned RegNo) {...}
def Day : Target {
  // def DayInstrInfo : InstrInfo as before.
  let InstructionSet = DayInstrInfo;
}

