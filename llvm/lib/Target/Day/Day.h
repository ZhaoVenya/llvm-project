#ifndef DAY_H_
#define DAY_H_

#include "MCTargetDesc/DayMCTargetDesc.h"
#include "llvm/Target/TargetMachine.h"


#define DIV_ROUND_UP(n, d)  (((n) + (d) - 1) / (d))
#define ROUND_UP(x, align)  (DIV_ROUND_UP(x, align) * (align))



namespace llvm {
    class DayTargetMachine;
    class FunctionPass;

    FunctionPass *createDayISelDag(DayTargetMachine &TM);

}


#endif
