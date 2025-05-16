//
// Created by zhaowenya on 25-5-6.
//

#include "llvm/MC/TargetRegistry.h"
#include "llvm/MC/MCRegister.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCInstrInfo.h"
#include "OneMCTargetDesc.h"
#include "OneMCAsmInfo.h"
#include "../TargetInfo/OneTargetInfo.h"
#include "OneInstrInfo.h"
#include "OneMCAsmInfo.h"
#include "OneSubtarget.h"
#include "OneRegisterInfo.h"
#include "OneInstPrinter.h"


using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#include "OneGenInstrInfo.inc"


#define GET_SUBTARGETINFO_MC_DESC
#include "OneGenSubtargetInfo.inc"


#define GET_REGINFO_MC_DESC
#include "OneGenRegisterInfo.inc"



MCAsmInfo *createOneMCAsmInfo(const MCRegisterInfo &MRI, const Triple &TT,
                              const MCTargetOptions &Options) {
  return new OneMCAsmInfo(TT);
}


MCRegisterInfo *createOneMCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitOneMCRegisterInfo(X, One::RA);

  return X;
}

MCInstrInfo *createOneMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitOneMCInstrInfo(X);
  return X;
}





MCSubtargetInfo *createOneMCSubtargetInfo(const Triple &TT, StringRef CPU,
                                          StringRef FS) {
  if (CPU.empty()) {
    CPU = "one";
  }
  return createOneMCSubtargetInfoImpl(TT, CPU, CPU, FS);
}


MCInstPrinter *createOneMCInstPrinter(const Triple &T, unsigned SyntaxVariant,
                                      const MCAsmInfo &MAI,
                                      const MCInstrInfo &MII,
                                      const MCRegisterInfo &MRI) {
  return new OneInstPrinter(MAI, MII, MRI);
}


extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeOneTargetMC() {

    TargetRegistry::RegisterMCAsmInfo(getTheOneTarget(), createOneMCAsmInfo);
    TargetRegistry::RegisterMCInstrInfo(getTheOneTarget(), createOneMCInstrInfo);
    TargetRegistry::RegisterMCRegInfo(getTheOneTarget(), createOneMCRegisterInfo);
    TargetRegistry::RegisterMCSubtargetInfo(getTheOneTarget(), createOneMCSubtargetInfo);
    TargetRegistry::RegisterMCInstPrinter(getTheOneTarget(), createOneMCInstPrinter);

}
