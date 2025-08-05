#include "DayCallingConv.h"
#include "DaySubtarget.h"
#include "DayRegisterInfo.h"
#include "MCTargetDesc/DayBaseInfo.h"
#include "llvm/CodeGen/CallingConvLower.h"

using namespace llvm;

static const MCPhysReg ArgFPRs[] = {
  Day::F00, Day::F01, Day::F02, Day::F03, Day::F04, Day::F05, Day::F06, Day::F07,
  Day::F08, Day::F09, Day::F10, Day::F11, Day::F12, Day::F13, Day::F14, Day::F15,
  Day::F16, Day::F17, Day::F18, Day::F19, Day::F20, Day::F21, Day::F22, Day::F23,
  Day::F24, Day::F25, Day::F26, Day::F27, Day::F28, Day::F29, Day::F30, Day::F31
};


static const MCPhysReg ArgGPRs[] = {
  Day::A0, Day::A1, Day::A2, Day::A3, Day::A4, Day::A5, Day::A6, Day::A7
};

// Implements the Day calling convention. Returns true upon failure.
bool llvm::CC_Day(unsigned ValNo, MVT ValVT, MVT LocVT,
                    CCValAssign::LocInfo LocInfo, ISD::ArgFlagsTy ArgFlags,
                    CCState &State, bool IsFixed, bool IsRet, Type *OrigTy) {
  const MachineFunction &MF = State.getMachineFunction();
  const DataLayout &DL = MF.getDataLayout();
  const DaySubtarget &Subtarget = MF.getSubtarget<DaySubtarget>();
  // const DayTargetLowering &TLI = static_cast<const DayTargetLowering &>(*Subtarget.getTargetLowering());

  unsigned XLen = Subtarget.getXLen();
  MVT XLenVT = Subtarget.getXLenVT();

  // Static chain parameter must not be passed in normal argument registers,
  // so we assign t2 for it as done in GCC's __builtin_call_with_static_chain
  // 判断当前参数是否具有静态链(static chine)的属性,
  // 如果具有静态链属性就会把这个参数分配到一个临时寄存器T2中.
  if (ArgFlags.isNest()) {
    if (MCRegister Reg = State.AllocateReg(Day::T2)) {
      State.addLoc(CCValAssign::getReg(ValNo, ValVT, Reg, LocVT, LocInfo));
      return false;
    }
  }

  // Any return value split in to more than two values can't be returned
  // directly. Vectors are returned via the available vector registers.
  // 数据类型是vector,还是返回数值,并且返回数值的编号大于1(也就是至少被拆分了3部分).
  if (!LocVT.isVector() && IsRet && ValNo > 1)
    return true;

  // UseGPRForF16_F32 if targeting one of the soft-float ABIs, if passing a
  // variadic argument, or if no F16/F32 argument registers are available.
  bool UseGPRForF32 = true;
  // UseGPRForF64 if targeting soft-float ABIs or an FLEN=32 ABI, if passing a
  // variadic argument, or if no F64 argument registers are available.
  // bool UseGPRForF64 = true;

  DayABI::ABI ABI = Subtarget.getTargetABI();
  switch (ABI) {
  default:
    llvm_unreachable("Unexpected ABI");
  case DayABI::ABI_ILP32F:
    UseGPRForF32 = !IsFixed;
    break;
  }


  if (LocVT == MVT::f32 && !UseGPRForF32) {
    if (MCRegister Reg = State.AllocateReg(ArgFPRs)) {
      State.addLoc(CCValAssign::getReg(ValNo, ValVT, Reg, LocVT, LocInfo));
      return false;
    }
  }


  // If this is a variadic argument, the Day calling convention requires
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
  if (!IsFixed && ArgFlags.getNonZeroOrigAlign() == TwoXLenInBytes &&
      DL.getTypeAllocSize(OrigTy) == TwoXLenInBytes) {

    unsigned RegIdx = State.getFirstUnallocated(ArgGPRs);
    // Skip 'odd' register if necessary.
    if (RegIdx != std::size(ArgGPRs) && RegIdx % 2 == 1)
      State.AllocateReg(ArgGPRs);
  }

  SmallVectorImpl<CCValAssign> &PendingLocs = State.getPendingLocs();
  SmallVectorImpl<ISD::ArgFlagsTy> &PendingArgFlags =
      State.getPendingArgFlags();

  assert(PendingLocs.size() == PendingArgFlags.size() &&
         "PendingLocs and PendingArgFlags out of sync");


  // Split arguments might be passed indirectly, so keep track of the pending
  // values. Split vectors are passed via a mix of registers and indirectly, so
  // treat them as we would any other argument.
  if (ValVT.isScalarInteger() && (ArgFlags.isSplit() || !PendingLocs.empty())) {
    LocVT = XLenVT;
    LocInfo = CCValAssign::Indirect;
    PendingLocs.push_back(
        CCValAssign::getPending(ValNo, ValVT, LocVT, LocInfo));
    PendingArgFlags.push_back(ArgFlags);
    if (!ArgFlags.isSplitEnd()) {
      return false;
    }
  }

  // If the split argument only had two elements, it should be passed directly
  // in registers or on the stack.
  // 
  // if (ValVT.isScalarInteger() && ArgFlags.isSplitEnd() &&
  //     PendingLocs.size() <= 2) {
  //   assert(PendingLocs.size() == 2 && "Unexpected PendingLocs.size()");
  //   // Apply the normal calling convention rules to the first half of the
  //   // split argument.
  //   CCValAssign VA = PendingLocs[0];
  //   ISD::ArgFlagsTy AF = PendingArgFlags[0];
  //   PendingLocs.clear();
  //   PendingArgFlags.clear();
  //   return CC_DayAssign2XLen(
  //       XLen, State, VA, AF, ValNo, ValVT, LocVT, ArgFlags,
  //       ABI == DayABI::ABI_ILP32E || ABI == DayABI::ABI_LP64E);
  // }

  // Allocate to a register if possible, or else a stack slot.
  MCRegister Reg;
  unsigned StoreSizeBytes = XLen / 8;
  Align StackAlign = Align(XLen / 8);

  Reg = State.AllocateReg(ArgGPRs);

  int64_t StackOffset =
      Reg ? 0 : State.AllocateStack(StoreSizeBytes, StackAlign);

  // If we reach this point and PendingLocs is non-empty, we must be at the
  // end of a split argument that must be passed indirectly.
  if (!PendingLocs.empty()) {
    assert(ArgFlags.isSplitEnd() && "Expected ArgFlags.isSplitEnd()");
    assert(PendingLocs.size() > 2 && "Unexpected PendingLocs.size()");

    for (auto &It : PendingLocs) {
      if (Reg)
        It.convertToReg(Reg);
      else
        It.convertToMem(StackOffset);
      State.addLoc(It);
    }
    PendingLocs.clear();
    PendingArgFlags.clear();
    return false;
  }

  assert(((ValVT.isFloatingPoint() && !ValVT.isVector()) || LocVT == XLenVT) &&
         "Expected an XLenVT or vector types at this stage");

  if (Reg) {
    State.addLoc(CCValAssign::getReg(ValNo, ValVT, Reg, LocVT, LocInfo));
    return false;
  }

  State.addLoc(CCValAssign::getMem(ValNo, ValVT, StackOffset, LocVT, LocInfo));
  return false;
}


