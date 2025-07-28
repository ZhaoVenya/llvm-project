#include "DayISelLowering.h"

using namespace llvm;

DayTargetLowering::DayTargetLowering(const TargetMachine &TM, const DaySubtarget &STI)
    : TargetLowering(TM), Subtarget(&STI)     // 初始化成员 Subtarget
{

}