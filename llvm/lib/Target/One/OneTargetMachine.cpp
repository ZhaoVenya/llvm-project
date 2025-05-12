//
// Created by zhaowenya on 25-5-6.
//

#include "llvm/MC/TargetRegistry.h"
#include "TargetInfo/OneTargetInfo.h"
#include "OneTargetMachine.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/TargetParser/Triple.h"

#define DEBUG_TYPE "one"

using namespace llvm;

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeOneTarget() {
//    extern Target TheFooTarget;
   RegisterTargetMachine<OneTargetMachine> X(getTheOneTarget());

}


static StringRef computeDataLayout(const Triple &TT,
    const TargetOptions &Options){

    assert(TT.isArch32Bit() && "Only 32bit currently supported");

  return "e-m:e-p:32:32-i64:64-n32-S128";
        
}

static Reloc::Model getEffectiveRelocModel(const Triple &TT,
    std::optional<Reloc::Model> RM) {
return RM.value_or(Reloc::Static);
}


OneTargetMachine::OneTargetMachine(const Target &T, const Triple &TT, 
    StringRef CPU, StringRef FS,
    const TargetOptions &Options, 
    std::optional<Reloc::Model> RM,
    std::optional<CodeModel::Model> CM, 
    CodeGenOptLevel OL, bool JIT) 
    : CodeGenTargetMachineImpl(T, computeDataLayout(TT, Options), TT, CPU, FS, Options, 
                            getEffectiveRelocModel(TT, RM),
                            getEffectiveCodeModel(CM, CodeModel::Small), OL),
    TLOF(std::make_unique<TargetLoweringObjectFileELF>()) , 
    Subtarget(TT, CPU, FS, *this)
{
    initAsmInfo();

}
 