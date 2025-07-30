#ifndef DAYREGISTERINFO_H_
#define DAYREGISTERINFO_H_

#include "DayTargetMachine.h"

#include "llvm/CodeGen/TargetRegisterInfo.h"

#define GET_REGINFO_HEADER
#include "DayGenRegisterInfo.inc"

namespace llvm {
struct DayRegisterInfo : public DayGenRegisterInfo {
  DayRegisterInfo();

  /// Code Generation virtual methods...
  const MCPhysReg *getCalleeSavedRegs(const MachineFunction *MF) const override;
  const uint32_t *getCallPreservedMask(const MachineFunction &MF,
                                       CallingConv::ID CC) const override;


  BitVector getReservedRegs(const MachineFunction &MF) const override;
  bool isReservedReg(const MachineFunction &MF, MCRegister Reg) const;

  bool eliminateFrameIndex(MachineBasicBlock::iterator II,
                           int SPAdj, unsigned FIOperandNum,
                           RegScavenger *RS = nullptr) const override;

  Register getFrameRegister(const MachineFunction &MF) const override;
};

} // end namespace llvm


#endif
