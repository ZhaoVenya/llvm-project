#include "DayMCTargetDesc.h"
#include "DayInstPrinter.h"
#include "DayMCAsmInfo.h"
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


static MCAsmInfo *createDayMCAsmInfo(const MCRegisterInfo &MRI,
                                        const Triple &TT,
                                        const MCTargetOptions &Options) {
  MCAsmInfo *MAI = new DayMCAsmInfo(TT);
  return MAI;
}

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

static MCInstPrinter *createDayMCInstPrinter(const Triple &T,
                                                unsigned SyntaxVariant,
                                                const MCAsmInfo &MAI,
                                                const MCInstrInfo &MII,
                                                const MCRegisterInfo &MRI) {
  return new DayInstPrinter(MAI, MII, MRI);
}




extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeDayTargetMC() {
    TargetRegistry::RegisterMCAsmInfo(getTheDayTarget(), createDayMCAsmInfo);
    TargetRegistry::RegisterMCInstrInfo(getTheDayTarget(), createDayMCInstrInfo);
    TargetRegistry::RegisterMCRegInfo(getTheDayTarget(), createDayMCRegisterInfo);
    TargetRegistry::RegisterMCSubtargetInfo(getTheDayTarget(), createDaySubtargetInfo);
    TargetRegistry::RegisterMCInstPrinter(getTheDayTarget(), createDayMCInstPrinter);
}

