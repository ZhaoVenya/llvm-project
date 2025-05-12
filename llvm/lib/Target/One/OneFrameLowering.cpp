#include "OneFrameLowering.h"

using namespace llvm;


void OneFrameLowering::emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const {


}

void OneFrameLowering::emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const {

}


bool OneFrameLowering::hasFPImpl(const MachineFunction &MF) const{
    return false;
}



