#ifndef DAY_MCEXPR_H_
#define DAY_MCEXPR_H_

#include "llvm/MC/MCExpr.h"

namespace llvm {
class DayMCExpr : public MCTargetExpr {
public:
  enum Kind { NONE, HI, LO };
  DayMCExpr(Kind K, const MCExpr *Expr) : Kd(K), Expr(Expr) {}

  void printImpl(raw_ostream &OS, const MCAsmInfo *MAI) const override;
  bool evaluateAsRelocatableImpl(MCValue &Res, const MCAssembler *Asm,
                                 const MCFixup *Fixup) const override {
    return false;
  };
  void visitUsedExpr(MCStreamer &Streamer) const override {}
  MCFragment *findAssociatedFragment() const override { return nullptr; }

  void fixELFSymbolsInTLSFixups(MCAssembler &) const override {}

private:
  const Kind Kd;
  const MCExpr *Expr;
};
} // namespace llvm

#endif

