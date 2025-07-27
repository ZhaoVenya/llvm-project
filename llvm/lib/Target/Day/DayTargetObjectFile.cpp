
#include "DayTargetObjectFile.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"

class SparcELFTargetObjectFile : public TargetLoweringObjectFileELF {
public:
  SparcELFTargetObjectFile() = default;

};