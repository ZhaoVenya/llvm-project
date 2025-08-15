#include "DayCallingConv.h"
#include "DaySubtarget.h"
#include "DayRegisterInfo.h"
#include "MCTargetDesc/DayBaseInfo.h"
#include "llvm/CodeGen/CallingConvLower.h"

using namespace llvm;

#include "DayGenRegisterInfo.inc"


static const MCPhysReg ArgFPRs[] = {
  Day::F00, Day::F01, Day::F02, Day::F03, Day::F04, Day::F05, Day::F06, Day::F07,
  Day::F08, Day::F09, Day::F10, Day::F11, Day::F12, Day::F13, Day::F14, Day::F15,
  Day::F16, Day::F17, Day::F18, Day::F19, Day::F20, Day::F21, Day::F22, Day::F23,
  Day::F24, Day::F25, Day::F26, Day::F27, Day::F28, Day::F29, Day::F30, Day::F31
};


static const MCPhysReg ArgGPRs[] = {
  Day::A0, Day::A1, Day::A2, Day::A3, Day::A4, Day::A5, Day::A6, Day::A7
};

bool llvm::CC_Day(unsigned ValNo, MVT ValVT, MVT LocVT,
              CCValAssign::LocInfo LocInfo, ISD::ArgFlagsTy ArgFlags,
              CCState &State) {
  unsigned XLen = 32;

  if (LocVT == MVT::i32) {
    static const MCPhysReg RegList1[] = {
      Day::A0, Day::A1, Day::A2, Day::A3, Day::A4, Day::A5, Day::A6, Day::A7
    };

    // If this is a variadic argument, the RISC-V calling convention requires
    // that it is assigned an 'even' or 'aligned' register if it has 8-byte
    // alignment (RV32) or 16-byte alignment (RV64). An aligned register should
    // be used regardless of whether the original argument was split during
    // legalisation or not. The argument will not be passed by registers if the
    // original type is larger than 2*XLEN, so the register alignment rule does
    // not apply.
    // TODO: To be compatible with GCC's behaviors, we don't align registers
    // currently if we are using ILP32E calling convention. This behavior may be
    // changed when RV32E/ILP32E is ratified.
    unsigned TwoXLenInBytes = (2 * XLen) / 8;
    if (ArgFlags.getNonZeroOrigAlign() == TwoXLenInBytes) {
      unsigned RegIdx = State.getFirstUnallocated(RegList1);
      // Skip 'odd' register if necessary.
      if (RegIdx != std::size(RegList1) && RegIdx % 2 == 1)
        State.AllocateReg(RegList1);
    }

    if (MCRegister Reg = State.AllocateReg(RegList1)) {
      State.addLoc(CCValAssign::getReg(ValNo, ValVT, Reg, LocVT, LocInfo));
      return false;
    }
  }

  int64_t Offset2 = State.AllocateStack(4, Align(4));
  State.addLoc(CCValAssign::getMem(ValNo, ValVT, Offset2, LocVT, LocInfo));
  return false;

  return true; // CC didn't match.
}

