#include "DayTargetInfo.h"
#include "llvm/MC/TargetRegistry.h"


using namespace llvm;

Target &llvm::getTheDayTarget() {
  static Target TheDayTarget;
  return TheDayTarget;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeDayTargetInfo() {
  RegisterTarget<Triple::day, /*HasJIT=*/false> X(getTheDayTarget(),
                                              "day", "Day 32-bit little-endian", "Day");
}
