#ifndef DAY_FRAME_LOWERING_H_
#define DAY_FRAME_LOWERING_H_

#include "Day.h"
#include "llvm/CodeGen/TargetFrameLowering.h"

namespace llvm{

class DaySubtarget;

class DayFrameLowering : public TargetFrameLowering {
public:
  explicit DayFrameLowering(const DaySubtarget &ST);


};

}

#endif
