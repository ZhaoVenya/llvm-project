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
                    
}


void DayFrameLowering::emitEpilogue(MachineFunction &MF,
                                      MachineBasicBlock &MBB) const {

}


bool DayFrameLowering::hasFPImpl(const MachineFunction &MF) const{

    return false;
}