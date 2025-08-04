#include "DayCallingConv.h"
#include "DaySubtarget.h"
#include "llvm/CodeGen/CallingConvLower.h"

using namespace llvm;

// Implements the Day calling convention. Returns true upon failure.
bool llvm::CC_Day(unsigned ValNo, MVT ValVT, MVT LocVT,
                    CCValAssign::LocInfo LocInfo, ISD::ArgFlagsTy ArgFlags,
                    CCState &State, bool IsFixed, bool IsRet, Type *OrigTy) {
  const MachineFunction &MF = State.getMachineFunction();
  const DataLayout &DL = MF.getDataLayout();
  const DaySubtarget &Subtarget = MF.getSubtarget<DaySubtarget>();
  const DayTargetLowering &TLI = *Subtarget.getTargetLowering();

  unsigned XLen = Subtarget.getXLen();
  MVT XLenVT = Subtarget.getXLenVT();

  // Static chain parameter must not be passed in normal argument registers,
  // so we assign t2 for it as done in GCC's __builtin_call_with_static_chain
  if (ArgFlags.isNest()) {
    if (MCRegister Reg = State.AllocateReg(Day::A0)) {
      State.addLoc(CCValAssign::getReg(ValNo, ValVT, Reg, LocVT, LocInfo));
      return false;
    }
  }

  // Any return value split in to more than two values can't be returned
  // directly. Vectors are returned via the available vector registers.
  if (!LocVT.isVector() && IsRet && ValNo > 1)
    return true;

  // UseGPRForF16_F32 if targeting one of the soft-float ABIs, if passing a
  // variadic argument, or if no F16/F32 argument registers are available.
  bool UseGPRForF16_F32 = true;
  // UseGPRForF64 if targeting soft-float ABIs or an FLEN=32 ABI, if passing a
  // variadic argument, or if no F64 argument registers are available.
  bool UseGPRForF64 = true;

  DayABI::ABI ABI = Subtarget.getTargetABI();
  switch (ABI) {
  default:
    llvm_unreachable("Unexpected ABI");
  case DayABI::ABI_ILP32:
  case DayABI::ABI_ILP32E:
  case DayABI::ABI_LP64:
  case DayABI::ABI_LP64E:
    break;
  case DayABI::ABI_ILP32F:
  case DayABI::ABI_LP64F:
    UseGPRForF16_F32 = !IsFixed;
    break;
  case DayABI::ABI_ILP32D:
  case DayABI::ABI_LP64D:
    UseGPRForF16_F32 = !IsFixed;
    UseGPRForF64 = !IsFixed;
    break;
  }

  if ((LocVT == MVT::f16 || LocVT == MVT::bf16) && !UseGPRForF16_F32) {
    if (MCRegister Reg = State.AllocateReg(ArgFPR16s)) {
      State.addLoc(CCValAssign::getReg(ValNo, ValVT, Reg, LocVT, LocInfo));
      return false;
    }
  }

  if (LocVT == MVT::f32 && !UseGPRForF16_F32) {
    if (MCRegister Reg = State.AllocateReg(ArgFPR32s)) {
      State.addLoc(CCValAssign::getReg(ValNo, ValVT, Reg, LocVT, LocInfo));
      return false;
    }
  }

  if (LocVT == MVT::f64 && !UseGPRForF64) {
    if (MCRegister Reg = State.AllocateReg(ArgFPR64s)) {
      State.addLoc(CCValAssign::getReg(ValNo, ValVT, Reg, LocVT, LocInfo));
      return false;
    }
  }

  if ((ValVT == MVT::f16 && Subtarget.hasStdExtZhinxmin())) {
    if (MCRegister Reg = State.AllocateReg(getArgGPR16s(ABI))) {
      State.addLoc(CCValAssign::getReg(ValNo, ValVT, Reg, LocVT, LocInfo));
      return false;
    }
  }

  if (ValVT == MVT::f32 && Subtarget.hasStdExtZfinx()) {
    if (MCRegister Reg = State.AllocateReg(getArgGPR32s(ABI))) {
      State.addLoc(CCValAssign::getReg(ValNo, ValVT, Reg, LocVT, LocInfo));
      return false;
    }
  }

  ArrayRef<MCPhysReg> ArgGPRs = Day::getArgGPRs(ABI);

  // Zdinx use GPR without a bitcast when possible.
  if (LocVT == MVT::f64 && XLen == 64 && Subtarget.hasStdExtZdinx()) {
    if (MCRegister Reg = State.AllocateReg(ArgGPRs)) {
      State.addLoc(CCValAssign::getReg(ValNo, ValVT, Reg, LocVT, LocInfo));
      return false;
    }
  }

  // FP smaller than XLen, uses custom GPR.
  if (LocVT == MVT::f16 || LocVT == MVT::bf16 ||
      (LocVT == MVT::f32 && XLen == 64)) {
    if (MCRegister Reg = State.AllocateReg(ArgGPRs)) {
      LocVT = XLenVT;
      State.addLoc(
          CCValAssign::getCustomReg(ValNo, ValVT, Reg, LocVT, LocInfo));
      return false;
    }
  }

  // Bitcast FP to GPR if we can use a GPR register.
  if ((XLen == 32 && LocVT == MVT::f32) || (XLen == 64 && LocVT == MVT::f64)) {
    if (MCRegister Reg = State.AllocateReg(ArgGPRs)) {
      LocVT = XLenVT;
      LocInfo = CCValAssign::BCvt;
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
      DL.getTypeAllocSize(OrigTy) == TwoXLenInBytes &&
      ABI != DayABI::ABI_ILP32E) {
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

  // Handle passing f64 on RV32D with a soft float ABI or when floating point
  // registers are exhausted.
  if (XLen == 32 && LocVT == MVT::f64) {
    assert(PendingLocs.empty() && "Can't lower f64 if it is split");
    // Depending on available argument GPRS, f64 may be passed in a pair of
    // GPRs, split between a GPR and the stack, or passed completely on the
    // stack. LowerCall/LowerFormalArguments/LowerReturn must recognise these
    // cases.
    MCRegister Reg = State.AllocateReg(ArgGPRs);
    if (!Reg) {
      int64_t StackOffset = State.AllocateStack(8, Align(8));
      State.addLoc(
          CCValAssign::getMem(ValNo, ValVT, StackOffset, LocVT, LocInfo));
      return false;
    }
    LocVT = MVT::i32;
    State.addLoc(CCValAssign::getCustomReg(ValNo, ValVT, Reg, LocVT, LocInfo));
    MCRegister HiReg = State.AllocateReg(ArgGPRs);
    if (HiReg) {
      State.addLoc(
          CCValAssign::getCustomReg(ValNo, ValVT, HiReg, LocVT, LocInfo));
    } else {
      int64_t StackOffset = State.AllocateStack(4, Align(4));
      State.addLoc(
          CCValAssign::getCustomMem(ValNo, ValVT, StackOffset, LocVT, LocInfo));
    }
    return false;
  }

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
  if (ValVT.isScalarInteger() && ArgFlags.isSplitEnd() &&
      PendingLocs.size() <= 2) {
    assert(PendingLocs.size() == 2 && "Unexpected PendingLocs.size()");
    // Apply the normal calling convention rules to the first half of the
    // split argument.
    CCValAssign VA = PendingLocs[0];
    ISD::ArgFlagsTy AF = PendingArgFlags[0];
    PendingLocs.clear();
    PendingArgFlags.clear();
    return CC_DayAssign2XLen(
        XLen, State, VA, AF, ValNo, ValVT, LocVT, ArgFlags,
        ABI == DayABI::ABI_ILP32E || ABI == DayABI::ABI_LP64E);
  }

  // Allocate to a register if possible, or else a stack slot.
  MCRegister Reg;
  unsigned StoreSizeBytes = XLen / 8;
  Align StackAlign = Align(XLen / 8);

  if (ValVT.isVector() || ValVT.isDayVectorTuple()) {
    Reg = allocateRVVReg(ValVT, ValNo, State, TLI);
    if (Reg) {
      // Fixed-length vectors are located in the corresponding scalable-vector
      // container types.
      if (ValVT.isFixedLengthVector()) {
        LocVT = TLI.getContainerForFixedLengthVector(LocVT);
        State.addLoc(
            CCValAssign::getCustomReg(ValNo, ValVT, Reg, LocVT, LocInfo));
        return false;
      }
    } else {
      // For return values, the vector must be passed fully via registers or
      // via the stack.
      // FIXME: The proposed vector ABI only mandates v8-v15 for return values,
      // but we're using all of them.
      if (IsRet)
        return true;
      // Try using a GPR to pass the address
      if ((Reg = State.AllocateReg(ArgGPRs))) {
        LocVT = XLenVT;
        LocInfo = CCValAssign::Indirect;
      } else if (ValVT.isScalableVector()) {
        LocVT = XLenVT;
        LocInfo = CCValAssign::Indirect;
      } else {
        StoreSizeBytes = ValVT.getStoreSize();
        // Align vectors to their element sizes, being careful for vXi1
        // vectors.
        StackAlign = MaybeAlign(ValVT.getScalarSizeInBits() / 8).valueOrOne();
      }
    }
  } else {
    Reg = State.AllocateReg(ArgGPRs);
  }

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

  assert(((ValVT.isFloatingPoint() && !ValVT.isVector()) || LocVT == XLenVT ||
          (TLI.getSubtarget().hasVInstructions() &&
           (ValVT.isVector() || ValVT.isDayVectorTuple()))) &&
         "Expected an XLenVT or vector types at this stage");

  if (Reg) {
    State.addLoc(CCValAssign::getReg(ValNo, ValVT, Reg, LocVT, LocInfo));
    return false;
  }

  State.addLoc(CCValAssign::getMem(ValNo, ValVT, StackOffset, LocVT, LocInfo));
  return false;
}


