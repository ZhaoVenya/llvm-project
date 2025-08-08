#ifndef DAY_SUBTARGET_H
#define DAY_SUBTARGET_H


#include "llvm/CodeGen/TargetSubtargetInfo.h"
#include "llvm/CodeGen/SelectionDAGTargetInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/TargetParser/Triple.h"

#include "DayFrameLowering.h"
#include "DayISelLowering.h"
#include "DayInstrInfo.h"
#include "DayRegisterInfo.h"
// #include "DayTargetMachine.h"
#include "MCTargetDesc/DayBaseInfo.h"

#define GET_SUBTARGETINFO_HEADER
#include "DayGenSubtargetInfo.inc"

namespace llvm{
class DaySubtarget : public DayGenSubtargetInfo{

    // const DayTargetMachine &TM;
    
    virtual void anchor();
    
    BitVector ReserveRegister;
    Triple TargetTriple;
    bool Is64Bit;


    DayFrameLowering FrameLowering;
    DayInstrInfo InstrInfo;
    DayRegisterInfo RegInfo;
    DayTargetLowering TLInfo;
    SelectionDAGTargetInfo TSInfo;

public:
    DaySubtarget(const StringRef &CPU, const StringRef &TuneCPU,
                    const StringRef &FS, const TargetMachine &TM, bool is64bit);

    const DayInstrInfo *getInstrInfo() const override {
        return &InstrInfo; }

    const TargetFrameLowering *getFrameLowering() const override {
        return &FrameLowering;
    }

    const DayRegisterInfo *getRegisterInfo() const override {
        return &RegInfo;
    }

    const DayTargetLowering *getTargetLowering() const override {
        return &TLInfo;
    }

    const SelectionDAGTargetInfo *getSelectionDAGInfo() const override {
        return &TSInfo;
    }

    void ParseSubtargetFeatures(StringRef CPU, StringRef TuneCPU, StringRef FS);
    DaySubtarget &initializeSubtargetDependencies(StringRef CPU,
                                                  StringRef TuneCPU,
                                                  StringRef FS);

    bool is64Bit() const { return Is64Bit;}


    MVT getXLenVT() const {
        return MVT::i32;
    }

    unsigned getXLen() const {
        return 32;
    }

    DayABI::ABI getTargetABI() const { return DayABI::ABI_ILP32F; }

};
}

#endif
