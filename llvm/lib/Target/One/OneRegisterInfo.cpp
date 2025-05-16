


#include "OneRegisterInfo.h"
#include "MCTargetDesc/OneMCTargetDesc.h"
#include "llvm/ADT/SmallSet.h"
#include "OneSubtarget.h"


using namespace llvm;

#define GET_REGINFO_TARGET_DESC
#include "OneGenRegisterInfo.inc"


OneRegisterInfo::OneRegisterInfo(const OneSubtarget &STI) : OneGenRegisterInfo(One::RA),STI(STI){

}

const MCPhysReg *OneRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {

    return CC_CSR_SaveList;
}


const uint32_t *OneRegisterInfo::getCallPreservedMask(const MachineFunction &MF, CallingConv::ID) const {
    
}



BitVector OneRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
    BitVector Reserved(getNumRegs());
    Reserved.set(One::ZERO);
    Reserved.set(One::RA);
    Reserved.set(One::SP);

    return Reserved;
}



bool OneRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II, int SPAdj,
                        unsigned FIOperandNum,
                        RegScavenger *RS) const {
    return false;
}



Register OneRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
    return One::SP;
}


