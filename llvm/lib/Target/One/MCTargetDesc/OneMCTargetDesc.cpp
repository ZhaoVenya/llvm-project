//
// Created by zhaowenya on 25-5-6.
//

#include "llvm/MC/TargetRegistry.h"
#include "OneMCTargetDesc.h"
#include "OneMCAsmInfo.h"
#include "../TargetInfo/OneTargetInfo.h"
#include "OneMCAsmInfo.h"

using namespace llvm;

static MCAsmInfo *createOneMCAsmInfo(const MCRegisterInfo &MRI,
                                       const Triple &TT,
                                       const MCTargetOptions &Options) {
    
    return new OneMCAsmInfo(TT);

}



static MCInstrInfo *createOneMCInstrInfo() {

}


static MCRegisterInfo * createOneMCRegisterInfo(const Triple &TT){



}

static MCSubtargetInfo * createOneMCSubtargetInfo(const Triple &TT,
                        StringRef CPU, StringRef FS){


}







extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeOneTargetMC() {

    TargetRegistry::RegisterMCAsmInfo(getTheOneTarget(), createOneMCAsmInfo);
    TargetRegistry::RegisterMCInstrInfo(getTheOneTarget(), createOneMCInstrInfo);
    TargetRegistry::RegisterMCRegInfo(getTheOneTarget(), createOneMCRegisterInfo);
    TargetRegistry::RegisterMCSubtargetInfo(getTheOneTarget(), createOneMCSubtargetInfo);

}
