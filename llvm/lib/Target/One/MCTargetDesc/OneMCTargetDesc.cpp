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
#include "OneMCAsmInfo.h"

using namespace llvm;

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

MCInstPrinter *createOneMCInstPrinter(const Triple &T, unsigned SyntaxVariant,
                                      const MCAsmInfo &MAI,
                                      const MCInstrInfo &MII,
                                      const MCRegisterInfo &MRI) {
  return new OneInstPrinter(MAI, MII, MRI);
}

MCSubtargetInfo *createOneMCSubtargetInfo(const Triple &TT, StringRef CPU,
                                          StringRef FS) {
  if (CPU.empty()) {
    CPU = "one";
  }
  return createOneMCSubtargetInfoImpl(TT, CPU, CPU, FS);
}




extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeOneTargetMC() {

    TargetRegistry::RegisterMCAsmInfo(getTheOneTarget(), createOneMCAsmInfo);
    TargetRegistry::RegisterMCInstrInfo(getTheOneTarget(), createOneMCInstrInfo);
    TargetRegistry::RegisterMCRegInfo(getTheOneTarget(), createOneMCRegisterInfo);
    TargetRegistry::RegisterMCSubtargetInfo(getTheOneTarget(), createOneMCSubtargetInfo);

}
