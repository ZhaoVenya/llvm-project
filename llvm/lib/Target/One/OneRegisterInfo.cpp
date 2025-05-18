


#include "OneRegisterInfo.h"
#include "MCTargetDesc/OneMCTargetDesc.h"
#include "llvm/ADT/SmallSet.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/Support/raw_ostream.h"
#include <cstdint>
#include "OneSubtarget.h"
#include "One.h"


using namespace llvm;

#define GET_REGINFO_TARGET_DESC
#include "OneGenRegisterInfo.inc"

#define DEBUG_TYPE "one register info"


OneRegisterInfo::OneRegisterInfo(const OneSubtarget &STI) : 
    OneGenRegisterInfo(One::RA),STI(STI){

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
    
    MachineInstr &MI = *II;
    LLVM_DEBUG(errs()<<MI);

    // int opNum = MI.getNumDebugOperands();
    // for (int i = 0; i < opNum; i++){
    //     errs()<<(int) MI.getOperand(i).getType()<<"\n";
    // }

    uint I = 0;
    while(!MI.getOperand(I).isFI()){
        ++I;
        assert(I < MI.getNumOperands());
    }

    const int FI = MI.getOperand(I).getIndex();
    
    const MachineFunction &MF = *MI.getParent()->getParent();
    const MachineFrameInfo &MFI = MF.getFrameInfo();

    int64_t Offset = MFI.getObjectOffset(FI);
    uint64_t STACKSIZE = ROUND_UP(MFI.getStackSize(), STI.getFrameLowering()->getStackAlignment());
    Offset += static_cast<int64_t>(STACKSIZE);

    MI.getOperand(I).ChangeToRegister(One::SP, false);
    // MI.getOperand(I+1).ChangeToImmediate(Offset);
    MI.getOperand(I+1).setImm(Offset);

    return true;
}



Register OneRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
    return One::SP;
}


