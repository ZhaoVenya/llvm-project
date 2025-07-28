#ifndef DAY_ISELLOWERING_H_
#define DAY_ISELLOWERING_H_

#include "Day.h"
#include "llvm/CodeGen/TargetLowering.h"

namespace llvm{
    class DaySubtarget;

class DayTargetLowering : public TargetLowering{
    const DaySubtarget *Subtarget;
public:
    DayTargetLowering(const TargetMachine &TM, const DaySubtarget &STI);
};


}



#endif
