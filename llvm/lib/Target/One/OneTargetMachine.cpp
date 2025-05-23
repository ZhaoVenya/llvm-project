//
// Created by 蔡鹏 on 2024/11/26.
//

#include "OneTargetMachine.h"
#include "One.h"
#include "TargetInfo/OneTargetInfo.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeOneTarget() {
  RegisterTargetMachine<OneTargetMachine> X(getTheOneTarget());
  
  // 这一行代码的目的是获取LLVM全局唯一的Pass注册表实例的指针。
  // 指针 PR 将用于后续的Pass注册操作。
  auto *PR = PassRegistry::getPassRegistry();
  // 调用 initializeOneDAGToDAGISelLegacyPass 函数，并将之前获取到的全局唯一的Pass注册表PR作为参数传递给它。
  initializeOneDAGToDAGISelLegacyPass(*PR);
}


// 根据Triple的架构，返回对应的DataLayout
static StringRef computeDataLayout(const Triple &TT, const TargetOptions &options) {
    assert(TT.isArch32Bit() && "only 32 bit are currently supported");
    return "e-m:e-p:32:32-i64:64-n32-S128";
}

// LLVM中用于**确定最终的重定位模型（Relocation Model）**的辅助函数。
static Reloc::Model getEffectiveRelocModel(const Triple &TT, std::optional<Reloc::Model> RM) {
    return RM.value_or(Reloc::Static);
}

// 这里是OneTargetMachine类构造函数的实现部分。
OneTargetMachine::OneTargetMachine(const Target &T, const Triple &TT,
                                   StringRef CPU, StringRef FS,
                                   const TargetOptions &Options,
                                   std::optional<Reloc::Model> RM,
                                   std::optional<CodeModel::Model> CM,
                                   CodeGenOptLevel OL, bool JIT)
    : LLVMTargetMachine(T, 
          computeDataLayout(TT, Options), TT, CPU, FS, Options,
                        getEffectiveRelocModel(TT, RM),
                        getEffectiveCodeModel(CM, CodeModel::Small), OL),
      TLOF(std::make_unique<TargetLoweringObjectFileELF>()),
      Subtarget(TT, CPU, FS, *this){
  initAsmInfo();

}

namespace {
    // 这个地方定义了一个OnePassConfig的类
    class OnePassConfig : public TargetPassConfig {

      public:

        OnePassConfig(OneTargetMachine &TM, PassManagerBase &PM) : TargetPassConfig(TM, PM){

        }

        OneTargetMachine &getOneTargetMachine() const {
          return getTM<OneTargetMachine>();
        }

        const OneSubtarget &getOneSubtarget() const {
          return *getOneTargetMachine().getSubtargetImpl();
        }

        bool addInstSelector() override;
    };
} // namespace

// 创建PassConfig的实例
TargetPassConfig *OneTargetMachine::createPassConfig(PassManagerBase &PM) {
    return new OnePassConfig(*this, PM);
}

// 将指令选择器 Pass 添加到当前的 Pass 管理器中。
bool OnePassConfig::addInstSelector() {
    // Install an instruction selector.
    addPass(createOneISelDag(getOneTargetMachine()));
    return false;
}
