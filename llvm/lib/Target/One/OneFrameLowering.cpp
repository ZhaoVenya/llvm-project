#include "OneFrameLowering.h"
#include "OneSubtarget.h"
#include "OneInstrInfo.h"
#include "MCTargetDesc/OneMCTargetDesc.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/IR/DebugLoc.h"
#include <cstdint>
#include "One.h"

using namespace llvm;


uint64_t OneFrameLowering::computeStateSize(MachineFunction &MF) const{
    uint64_t STACKSIZE = MF.getFrameInfo().getStackSize();
    if(getStackAlignment()>0){
        STACKSIZE = ROUND_UP(STACKSIZE, getStackAlignment());
    }

    return STACKSIZE;
}


void OneFrameLowering::emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const {

    MachineBasicBlock::iterator MBBI = MBB.begin();
    const TargetInstrInfo &TII = *STI.getInstrInfo();
    DebugLoc DL= MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();

    uint64_t STACKSIZE = computeStateSize(MF);

    if(STACKSIZE == 0){
        return;
    }

    BuildMI(MBB,MBBI,DL,TII.get(One::ADDI), One::SP)
        .addReg(One::SP)
        .addImm(-STACKSIZE)
        .setMIFlag(MachineInstr::FrameSetup);
}

void OneFrameLowering::emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const {
    MachineBasicBlock::iterator MBBI = MBB.getLastNonDebugInstr();
    const TargetInstrInfo &TII = *STI.getInstrInfo();
    DebugLoc DL= MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();

    uint64_t STACKSIZE = computeStateSize(MF);

    if(STACKSIZE == 0){
        return;
    }

    BuildMI(MBB,MBBI,DL,TII.get(One::ADDI), One::SP)
        .addReg(One::SP)
        .addImm(STACKSIZE)
        .setMIFlag(MachineInstr::FrameDestroy);


}


bool OneFrameLowering::hasFPImpl(const MachineFunction &MF) const{
    return false;
}



