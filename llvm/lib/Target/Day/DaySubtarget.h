#ifndef DAY_SUBTARGET_H
#define DAY_SUBTARGET_H


#include "llvm/CodeGen/TargetSubtargetInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/TargetParser/Triple.h"

#include "DayISelLowering.h"
#include "DayInstrInfo.h"

#define GET_SUBTARGETINFO_HEADER
#include "DayGenSubtargetInfo.inc"

namespace llvm{
class DaySubtarget : public DayGenSubtargetInfo{

    virtual void anchor();
    bool Is64Bit;
    BitVector ReserveRegister;
    Triple TargetTriple;
    
    DayInstrInfo InstrInfo;
    DayTargetLowering TLInfo;
    DayFrameLowering FrameLowering;

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
    void ParseSubtargetFeatures(StringRef CPU, StringRef TuneCPU, StringRef FS);
    DaySubtarget &initializeSubtargetDependencies(StringRef CPU,
                                                  StringRef TuneCPU,
                                                  StringRef FS);

    bool is64Bit() const { return Is64Bit;}

};
}

#endif
