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
                          Align(8), 0, Align(8),
                          /*StackRealignable=*/false),STI(ST) {

}


void DayFrameLowering::emitPrologue(MachineFunction &MF,
                                      MachineBasicBlock &MBB) const {

    MachineFrameInfo &MFI = MF.getFrameInfo();
    auto *RVFI = MF.getInfo<DayMachineFunctionInfo>();
    const DayRegisterInfo *RI = static_cast<const DayRegisterInfo *>(STI.getRegisterInfo());
    const DayInstrInfo *TII = STI.getInstrInfo();
    MachineBasicBlock::iterator MBBI = MBB.begin();


}


void DayFrameLowering::emitEpilogue(MachineFunction &MF,
                                      MachineBasicBlock &MBB) const {

}


void DayFrameLowering::determineCalleeSaves(MachineFunction &MF, BitVector &SavedRegs,
                            RegScavenger *RS) const {
    TargetFrameLowering::determineCalleeSaves(MF, SavedRegs, RS);  
//     if (hasFP(MF)) {
//         SavedRegs.set(RAReg);
//         SavedRegs.set(FPReg);
//     }
//     // Mark BP as used if function has dedicated base pointer.
//     if (hasBP(MF))
//         SavedRegs.set(RISCVABI::getBPReg());

//   // When using cm.push/pop we must save X27 if we save X26.
//     auto *RVFI = MF.getInfo<RISCVMachineFunctionInfo>();
//     if (RVFI->isPushable(MF) && SavedRegs.test(RISCV::X26))
//         SavedRegs.set(RISCV::X27);
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