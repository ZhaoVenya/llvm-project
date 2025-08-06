#ifndef LLVM_LIB_TARGET_Day_MCTARGETDESC_DayMCASMINFO_H
#define LLVM_LIB_TARGET_Day_MCTARGETDESC_DayMCASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {

class Triple;

class DayMCAsmInfo : public MCAsmInfoELF {
public:
  explicit DayMCAsmInfo(const Triple &TargetTriple);
};
} // namespace llvm

#endif