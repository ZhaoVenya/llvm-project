//
// Created by 蔡鹏 on 2024/12/1.
//

#include "OneRegisterInfo.h"
#include "MCTargetDesc/OneMCTargetDesc.h"
#include "One.h"
#include "OneSubtarget.h"

#include "llvm/CodeGen/MachineFrameInfo.h"

using namespace llvm;

#define GET_REGINFO_TARGET_DESC
#include "OneGenRegisterInfo.inc"

#define DEBUG_TYPE "one register info"

// 这个基类的构造函数通常需要一个参数，表示返回地址寄存器（Return Address Register）。
// 在这里，指定 One::RA 为返回地址寄存器
// 这个调用初始化了所有由 TableGen 生成的关于寄存器、寄存器类、寄存器别名等元数据。
// 它提供了访问这些自动生成信息的基础设施。
OneRegisterInfo::OneRegisterInfo(const OneSubtarget &STI) : OneGenRegisterInfo(One::RA), STI(STI) {
    
}

// 这个函数返回一个指向 "被调用者保存寄存器（Callee-Saved Registers，
// 也称为 Non-Volatile Registers)" 列表的指针。 
// 根据调用约定（Calling Convention），这些寄存器在函数调用过程中，
// 如果被被调用函数（callee）修改了，被调用函数有责任在返回前将其原始值恢复。
// 调用者（caller）不负责保存这些寄存器。
const MCPhysReg *OneRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
    return CSR_SaveList;
}

// 这个函数返回一个指向**调用保留掩码（Call Preserved Mask）**的指针。
// 这是一个位掩码，表示在一次函数调用后，
// 哪些寄存器（通常是调用者保存寄存器和被调用者保存寄存器的组合）
// 的值在被调用函数返回后依然有效（保持不变）。在寄存器分配和调用约定处理中，
// 这个掩码用于确定哪些寄存器在函数调用前后不会被修改，从而避免不必要的保存/恢复操作。
const uint32_t *OneRegisterInfo::getCallPreservedMask(const MachineFunction &MF,
                                     CallingConv::ID) const {
    return CSR_RegMask;
}

// 这个函数返回一个 BitVector，其中标记了**被保留的（Reserved）**物理寄存器。
//  这些寄存器不能被寄存器分配器随意使用。它们通常有特定的用途，例如：
// One::ZERO： 零寄存器，可能是一个硬编码为 0 的寄存器，不能被程序修改。
// One::RA： 返回地址寄存器，用于保存函数返回地址。
// One::SP： 栈指针寄存器，用于管理函数栈帧。
// 寄存器分配器在分配物理寄存器时，会查询这个列表，
// 避免将这些特殊用途的寄存器分配给普通的变量。
BitVector OneRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
    BitVector Reserved(getNumRegs());
    Reserved.set(One::ZERO);
    Reserved.set(One::RA);
    Reserved.set(One::SP);
    return Reserved;
}

// 这是 RegisterInfo 类中一个非常关键且复杂的函数。它的任务是消除帧索引（Frame Index, FI）。
bool OneRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                          int SPAdj, unsigned FIOperandNum,
                                          RegScavenger *RS) const {
    MachineInstr &MI = *II;
    LLVM_DEBUG(errs() << MI);

    /// MI (Reg, FrameIndex, IMM)

    uint I = 0;
    while (!MI.getOperand(I).isFI()) {
        ++I;
        assert(I < MI.getNumOperands());
    }

    const int FI = MI.getOperand(I).getIndex();

    /// 根据 index -> 函数栈帧内部的偏移量
    const MachineFunction &MF = *MI.getParent()->getParent();
    const MachineFrameInfo &MFI = MF.getFrameInfo();
    int64_t Offset = MFI.getObjectOffset(FI);
    uint64_t STACKSIZE = ROUND_UP(MFI.getStackSize(), STI.getFrameLowering()->getStackAlignment());
    Offset += static_cast<int64_t>(STACKSIZE);

    int64_t O = MI.getOperand(I + 1).getImm();
    Offset += O;
    MI.getOperand(I).ChangeToRegister(One::SP, false);
    MI.getOperand(I + 1).ChangeToImmediate(Offset);
    // MI.getOperand(I + 1).setImm(Offset);
    return true;
}

// 这个函数返回当前函数所使用的帧寄存器（Frame Register）.
Register OneRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
    return One::SP;
}