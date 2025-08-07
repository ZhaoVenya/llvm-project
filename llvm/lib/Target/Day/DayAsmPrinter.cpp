


#include "DayAsmPrinter.h"

#include "MCTargetDesc/DayMCTargetDesc.h"
#include "TargetInfo/DayTargetInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

#include "DayGenMCPseudoLowering.inc"
// #include "DayGenAsmWriter.inc"

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


bool DayAsmPrinter::lowerOperand(const MachineOperand &MO,
                                   MCOperand &MCOp) const {
  switch (MO.getType()) {
  default:
    report_fatal_error("lowerOperand: unknown operand type");
  case MachineOperand::MO_Register:
    // Ignore all implicit register operands.
    if (MO.isImplicit())
      return false;
    MCOp = MCOperand::createReg(MO.getReg());
    break;
  case MachineOperand::MO_RegisterMask:
    // Regmasks are like implicit defs.
    return false;
  case MachineOperand::MO_Immediate:
    MCOp = MCOperand::createImm(MO.getImm());
    break;
  // case MachineOperand::MO_MachineBasicBlock:
  //   MCOp = lowerSymbolOperand(MO, MO.getMBB()->getSymbol(), *this);
  //   break;
  // case MachineOperand::MO_GlobalAddress:
  //   MCOp = lowerSymbolOperand(MO, getSymbolPreferLocal(*MO.getGlobal()), *this);
  //   break;
  // case MachineOperand::MO_BlockAddress:
  //   MCOp = lowerSymbolOperand(MO, GetBlockAddressSymbol(MO.getBlockAddress()),
  //                             *this);
  //   break;
  // case MachineOperand::MO_ExternalSymbol:
  //   MCOp = lowerSymbolOperand(MO, GetExternalSymbolSymbol(MO.getSymbolName()),
  //                             *this);
  //   break;
  // case MachineOperand::MO_ConstantPoolIndex:
  //   MCOp = lowerSymbolOperand(MO, GetCPISymbol(MO.getIndex()), *this);
  //   break;
  // case MachineOperand::MO_JumpTableIndex:
  //   MCOp = lowerSymbolOperand(MO, GetJTISymbol(MO.getIndex()), *this);
  //   break;
  // case MachineOperand::MO_MCSymbol:
  //   MCOp = lowerSymbolOperand(MO, MO.getMCSymbol(), *this);
  //   break;
  }
  return true;
}



// Force static initialization.
extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeDayAsmPrinter() {
  RegisterAsmPrinter<DayAsmPrinter> X(getTheDayTarget());
}

