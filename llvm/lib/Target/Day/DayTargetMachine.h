#ifndef DAYTARGETMACHINE_H_
#define DAYTARGETMACHINE_H_

#include "DayInstrInfo.h"
#include "DaySubtarget.h"
#include "llvm/CodeGen/CodeGenTargetMachineImpl.h"
#include "llvm/Target/TargetMachine.h"
#include <optional>

namespace llvm{

class DayTargetMachine : public CodeGenTargetMachineImpl {

    // const DaySubtarget& Subtarget;

    std::unique_ptr<TargetLoweringObjectFile> TLOF;
    bool is64Bit;
    // bool isJIT;
    mutable StringMap<std::unique_ptr<DaySubtarget>> SubtargetMap;

public:
  DayTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                     StringRef FS, const TargetOptions &Options,
                     std::optional<Reloc::Model> RM,
                     std::optional<CodeModel::Model> CM, CodeGenOptLevel OL,
                     bool JIT, bool is64bit);
  ~DayTargetMachine() override;

  const DaySubtarget *getSubtargetImpl(const Function &F) const override;

  // Pass Pipeline Configuration
  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;
  TargetLoweringObjectFile *getObjFileLowering() const override {
    return TLOF.get();
  }

  MachineFunctionInfo *
  createMachineFunctionInfo(BumpPtrAllocator &Allocator, const Function &F,
                            const TargetSubtargetInfo *STI) const override;

};

class Day32TargetMachine : public DayTargetMachine {
    virtual void anchor();
public:
    Day32TargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                     StringRef FS, const TargetOptions &Options,
                     std::optional<Reloc::Model> RM,
                     std::optional<CodeModel::Model> CM, CodeGenOptLevel OL,
                     bool JIT);
};
    
}

#endif
