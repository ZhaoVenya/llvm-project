#include "Day.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/MacroBuilder.h"
#include "llvm/TargetParser/RISCVTargetParser.h"
#include <optional>

namespace clang {
namespace targets {
DayTargetInfo::DayTargetInfo(const llvm::Triple &Triple,
                                   const TargetOptions &)
    : TargetInfo(Triple) {
  IntPtrType = SignedInt;
  PtrDiffType = SignedInt;
  SizeType = UnsignedInt;
  resetDataLayout("e-m:e-p:32:32-i32:32-n32-S32");
}

void DayTargetInfo::getTargetDefines(const LangOptions &Opts,
                                        MacroBuilder &Builder) const {}

bool DayTargetInfo::validateAsmConstraint(
    const char *&Name, TargetInfo::ConstraintInfo &info) const {
  return false;
}

} // namespace targets
} // namespace clang