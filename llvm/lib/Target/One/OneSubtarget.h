#ifndef ONESUBTARGET_H
#define ONESUBTARGET_H

#include "OneInstrInfo.h"
#include "OneRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAGTargetInfo.h"
#include "OneFrameLowering.h"

#define GET_SUBTARGETINFO_HEADER
#include "OneGenSubtargetInfo.inc"


namespace llvm
{
    class OneSubtarget : public OneGenSubtargetInfo
    {
        private:
            OneInstrInfo InstrInfo;
            OneRegisterInfo RegInfo;
            OneFrameLowering FrameLowering;
            OneTargetLowering TLInfo;

        public:
            OneSubtarget(const Triple &TT, StringRef CPU, StringRef Features, const TargetMachine &TM);
            OneSubtarget &initializeSubtargetDependencies(StringRef CPU, StringRef FS,
                                                            const TargetMachine &TM);
                                                            
            const OneInstrInfo *getInstrInfo() const override{
                return &InstrInfo;
            }

            const OneRegisterInfo *getRegisterInfo() const override{
                return &RegInfo;
            }

            const TargetFrameLowering *getFrameLowering() const override{
                return &FrameLowering;
            }

            void ParseSubtargetFeatures(StringRef CPU, StringRef TuneCPU, StringRef FS);


    };

} // namespace name

#endif

