#include "DayFrameLowering.h"
#include "DaySubtarget.h"

using namespace llvm;

DayFrameLowering::DayFrameLowering(const DaySubtarget &ST)
    : TargetFrameLowering(TargetFrameLowering::StackGrowsDown,
                          ST.is64Bit() ? Align(16) : Align(8), 0,
                          ST.is64Bit() ? Align(16) : Align(8),
                          /*StackRealignable=*/false) {

}


void DayFrameLowering::emitPrologue(MachineFunction &MF,
                                      MachineBasicBlock &MBB) const {

    MachineFrameInfo &MFI = MF.getFrameInfo();
    auto *RVFI = MF.getInfo<DayMachineFunctionInfo>();
    const DayRegisterInfo *RI = STI.getRegisterInfo();
    const DayInstrInfo *TII = STI.getInstrInfo();
    MachineBasicBlock::iterator MBBI = MBB.begin();


}


void DayFrameLowering::emitEpilogue(MachineFunction &MF,
                                      MachineBasicBlock &MBB) const {

}


void DayFrameLowering::determineCalleeSaves(MachineFunction &MF, BitVector &SavedRegs,
                            RegScavenger *RS = nullptr) const {

}


StackOffset DayFrameLowering::getFrameIndexReference(const MachineFunction &MF, int FI,
                                     Register &FrameReg) const {

}

MachineBasicBlock::iterator DayFrameLowering::eliminateCallFramePseudoInstr(MachineFunction &MF,
                                MachineBasicBlock &MBB,
                                MachineBasicBlock::iterator I) const {
    
}


bool DayFrameLowering::hasFPImpl(const MachineFunction &MF) const{

    return false;
}