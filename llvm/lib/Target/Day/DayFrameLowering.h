#ifndef DAY_FRAME_LOWERING_H_
#define DAY_FRAME_LOWERING_H_

#include "Day.h"
#include "llvm/CodeGen/TargetFrameLowering.h"

namespace llvm{

class DaySubtarget;

class DayFrameLowering : public TargetFrameLowering {
  
public:
  explicit DayFrameLowering(const DaySubtarget &ST);

  // 这个是序言插入实现对应的方法
  void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
  // 这个是尾声插入实现对应的方法
  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
  // 这个函数确定哪些寄存器
  void determineCalleeSaves(MachineFunction &MF, BitVector &SavedRegs,
                            RegScavenger *RS = nullptr) const override;
  
  StackOffset getFrameIndexReference(const MachineFunction &MF, int FI,
                                     Register &FrameReg) const override;
  
  MachineBasicBlock::iterator eliminateCallFramePseudoInstr(MachineFunction &MF,
                                MachineBasicBlock &MBB,
                                MachineBasicBlock::iterator I) const override;

protected:
  const DaySubtarget &STI;
  // 这个函数返回的是一个bool值，用来表示当前函数时都需要实现帧指针
  // 帧指针相对于栈指针，基地址比较稳定
  bool hasFPImpl(const MachineFunction &MF) const override;




};

}

#endif
