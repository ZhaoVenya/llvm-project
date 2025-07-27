#ifndef DAY_SUBTARGET_H
#define DAY_SUBTARGET_H


#include "llvm/CodeGen/TargetSubtargetInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
// #include "DayFrameLowering.h"
// #include "DayISelLowering.h"
#include "DayInstrInfo.h"

#include "DayGenSubtargetInfo.inc"

namespace llvm{
class DaySubtarget : public DayGenSubtargetInfo{

    DayInstrInfo InstrInfo;
    // DayTargetLowering TLInfo;
    // SelectionDAGTargetInfo TSInfo;
    // DayFrameLowering FrameLowering;

public:
    DaySubtarget(const StringRef &CPU, const StringRef &TuneCPU,
                    const StringRef &FS, const TargetMachine &TM, bool is64bit);

    const DayInstrInfo *getInstrInfo() const override { return &InstrInfo; }
    // const TargetFrameLowering *getFrameLowering() const override {
    //     return &FrameLowering;
    // }
    // const DayRegisterInfo *getRegisterInfo() const override {
    //     return &InstrInfo.getRegisterInfo();
    // }
    // const DayTargetLowering *getTargetLowering() const override {
    //     return &TLInfo;
    // }
    // const SelectionDAGTargetInfo *getSelectionDAGInfo() const override {
    //     return &TSInfo;
    // }
};
}

#endif
