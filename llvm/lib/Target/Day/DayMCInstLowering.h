#ifndef DAY_MCINSTLOWERING_H_
#define DAY_MCINSTLOWERING_H_

#include "llvm/Support/Compiler.h"

namespace llvm {
class AsmPrinter;
class MCContext;
class MCInst;
class MCOperand;
class MachineInstr;
class MachineOperand;

class LLVM_LIBRARY_VISIBILITY DayMCInstLower {
  MCContext &Ctx;
  AsmPrinter &Printer;

public:
  DayMCInstLower(MCContext &Ctx, AsmPrinter &Printer)
      : Ctx(Ctx), Printer(Printer) {}

  void Lower(const MachineInstr *MI, MCInst &OutMI) const;
};
} // namespace llvm


#endif
