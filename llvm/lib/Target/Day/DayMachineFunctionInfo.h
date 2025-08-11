
#ifndef LLVM_LIB_TARGET_DAY_DAYMACHINEFUNCTIONINFO_H
#define LLVM_LIB_TARGET_DAY_DAYMACHINEFUNCTIONINFO_H

#include "DaySubtarget.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MIRYamlMapping.h"

namespace llvm {

class DayMachineFunctionInfo;

namespace yaml {
struct DayMachineFunctionInfo final : public yaml::MachineFunctionInfo {
  int VarArgsFrameIndex;
  int VarArgsSaveSize;

  DayMachineFunctionInfo() = default;
  DayMachineFunctionInfo(const llvm::DayMachineFunctionInfo &MFI);

  void mappingImpl(yaml::IO &YamlIO) override;
  ~DayMachineFunctionInfo() = default;
};

template <> struct MappingTraits<DayMachineFunctionInfo> {
  static void mapping(IO &YamlIO, DayMachineFunctionInfo &MFI) {
    YamlIO.mapOptional("varArgsFrameIndex", MFI.VarArgsFrameIndex);
    YamlIO.mapOptional("varArgsSaveSize", MFI.VarArgsSaveSize);
  }
};
} // end namespace yaml

/// DayMachineFunctionInfo - This class is derived from MachineFunctionInfo
/// and contains private Day-specific information for each MachineFunction.
class DayMachineFunctionInfo : public MachineFunctionInfo {
  // const DaySubtarget *Subtarget;
private:
  /// FrameIndex for start of varargs area
  int VarArgsFrameIndex = 0;
  /// Size of the save area used for varargs
  int VarArgsSaveSize = 0;
  /// FrameIndex used for transferring values between 64-bit FPRs and a pair
  /// of 32-bit GPRs via the stack.
  int MoveF64FrameIndex = -1;
  /// FrameIndex of the spill slot for the scratch register in BranchRelaxation.
  int BranchRelaxationScratchFrameIndex = -1;
  /// Size of any opaque stack adjustment due to save/restore libcalls.
  unsigned LibCallStackSize = 0;
  /// Size of stack frame to save callee saved registers
  unsigned CalleeSavedStackSize = 0;

  /// Registers that have been sign extended from i32.
  SmallVector<Register, 8> SExt32Registers;

  int64_t StackProbeSize = 0;

  /// Does it probe the stack for a dynamic allocation?
  bool HasDynamicAllocation = false;

public:
  DayMachineFunctionInfo(const Function &F, const DaySubtarget *STI);

  MachineFunctionInfo *
  clone(BumpPtrAllocator &Allocator, MachineFunction &DestMF,
        const DenseMap<MachineBasicBlock *, MachineBasicBlock *> &Src2DstMBB)
      const override;
  // 获取可变参数区域在栈帧中的索引。
  int getVarArgsFrameIndex() const { return VarArgsFrameIndex; }
  // 设置可变参数区域的栈帧索引。
  void setVarArgsFrameIndex(int Index) { VarArgsFrameIndex = Index; }

  // 获取用于保存可变参数的区域大小。
  unsigned getVarArgsSaveSize() const { return VarArgsSaveSize; }
  // 设置可变参数保存区域的大小。
  void setVarArgsSaveSize(int Size) { VarArgsSaveSize = Size; }

  // 获取用于在 64 位浮点寄存器和 32 位通用寄存器之间通过栈传递值的栈帧索引，并在需要时创建该栈对象。
  int getMoveF64FrameIndex(MachineFunction &MF) {
    if (MoveF64FrameIndex == -1)
      MoveF64FrameIndex =
          MF.getFrameInfo().CreateStackObject(8, Align(8), false);
    return MoveF64FrameIndex;
  }

  // 获取分支弛豫优化的 scratch 寄存器栈帧索引。
  int getBranchRelaxationScratchFrameIndex() const {
    return BranchRelaxationScratchFrameIndex;
  }
  // 设置分支弛豫优化的 scratch 寄存器栈帧索引。
  void setBranchRelaxationScratchFrameIndex(int Index) {
    BranchRelaxationScratchFrameIndex = Index;
  }

  // 返回所有预留的溢出区域（如库调用栈）的总大小。
  unsigned getReservedSpillsSize() const {
    return LibCallStackSize + 0;
  }

  // 获取因库调用导致的栈调整大小。
  unsigned getLibCallStackSize() const { return LibCallStackSize; }
  // 设置因库调用导致的栈调整大小。
  void setLibCallStackSize(unsigned Size) { LibCallStackSize = Size; }

  // 判断是否可以使用库调用来保存/恢复寄存器，这是基于多种条件来决定的。
  // bool useSaveRestoreLibCalls(const MachineFunction &MF) const {
  //   // We cannot use fixed locations for the callee saved spill slots if the
  //   // function uses a varargs save area, or is an interrupt handler.
  //   return !isPushable(MF) &&
  //          MF.getSubtarget<DaySubtarget>().enableSaveRestore() &&
  //          VarArgsSaveSize == 0 && !MF.getFrameInfo().hasTailCall() &&
  //          !MF.getFunction().hasFnAttribute("interrupt");
  // }

  // 获取用于保存被调用者保存寄存器的栈帧大小。
  unsigned getCalleeSavedStackSize() const { return CalleeSavedStackSize; }
  // 设置用于保存被调用者保存寄存器的栈帧大小。
  void setCalleeSavedStackSize(unsigned Size) { CalleeSavedStackSize = Size; }
  // 从 YAML 数据中初始化基本字段
  void initializeBaseYamlFields(const yaml::DayMachineFunctionInfo &YamlMFI);
  
  // 将一个从i32符号扩展到i64的寄存器添加到列表中。
  void addSExt32Register(Register Reg);
  
  // 检查给定的寄存器是否在符号扩展寄存器列表中。
  bool isSExt32Register(Register Reg) const;

  // 判断函数是否包含动态栈分配
  bool hasDynamicAllocation() const { return HasDynamicAllocation; }
  // 设置函数包含动态栈分配的标志。
  void setDynamicAllocation() { HasDynamicAllocation = true; }
};

} // end namespace llvm


#endif
