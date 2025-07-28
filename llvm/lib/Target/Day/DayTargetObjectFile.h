#ifndef DAY_TARGETOBJECTFILE_H_
#define DAY_TARGETOBJECTFILE_H_


#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"

namespace llvm
{

class MCContext;
class TargetMachine;

class DayELFTargetObjectFile : public TargetLoweringObjectFileELF {
public:
  DayELFTargetObjectFile() = default;
  void Initialize(MCContext &Ctx, const TargetMachine &TM) override;

};

}



#endif


