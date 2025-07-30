#ifndef DAY_ISELLOWERING_H_
#define DAY_ISELLOWERING_H_

#include "Day.h"
#include "llvm/CodeGen/TargetLowering.h"

namespace llvm{
class DaySubtarget;

namespace DayISD{

    enum NodeType{
        FIRST_NUMBER = ISD::BUILTIN_OP_END,
        M_LOAD,
        M_STORE,
        RET_GLUE,
        Call,

    };

}

class DayTargetLowering : public TargetLowering{
    const DaySubtarget *Subtarget;
public:
    DayTargetLowering(const TargetMachine &TM, const DaySubtarget &STI);
};


}



#endif
