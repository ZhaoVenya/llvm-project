#include "DayAsmPrinter.h"



#include "DayAsmPrinter.h"
#include "MCTargetDesc/DayMCTargetDesc.h"
#include "TargetInfo/DayTargetInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

#include "DayGenMCPseudoLowering.inc"

DayAsmPrinter::DayAsmPrinter(TargetMachine &TM,
                                   std::unique_ptr<MCStreamer> Streamer)
    : AsmPrinter(TM, std::move(Streamer)), MCInstLowering(OutContext, *this) {}

void DayAsmPrinter::emitInstruction(const MachineInstr *MI) {
  if (emitPseudoExpansionLowering(*OutStreamer, MI)) {
    return;
  }
  MCInst TmpInst;
  MCInstLowering.Lower(MI, TmpInst);
  EmitToStreamer(*OutStreamer, TmpInst);
}

// Force static initialization.
extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeDayAsmPrinter() {
  RegisterAsmPrinter<DayAsmPrinter> X(getTheDayTarget());
}

