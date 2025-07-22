#ifndef DAYTARGETMACHINE_H_
#define DAYTARGETMACHINE_H_

#include "llvm/CodeGen/CodeGenTargetMachineImpl.h"

namespace llvm{
    class DayTargetMachine : public CodeGenTargetMachineImpl{
        std::unique_ptr<TargetLoweringObjectFile> TLOF;
        mutable StringMap<std::unique_ptr<DaySubtarget>> SubtargetMap;

        public:
            DayTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                         StringRef FS, const TargetOptions &Options,
                         std::optional<Reloc::Model> RM,
                         std::optional<CodeModel::Model> CM, CodeGenOptLevel OL, bool JIT);
            
            
            const DaySubtarget *getSubtargetImpl(const Function &F) const override;
            // DO NOT IMPLEMENT: There is no such thing as a valid default subtarget,
            // subtargets are per-function entities based on the target-specific
            // attributes of each function.
            const DaySubtarget *getSubtargetImpl() const = delete;
            TargetPassConfig *createPassConfig(PassManagerBase &PM) override;
            TargetLoweringObjectFile *getObjFileLowering() const override {
                return TLOF.get();
            }
    };
    
}

#endif
