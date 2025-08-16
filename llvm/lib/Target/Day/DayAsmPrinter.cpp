


#include "DayAsmPrinter.h"

#include "MCTargetDesc/DayMCTargetDesc.h"
#include "MCTargetDesc/DayMCExpr.h"
#include "TargetInfo/DayTargetInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

#include "DayGenMCPseudoLowering.inc"
// #include "DayGenAsmWriter.inc"

DayAsmPrinter::DayAsmPrinter(TargetMachine &TM,
                                   std::unique_ptr<MCStreamer> Streamer)
    : AsmPrinter(TM, std::move(Streamer)) {}

void DayAsmPrinter::emitInstruction(const MachineInstr *MI) {

  if (MCInst OutInst; lowerPseudoInstExpansion(MI, OutInst)) {
    EmitToStreamer(*OutStreamer, OutInst);
    return;
  }

  MCInst TmpInst;
  lowerToMCInst(MI, TmpInst);
  EmitToStreamer(*OutStreamer, TmpInst);
}


void DayAsmPrinter::lowerToMCInst(const MachineInstr *MI, MCInst &Out) {
  Out.setOpcode(MI->getOpcode());

  for (const MachineOperand &MO : MI->operands()) {
    MCOperand MCOp;
    lowerOperand(MO, MCOp);
    Out.addOperand(MCOp);
  }
}

static MCOperand lowerSymbolOperand(const MachineOperand &MO, MCSymbol *Sym,
                                    const AsmPrinter &AP) {
  MCContext &Ctx = AP.OutContext;
  unsigned Kind;
  const MCSymbol *symbol = nullptr;

  switch (MO.getTargetFlags()) {
  default:
    llvm_unreachable("Unknown target flag on GV operand");
  case 0:
    Kind = 0;
    break;
  case DayMCExpr::HI:
    Kind = DayMCExpr::HI;
    break;
  case DayMCExpr::LO:
    Kind = DayMCExpr::LO;
    break;
  }

  // if (MO.getType() == MachineOperand::MO_MachineBasicBlock) {
  //   symbol = MO.getMBB()->getSymbol();
  // } else if (MO.getType() == MachineOperand::MO_ExternalSymbol) {
  //   symbol = GetExternalSymbolSymbol(MO.getSymbolName());
  // }
  // else {
  //   symbol = getSymbol(MO.getGlobal());
  // }

  const MCExpr *ME =
      MCSymbolRefExpr::create(Sym, MCSymbolRefExpr::VK_None, Ctx);

  if (!MO.isJTI() && !MO.isMBB() && MO.getOffset())
    ME = MCBinaryExpr::createAdd(
        ME, MCConstantExpr::create(MO.getOffset(), Ctx), Ctx);

  return MCOperand::createExpr(ME);
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
  case MachineOperand::MO_MachineBasicBlock:
    MCOp = lowerSymbolOperand(MO, MO.getMBB()->getSymbol(), *this);
    break;
  case MachineOperand::MO_GlobalAddress:
    MCOp = lowerSymbolOperand(MO, getSymbolPreferLocal(*MO.getGlobal()), *this);
    break;
  case MachineOperand::MO_ExternalSymbol:
    MCOp = lowerSymbolOperand(MO, GetExternalSymbolSymbol(MO.getSymbolName()),
                              *this);
    break;
  
  }
  return true;
}



// Force static initialization.
extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeDayAsmPrinter() {
  RegisterAsmPrinter<DayAsmPrinter> X(getTheDayTarget());
}

