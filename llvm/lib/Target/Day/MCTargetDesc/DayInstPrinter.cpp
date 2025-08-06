#include "DayInstPrinter.h"

#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCRegisterInfo.h"

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

#define PRINT_ALIAS_INSTR
#include "DayGenAsmWriter.inc"

void DayInstPrinter::printInst(const MCInst *MI, uint64_t Address,
                                  StringRef Annot, const MCSubtargetInfo &STI,
                                  raw_ostream &O) {
  if (!printAliasInstr(MI, Address, STI, O)) {
    printInstruction(MI, Address, STI, O);
  }
}

void DayInstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                  const MCSubtargetInfo &STI, raw_ostream &O) {

}

void DayInstPrinter::printBranchOperand(const MCInst *MI,
                                          unsigned OpNo,
                                          const MCSubtargetInfo &STI,
                                          raw_ostream &O) {

}


void printCustomAliasOperand(const MCInst *MI, uint64_t Address,
                               unsigned OpIdx, unsigned PrintMethodIdx,
                               const MCSubtargetInfo &STI, raw_ostream &O){

}
// const char *DayInstPrinter::getRegisterName(MCRegister Reg) {
//   return getRegisterName(Reg, Day::NoRegAltName);
// }



