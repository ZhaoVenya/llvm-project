#include "ABIInfoImpl.h"
#include "TargetInfo.h"

using namespace clang;
using namespace clang::CodeGen;

//===----------------------------------------------------------------------===//
// RISC-V ABI Implementation
//===----------------------------------------------------------------------===//

namespace {
class DayABIInfo : public DefaultABIInfo {

public:
  DayABIInfo(CodeGenTypes &CGT) : DefaultABIInfo(CGT) {}

  // DefaultABIInfo's classifyReturnType and classifyArgumentType are
  // non-virtual, but computeInfo is virtual, so we overload it.
  void computeInfo(CGFunctionInfo &FI) const override;

  ABIArgInfo classifyReturnType(QualType Ty) const;
};
} // end anonymous namespace

void DayABIInfo::computeInfo(CGFunctionInfo &FI) const {
  FI.getReturnInfo() = classifyReturnType(FI.getReturnType());
  for (auto &Arg : FI.arguments())
    Arg.info = classifyArgumentType(Arg.type);
}

ABIArgInfo DayABIInfo::classifyReturnType(QualType Ty) const {
  if (Ty->isAnyComplexType()) {
    return ABIArgInfo::getDirect();
  }
  else {
    return DefaultABIInfo::classifyReturnType(Ty);
  }
}

namespace {
class DayTargetCodeGenInfo : public TargetCodeGenInfo {
public:
  DayTargetCodeGenInfo(CodeGen::CodeGenTypes &CGT)
      : TargetCodeGenInfo(std::make_unique<DayABIInfo>(CGT)) {}

  void setTargetAttributes(const Decl *D, llvm::GlobalValue *GV,
                           CodeGen::CodeGenModule &CGM) const override {}
};
} // namespace

std::unique_ptr<TargetCodeGenInfo>
CodeGen::createDayTargetCodeGenInfo(CodeGenModule &CGM) {
  return std::make_unique<DayTargetCodeGenInfo>(CGM.getTypes());
}
