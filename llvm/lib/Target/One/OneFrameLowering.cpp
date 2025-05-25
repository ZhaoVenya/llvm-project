//
// Created by 蔡鹏 on 2024/12/1.
//

#include "OneFrameLowering.h"
#include "MCTargetDesc/OneMCTargetDesc.h"

#include "One.h"
#include "OneSubtarget.h"

#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"

using namespace llvm;


//  这个函数计算当前 MachineFunction（机器函数）需要使用的总栈帧大小。
uint64_t OneFrameLowering::computeStateSize(MachineFunction &MF) const {
    uint64_t STACKSIZE = MF.getFrameInfo().getStackSize();
    if (getStackAlignment() > 0) {
        STACKSIZE = ROUND_UP(STACKSIZE, getStackAlignment());
    }
    return STACKSIZE;
}


// 这个函数负责在函数的**入口（prologue）**处生成机器指令。函数序言通常包括：
// 调整栈指针以分配栈帧空间。
// 保存被调用者保存寄存器（Callee-Saved Registers）。
// 设置帧指针（如果使用）。
void OneFrameLowering::emitPrologue(MachineFunction &MF,
                                    MachineBasicBlock &MBB) const {
    MachineBasicBlock::iterator MBBI = MBB.begin();

    const TargetInstrInfo &TII = *STI.getInstrInfo();

    int STACKSIZE = computeStateSize(MF);

    if (STACKSIZE == 0) {
        return;
    }

    DebugLoc DL = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();
    
    BuildMI(MBB, MBBI, DL, TII.get(One::ADDI), One::SP)
        .addReg(One::SP)
        .addImm(-STACKSIZE)
        .setMIFlag(MachineInstr::FrameSetup);

}


// 这个函数负责在函数的**出口（epilogue）**处生成机器指令。函数尾声通常包括：
// 恢复被调用者保存寄存器。
// 调整栈指针以释放栈帧空间。
// 跳转回调用者。
void OneFrameLowering::emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const {
    MachineBasicBlock::iterator MBBI = MBB.getLastNonDebugInstr();

    const TargetInstrInfo &TII = *STI.getInstrInfo();

    int STACKSIZE = computeStateSize(MF);

    if (STACKSIZE == 0) {
      return;
    }

    DebugLoc DL = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();
    BuildMI(MBB, MBBI, DL, TII.get(One::ADDI), One::SP)
        .addReg(One::SP)
        .addImm(STACKSIZE)
        .setMIFlag(MachineInstr::FrameDestroy);
}



// 这个函数负责确定当前函数需要保存哪些被调用者保存寄存器（Callee-Saved Registers）
void OneFrameLowering::determineCalleeSaves(MachineFunction &MF, BitVector &SavedRegs, RegScavenger *RS) const {
    TargetFrameLowering::determineCalleeSaves(MF, SavedRegs, RS);
    if (MF.getFrameInfo().hasCalls()) {
        SavedRegs.set(One::RA);
    }
}


// 这个函数是一个查询方法，
// 用于告知 LLVM 当前目标机器在给定函数中是否使用了一个专门的帧指针（Frame Pointer, FP）
bool OneFrameLowering::hasFPImpl(const MachineFunction &MF) const {
    return false;
}