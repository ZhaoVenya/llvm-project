#ifndef DAY_MCTARGETDESC_H
#define DAY_MCTARGETDESC_H

#define GET_REGINFO_ENUM
#include "DayGenRegisterInfo.inc"

#define GET_INSTRINFO_ENUM
#define GET_INSTRINFO_MC_HELPER_DECLS
#include "DayGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "DayGenSubtargetInfo.inc"


// 在 Day.h 或 DayMCTargetDesc.h 中
namespace llvm{
  namespace Day {
  enum {
    NoRegAltName = 0,
    ABIRegAltName = 1
  };
}

}





#endif

