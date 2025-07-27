#include "DayMCTargetDesc.h"
#include "TargetInfo/DayTargetInfo.h"


#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"



using namespace llvm;


#define GET_INSTRINFO_MC_DESC
#define ENABLE_INSTR_PREDICATE_VERIFIER
#include "DayGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "DayGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "DayGenRegisterInfo.inc"


static MCInstrInfo *createDayMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitDayMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createDayMCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitDayMCRegisterInfo(X, Day::RA);
  return X;
}

static MCSubtargetInfo *createDaySubtargetInfo(const Triple &TT,
                                                  StringRef CPU, StringRef FS) {
    return createDayMCSubtargetInfoImpl(TT, CPU, /*TuneCPU*/CPU, FS);
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeDayTargetMC() {
    TargetRegistry::RegisterMCInstrInfo(getTheDayTarget(), createDayMCInstrInfo);
    TargetRegistry::RegisterMCRegInfo(getTheDayTarget(), createDayMCRegisterInfo);
    TargetRegistry::RegisterMCSubtargetInfo(getTheDayTarget(), createDaySubtargetInfo);
}


