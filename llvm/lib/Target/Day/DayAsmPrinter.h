#ifndef DAY_ASMPRINTER_H_
#define DAY_ASMPRINTER_H_

#include "DayMCInstLowering.h"
#include "llvm/CodeGen/AsmPrinter.h"

namespace llvm {

class LLVM_LIBRARY_VISIBILITY DayAsmPrinter : public AsmPrinter {
  DayMCInstLower MCInstLowering;

public:
  explicit DayAsmPrinter(TargetMachine &TM,
                            std::unique_ptr<MCStreamer> Streamer);

  StringRef getPassName() const override { return "Day Assembly Printer"; }

  void emitInstruction(const MachineInstr *MI) override;

private:
  bool emitPseudoExpansionLowering(MCStreamer &OutStreamer,
                                   const MachineInstr *MI);
};
} // end namespace llvm

#endif
