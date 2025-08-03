#include "DayFrameLowering.h"
#include "DayInstrInfo.h"
#include "DaySubtarget.h"
#include "DayMachineFunctionInfo.h"

using namespace llvm;


/*
 DayFrameLowering class的构造函数，继承 TargetFrameLowering.

 TargetFrameLowering的构造函数有几个入口参数：

 StackDirection D: 该架构对应栈的生长方式，一般都是向下生长。
 Align StackAl：栈帧的对齐方式
 int LAO: 这个参数代表了本地变量区域的偏移量（Local Area Offset）。
 Align TransAl: 这个参数是临时堆栈区域的对齐方式.
 bool StackReal: 这个参数是一个布尔值，用于指示栈是否可以重新对齐.

*/
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