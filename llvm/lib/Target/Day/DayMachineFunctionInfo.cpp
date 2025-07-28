#include "DayMachineFunctionInfo.h"

using namespace llvm;

void DayMachineFunctionInfo::anchor() { }

MachineFunctionInfo *DayMachineFunctionInfo::clone(
    BumpPtrAllocator &Allocator, MachineFunction &DestMF,
    const DenseMap<MachineBasicBlock *, MachineBasicBlock *> &Src2DstMBB)
    const {
  return DestMF.cloneInfo<DayMachineFunctionInfo>(*this);
}