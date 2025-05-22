


#ifndef ONEINSTSRINFO_H
#define ONEINSTSRINFO_H

#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "OneGenInstrInfo.inc"

namespace llvm {
class OneInstrInfo : public OneGenInstrInfo {
public:
  explicit OneInstrInfo();

  /// 处理callee
  void storeRegToStackSlot(MachineBasicBlock &MBB, MachineBasicBlock::iterator MI, 
                          Register SrcReg, bool isKill, int FrameIndex, 
                          const TargetRegisterClass *RC, 
                          const TargetRegisterInfo *TRI, Register VReg,
                          MachineInstr::MIFlag Flags) const override;

  void loadRegFromStackSlot(MachineBasicBlock &MBB, MachineBasicBlock::iterator MI, 
                            Register DestReg, int FrameIndex, 
                            const TargetRegisterClass *RC, 
                            const TargetRegisterInfo *TRI, Register VReg,
                            MachineInstr::MIFlag Flags) const override;

  void copyPhysReg(MachineBasicBlock &MBB, MachineBasicBlock::iterator MI, 
                  const DebugLoc &DL, MCRegister DestReg, 
                  MCRegister SrcReg, bool KillSrc, 
                  bool RenamableDest, bool RenamableSrc) const override;
};
} // namespace llvm



#endif
