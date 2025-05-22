/*******************************************************
** File: DayTargetMachine.h
** Author: zhaowenya-gbw
** Date: 2025-05-21
** Company: LightStandard
*******************************************************/


#ifndef DAYTARGETMACHINE_H
#define DAYTARGETMACHINE_H

#include "MCTargetDesc/DayMCTargetDesc.h"
#include "DaySubtarget.h"
#include "llvm/CodeGen/CodeGenTargetMachineImpl.h"
#include "llvm/IR/DataLayout.h"
#include <optional>

namespace llvm{
    class DayTargetMachine: public CodeGenTargetMachineImpl/*Add Parent Class*/ 
    {   
        std::unique_ptr<TargetLoweringObjectFile> TLOF;
        mutable StringMap<std::unique_ptr<DaySubtarget>> SubtargetMap;

        public:
            DayTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                        StringRef FS, const TargetOptions &Options,
                        std::optional<Reloc::Model> RM,
                        std::optional<CodeModel::Model> CM, CodeGenOptLevel OL,
                        bool JIT);

            const DaySubtarget *getSubtargetImpl(const Function &F) const override;
            // DO NOT IMPLEMENT: There is no such thing as a valid default subtarget,
            // subtargets are per-function entities based on the target-specific
            // attributes of each function.
            const DaySubtarget *getSubtargetImpl() const = delete;

            TargetPassConfig *createPassConfig(PassManagerBase &PM) override;

            TargetLoweringObjectFile *getObjFileLowering() const override {
                return TLOF.get();
            }

            MachineFunctionInfo * createMachineFunctionInfo(BumpPtrAllocator &Allocator, 
                const Function &F, const TargetSubtargetInfo *STI) const override;

            TargetTransformInfo getTargetTransformInfo(const Function &F) const override;

            bool isNoopAddrSpaceCast(unsigned SrcAS, unsigned DstAS) const override;

            yaml::MachineFunctionInfo *createDefaultFuncInfoYAML() const override;
            yaml::MachineFunctionInfo *convertFuncInfoToYAML(const MachineFunction &MF) const override;
            bool parseMachineFunctionInfo(const yaml::MachineFunctionInfo &,
                                            PerFunctionMIParsingState &PFS,
                                            SMDiagnostic &Error,
                                            SMRange &SourceRange) const override;
            void registerPassBuilderCallbacks(PassBuilder &PB) override;

    };
    
    //// 下边这个函数用不上，是和矢量处理相关的，先注释掉
    // std::unique_ptr<ScheduleDAGMutation> createRISCVVectorMaskDAGMutation(const TargetRegisterInfo *TRI);

} // namespace llvm



#endif // DayTargetMachine

