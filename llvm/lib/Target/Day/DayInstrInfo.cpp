#include "DayInstrInfo.h"
#include "Day.h"
#include "DayMachineFunctionInfo.h"
#include "DaySubtarget.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineMemOperand.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

#define GET_INSTRINFO_CTOR_DTOR
#include "DayGenInstrInfo.inc"



static cl::opt<unsigned> BPccDisplacementBits(
    "day-bpcc-offset-bits", cl::Hidden, cl::init(19),
    cl::desc("Restrict range of BPcc/FBPfcc instructions (DEBUG)"));

static cl::opt<unsigned>
    BPrDisplacementBits("day-bpr-offset-bits", cl::Hidden, cl::init(16),
                        cl::desc("Restrict range of BPr instructions (DEBUG)"));

// Pin the vtable to this file.
void DayInstrInfo::anchor() {}

DayInstrInfo::DayInstrInfo(DaySubtarget &ST)
    : DayGenInstrInfo(SP::ADJCALLSTACKDOWN, SP::ADJCALLSTACKUP), RI(),
      Subtarget(ST) {}

/// isLoadFromStackSlot - If the specified machine instruction is a direct
/// load from a stack slot, return the virtual or physical register number of
/// the destination along with the FrameIndex of the loaded stack slot.  If
/// not, return 0.  This predicate must return 0 if the instruction has
/// any side effects other than loading from the stack slot.
Register DayInstrInfo::isLoadFromStackSlot(const MachineInstr &MI,
                                             int &FrameIndex) const {
  return 0;
}

/// isStoreToStackSlot - If the specified machine instruction is a direct
/// store to a stack slot, return the virtual or physical register number of
/// the source reg along with the FrameIndex of the loaded stack slot.  If
/// not, return 0.  This predicate must return 0 if the instruction has
/// any side effects other than storing to the stack slot.
Register DayInstrInfo::isStoreToStackSlot(const MachineInstr &MI,
                                            int &FrameIndex) const {
  return 0;
}

// static SPCC::CondCodes GetOppositeBranchCondition(SPCC::CondCodes CC)
// {

// }

static bool isUncondBranchOpcode(int Opc) {

}

static bool isI32CondBranchOpcode(int Opc) {
}

static bool isI64CondBranchOpcode(int Opc) {
}

static bool isRegCondBranchOpcode(int Opc) {
}

static bool isFCondBranchOpcode(int Opc) {
}

static bool isCondBranchOpcode(int Opc) {
}

static bool isIndirectBranchOpcode(int Opc) {
}

static void parseCondBranch(MachineInstr *LastInst, MachineBasicBlock *&Target,
                            SmallVectorImpl<MachineOperand> &Cond) {
  
}

MachineBasicBlock *
DayInstrInfo::getBranchDestBlock(const MachineInstr &MI) const {
  
}

bool DayInstrInfo::analyzeBranch(MachineBasicBlock &MBB,
                                   MachineBasicBlock *&TBB,
                                   MachineBasicBlock *&FBB,
                                   SmallVectorImpl<MachineOperand> &Cond,
                                   bool AllowModify) const {
  MachineBasicBlock::iterator I = MBB.getLastNonDebugInstr();
  if (I == MBB.end())
    return false;

  if (!isUnpredicatedTerminator(*I))
    return false;

  // Get the last instruction in the block.
  MachineInstr *LastInst = &*I;
  unsigned LastOpc = LastInst->getOpcode();

  // If there is only one terminator instruction, process it.
  if (I == MBB.begin() || !isUnpredicatedTerminator(*--I)) {
    if (isUncondBranchOpcode(LastOpc)) {
      TBB = LastInst->getOperand(0).getMBB();
      return false;
    }
    if (isCondBranchOpcode(LastOpc)) {
      // Block ends with fall-through condbranch.
      parseCondBranch(LastInst, TBB, Cond);
      return false;
    }
    return true; // Can't handle indirect branch.
  }

  // Get the instruction before it if it is a terminator.
  MachineInstr *SecondLastInst = &*I;
  unsigned SecondLastOpc = SecondLastInst->getOpcode();

  // If AllowModify is true and the block ends with two or more unconditional
  // branches, delete all but the first unconditional branch.
  if (AllowModify && isUncondBranchOpcode(LastOpc)) {
    while (isUncondBranchOpcode(SecondLastOpc)) {
      LastInst->eraseFromParent();
      LastInst = SecondLastInst;
      LastOpc = LastInst->getOpcode();
      if (I == MBB.begin() || !isUnpredicatedTerminator(*--I)) {
        // Return now the only terminator is an unconditional branch.
        TBB = LastInst->getOperand(0).getMBB();
        return false;
      } else {
        SecondLastInst = &*I;
        SecondLastOpc = SecondLastInst->getOpcode();
      }
    }
  }

  // If there are three terminators, we don't know what sort of block this is.
  if (SecondLastInst && I != MBB.begin() && isUnpredicatedTerminator(*--I))
    return true;

  // If the block ends with a B and a Bcc, handle it.
  if (isCondBranchOpcode(SecondLastOpc) && isUncondBranchOpcode(LastOpc)) {
    parseCondBranch(SecondLastInst, TBB, Cond);
    FBB = LastInst->getOperand(0).getMBB();
    return false;
  }

  // If the block ends with two unconditional branches, handle it.  The second
  // one is not executed.
  if (isUncondBranchOpcode(SecondLastOpc) && isUncondBranchOpcode(LastOpc)) {
    TBB = SecondLastInst->getOperand(0).getMBB();
    return false;
  }

  // ...likewise if it ends with an indirect branch followed by an unconditional
  // branch.
  if (isIndirectBranchOpcode(SecondLastOpc) && isUncondBranchOpcode(LastOpc)) {
    I = LastInst;
    if (AllowModify)
      I->eraseFromParent();
    return true;
  }

  // Otherwise, can't handle this.
  return true;
}

unsigned DayInstrInfo::insertBranch(MachineBasicBlock &MBB,
                                      MachineBasicBlock *TBB,
                                      MachineBasicBlock *FBB,
                                      ArrayRef<MachineOperand> Cond,
                                      const DebugLoc &DL,
                                      int *BytesAdded) const {
  assert(TBB && "insertBranch must not be told to insert a fallthrough");
  assert((Cond.size() <= 3) &&
         "Day branch conditions should have at most three components!");

  if (Cond.empty()) {
    return 1;
  }

  // Conditional branch
  unsigned Opc = Cond[0].getImm();
  unsigned CC = Cond[1].getImm();
  if (isRegCondBranchOpcode(Opc)) {
    Register Reg = Cond[2].getReg();
    BuildMI(&MBB, DL, get(Opc)).addMBB(TBB).addImm(CC).addReg(Reg);
  } else {
    BuildMI(&MBB, DL, get(Opc)).addMBB(TBB).addImm(CC);
  }

  if (!FBB) {
    if (BytesAdded)
      *BytesAdded = 8;
    return 1;
  }


  return 2;
}

unsigned DayInstrInfo::removeBranch(MachineBasicBlock &MBB,
                                      int *BytesRemoved) const {
  MachineBasicBlock::iterator I = MBB.end();
  unsigned Count = 0;
  int Removed = 0;
  while (I != MBB.begin()) {
    --I;

    if (I->isDebugInstr())
      continue;

    if (!isCondBranchOpcode(I->getOpcode()) &&
        !isUncondBranchOpcode(I->getOpcode()))
      break; // Not a branch

    Removed += getInstSizeInBytes(*I);
    I->eraseFromParent();
    I = MBB.end();
    ++Count;
  }

  if (BytesRemoved)
    *BytesRemoved = Removed;
  return Count;
}

bool DayInstrInfo::reverseBranchCondition(
    SmallVectorImpl<MachineOperand> &Cond) const {

  return false;
}

bool DayInstrInfo::isBranchOffsetInRange(unsigned BranchOpc,
                                           int64_t Offset) const {


}

void DayInstrInfo::copyPhysReg(MachineBasicBlock &MBB,
                                 MachineBasicBlock::iterator I,
                                 const DebugLoc &DL, MCRegister DestReg,
                                 MCRegister SrcReg, bool KillSrc,
                                 bool RenamableDest, bool RenamableSrc) const {

}

void DayInstrInfo::storeRegToStackSlot(MachineBasicBlock &MBB,
                                         MachineBasicBlock::iterator I,
                                         Register SrcReg, bool isKill, int FI,
                                         const TargetRegisterClass *RC,
                                         const TargetRegisterInfo *TRI,
                                         Register VReg,
                                         MachineInstr::MIFlag Flags) const {

}

void DayInstrInfo::loadRegFromStackSlot(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator I, Register DestReg,
    int FI, const TargetRegisterClass *RC, const TargetRegisterInfo *TRI,
    Register VReg, MachineInstr::MIFlag Flags) const {

}

Register DayInstrInfo::getGlobalBaseReg(MachineFunction *MF) const {

}

unsigned DayInstrInfo::getInstSizeInBytes(const MachineInstr &MI) const {
 
}

bool DayInstrInfo::expandPostRAPseudo(MachineInstr &MI) const {
    
  return false;
}



