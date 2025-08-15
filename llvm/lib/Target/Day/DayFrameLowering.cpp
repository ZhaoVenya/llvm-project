#include "DayFrameLowering.h"
#include "DayInstrInfo.h"
#include "DaySubtarget.h"
#include "DayMachineFunctionInfo.h"

using namespace llvm;



static Align getABIStackAlignment() {
  return Align(8);
}

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
                          getABIStackAlignment(),
                          0,
                          getABIStackAlignment(),
                          /*StackRealignable=*/false),STI(ST) {

}


uint64_t DayFrameLowering::computeStackSize(MachineFunction &MF) const {
  uint64_t STACKSIZE = MF.getFrameInfo().getStackSize();
  if (getStackAlignment() > 0) {
    STACKSIZE = ROUND_UP(STACKSIZE, getStackAlignment());
  }
  return STACKSIZE;
}

void DayFrameLowering::emitPrologue(MachineFunction &MF,
                                    MachineBasicBlock &MBB) const {

  MachineBasicBlock::iterator MBBI = MBB.begin();

  const TargetInstrInfo &TII = *STI.getInstrInfo();

  int STACKSIZE = computeStackSize(MF);

  if (STACKSIZE == 0) {
    return;
  }

  DebugLoc DL = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();
  BuildMI(MBB, MBBI, DL, TII.get(Day::ADDI), Day::SP)
      .addReg(Day::SP)
      .addImm(-STACKSIZE)
      .setMIFlag(MachineInstr::FrameSetup);

}



void DayFrameLowering::emitEpilogue(MachineFunction &MF,
                                      MachineBasicBlock &MBB) const {
  MachineBasicBlock::iterator MBBI = MBB.getLastNonDebugInstr();

  const TargetInstrInfo &TII = *STI.getInstrInfo();

  int STACKSIZE = computeStackSize(MF);

  if (STACKSIZE == 0) {
    return;
  }

  DebugLoc DL = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();
  BuildMI(MBB, MBBI, DL, TII.get(Day::ADDI), Day::SP)
      .addReg(Day::SP)
      .addImm(STACKSIZE)
      .setMIFlag(MachineInstr::FrameDestroy);
}


void DayFrameLowering::determineCalleeSaves(MachineFunction &MF, BitVector &SavedRegs,
                            RegScavenger *RS) const {
    TargetFrameLowering::determineCalleeSaves(MF, SavedRegs, RS);
    if (MF.getFrameInfo().hasCalls()) {
        SavedRegs.set(Day::RA);
    }
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