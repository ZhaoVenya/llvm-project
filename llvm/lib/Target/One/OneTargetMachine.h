#ifndef ONETARGETMACHINE_H
#define ONETARGETMACHINE_H

#include "llvm/Target/TargetMachine.h"

namespace llvm {

    class OneTargetMachine : public LLVMTargetMachine {
        std::unique_ptr<TargetLoweringObjectFile> TLOF;

        public:
        OneTargetMachine(const Target &T, const Triple &TT, StringRef CPU, StringRef FS,
            const TargetOptions &Options, std::optional<Reloc::Model> RM,
            std::optional<CodeModel::Model> CM, CodeGenOptLevel OL, bool JIT);
        
        TargetLoweringObjectFile *getObjFileLowering() const override {
            return TLOF.get();
        }
    };
    
    
}


#endif
