#include "llvm/MC/TargetRegistry.h"

#include "DayTargetMachine.h"
#include "DayMachineFunctionInfo.h"
#include "DayTargetObjectFile.h"

#include "TargetInfo/DayTargetInfo.h"
#define DEBUG_TYPE "day"

using namespace llvm;

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeDayTarget() {
    RegisterTargetMachine<Day32TargetMachine> X(getTheDayTarget());

    // PassRegistry &PR = *PassRegistry::getPassRegistry();
    // initializeDayDAGToDAGISelLegacyPass(PR);
    // initializeErrataWorkaroundPass(PR);

}


///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// 能用到的子函数
static std::string computeDataLayout(const Triple &T) {
  // Sparc is typically big endian, but some are little.
    std::string Ret;
    Ret = "e-m:e-p:32:32-i64:64-n32-S128";// 这个就是Day架构的Data Layout

    return Ret;
}

// 如果用户明确指定了一个重定位模型，就使用用户指定的模型；否则，默认使用 Reloc::Static 重定位模型。
// 静态赋值，也就是说所有地址在链接时都是固定的。一般来说，没什么共享库、动态链接库的话，那就是Static
static Reloc::Model getEffectiveRelocModel(std::optional<Reloc::Model> RM) {
  return RM.value_or(Reloc::Static);
}

// 代码模型，代码模型定义了生成的代码在内存中的大小和地址访问方式，
// 这直接影响到编译器如何生成访问全局数据和代码的指令。
// 根据用户指定的代码模型（如果有），结合有效的重定位模型、架构位数和是否 JIT 编译，
// 来确定最终使用的代码模型。
// 如果命令行选项：-code-model=large， 那么就返回CodeModel::large
// 如果不输入，就返回CodeModel::small
static CodeModel::Model getEffectiveDayCodeModel(
                    std::optional<CodeModel::Model> CM, 
                    Reloc::Model RM,
                    bool Is64Bit, 
                    bool JIT) {
  if (CM) {
    if (*CM == CodeModel::Tiny)
      report_fatal_error("Target does not support the tiny CodeModel", false);
    if (*CM == CodeModel::Kernel)
      report_fatal_error("Target does not support the kernel CodeModel", false);
    return *CM;
  }
  if (Is64Bit) {
    if (JIT)
      return CodeModel::Large;
    return RM == Reloc::PIC_ ? CodeModel::Small : CodeModel::Medium;
  }
  return CodeModel::Small;
}



///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// DayTargetMachine类相关的函数
///////// 构造函数
DayTargetMachine::DayTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                     StringRef FS, const TargetOptions &Options,
                     std::optional<Reloc::Model> RM,
                     std::optional<CodeModel::Model> CM, CodeGenOptLevel OL,
                     bool JIT, bool is64bit)
    : CodeGenTargetMachineImpl(
          T, computeDataLayout(TT), TT, CPU, FS, Options,
          getEffectiveRelocModel(RM),
          getEffectiveDayCodeModel(CM, getEffectiveRelocModel(RM), is64bit, JIT),
          OL), 
    TLOF(std::make_unique<TargetLoweringObjectFile>()), 
    is64Bit(is64bit)
{
    initAsmInfo();
}
///////// 析构函数
DayTargetMachine::~DayTargetMachine() = default;

///////// 
MachineFunctionInfo *DayTargetMachine::createMachineFunctionInfo(
    BumpPtrAllocator &Allocator, const Function &F,
    const TargetSubtargetInfo *STI) const {
  return DayMachineFunctionInfo::create<DayMachineFunctionInfo>(Allocator,
                                                                    F, STI);
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// Day32TargetMachine类相关的函数
void Day32TargetMachine::anchor(){

}

Day32TargetMachine::Day32TargetMachine(const Target &T, const Triple &TT,
                                           StringRef CPU, StringRef FS,
                                           const TargetOptions &Options,
                                           std::optional<Reloc::Model> RM,
                                           std::optional<CodeModel::Model> CM,
                                           CodeGenOptLevel OL, bool JIT)
    :DayTargetMachine(T, TT, CPU, FS, Options, RM, CM, OL, JIT, false)
{

}

