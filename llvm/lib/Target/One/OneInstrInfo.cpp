//
// Created by 蔡鹏 on 2024/12/1.
//

#include "OneInstrInfo.h"
#include "MCTargetDesc/OneMCTargetDesc.h"

using namespace llvm;

#define GET_INSTRINFO_CTOR_DTOR
#include "OneGenInstrInfo.inc"

OneInstrInfo::OneInstrInfo() : OneGenInstrInfo() {}

// 这个函数负责生成一条机器指令，
// 将一个寄存器（SrcReg）的值存储到栈上的一个指定位置（FrameIndex）。
// 这个过程通常被称为溢出（Spill），发生在寄存器不够用，
// 需要把某个寄存器的值暂时存到内存中时。
void OneInstrInfo::storeRegToStackSlot(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator MI, Register SrcReg,
    bool isKill, int FrameIndex, const TargetRegisterClass *RC,
    const TargetRegisterInfo *TRI, Register VReg) const {
  DebugLoc DL;
  BuildMI(MBB, MI, DL, get(One::STOREWFI))
      .addReg(SrcReg, getKillRegState(isKill))
      .addFrameIndex(FrameIndex)
      .addImm(0);
}


// 这个函数负责生成一条机器指令，
// 将栈上的一个指定位置（FrameIndex）的值加载到一个寄存器（DestReg）中。
// 这个过程通常被称为重载（Reload），
// 发生在之前被溢出到内存中的值需要被重新载入寄存器中时。
void OneInstrInfo::loadRegFromStackSlot(MachineBasicBlock &MBB,
                                        MachineBasicBlock::iterator MI,
                                        Register DestReg, int FrameIndex,
                                        const TargetRegisterClass *RC,
                                        const TargetRegisterInfo *TRI,
                                        Register VReg) const {
  DebugLoc DL;
  BuildMI(MBB, MI, DL, get(One::LOADWFI), DestReg)
      .addFrameIndex(FrameIndex)
      .addImm(0);
}


// 这个函数负责生成一条机器指令，将一个物理寄存器（SrcReg）的值拷贝
// 到另一个物理寄存器（DestReg）。
void OneInstrInfo::copyPhysReg(MachineBasicBlock &MBB, MachineBasicBlock::iterator MI, const DebugLoc &DL, MCRegister DestReg, MCRegister SrcReg, bool KillSrc, bool RenamableDest, bool RenamableSrc) const {
  /// add dst, zero, src
  MachineInstrBuilder MIB = BuildMI(MBB, MI, DL, get(One::ADD));
  MIB.addReg(DestReg, RegState::Define);
  MIB.addReg(One::ZERO);
  MIB.addReg(SrcReg, getKillRegState(KillSrc));
}