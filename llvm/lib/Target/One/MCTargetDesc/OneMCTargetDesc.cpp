//
// Created by 蔡鹏 on 2024/11/26.
//

#include "OneMCTargetDesc.h"
#include "OneInstPrinter.h"
#include "OneInstrInfo.h"
#include "OneMCAsmInfo.h"
#include "OneRegisterInfo.h"
#include "OneSubtarget.h"
#include "TargetInfo/OneTargetInfo.h"
#include "llvm/MC/TargetRegistry.h"

#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#include "OneGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "OneGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "OneGenRegisterInfo.inc"
/*
*  using MCInstrInfoCtorFnTy = MCInstrInfo *(*)();
  using MCInstrAnalysisCtorFnTy = MCInstrAnalysis *(*)(const MCInstrInfo *Info);
  using MCRegInfoCtorFnTy = MCRegisterInfo *(*)(const Triple &TT);
  using MCSubtargetInfoCtorFnTy = MCSubtargetInfo *(*)(const Triple &TT,
                                                       StringRef CPU,
                                                       StringRef Features);
 */

// 这个函数是一个工厂，用于创建并返回一个 MCAsmInfo 对象（或其派生类 OneMCAsmInfo 的实例）。
// MCAsmInfo 的职责： 提供关于目标架构的基本汇编语法信息，例如：
//  注释字符（例如 # 或 //）
//  标签的后缀（例如 :）
//  段（section）的命名约定
//  指令前缀、操作数顺序等。
MCAsmInfo *createOneMCAsmInfo(const MCRegisterInfo &MRI, const Triple &TT,
                              const MCTargetOptions &Options) {
    return new OneMCAsmInfo(TT);
}


// 创建并返回一个 MCRegisterInfo 对象。
// MCRegisterInfo 的职责： 提供关于目标架构寄存器的详细信息，例如：
// 所有物理寄存器的列表及其名称。
// 寄存器类（Register Classes），例如通用寄存器、浮点寄存器。
// 寄存器的别名（例如，a0 可能是 x10 的别名）。
// 调用约定中哪些寄存器是调用者保存的，哪些是被调用者保存的。
// 寄存器的子寄存器和父寄存器关系。
MCRegisterInfo *createOneMCRegisterInfo(const Triple &TT) {
    MCRegisterInfo *X = new MCRegisterInfo();
    InitOneMCRegisterInfo(X, One::RA);
    return X;
}


// 创建并返回一个 MCInstrInfo 对象。
// MCInstrInfo 的职责： 提供关于目标架构指令集的详细信息，例如：
// 所有指令的列表及其操作码。
// 每条指令的操作数类型、数量和语义。
// 指令的隐式定义和使用（例如，ADD 指令会隐式地设置条件码寄存器）。
// 指令的格式、延迟槽等。
MCInstrInfo *createOneMCInstrInfo() {
    MCInstrInfo *X = new MCInstrInfo();
    InitOneMCInstrInfo(X);
    return X;
}

// MCInstPrinter 的职责： 
// 将 MCInst（机器指令的抽象表示）转换为人类可读的文本汇编指令字符串。
// 这是 llc -S 和 llvm-objdump 用来生成汇编代码的关键组件。
MCInstPrinter *createOneMCInstPrinter(const Triple &T, unsigned SyntaxVariant,
                                      const MCAsmInfo &MAI,
                                      const MCInstrInfo &MII,
                                      const MCRegisterInfo &MRI) {
    return new OneInstPrinter(MAI, MII, MRI);
}

// 创建并返回一个 MCSubtargetInfo 对象。
// MCSubtargetInfo 的职责： 提供关于目标架构子目标（subtarget）特性的信息。
// 一个架构可能有多个 CPU 型号，每个型号支持不同的特性集（例如，是否支持浮点运算、特定的指令扩展等）。
MCSubtargetInfo *createOneMCSubtargetInfo(const Triple &TT, StringRef CPU,
                                          StringRef FS) {
    if (CPU.empty()) {
        CPU = "one";
    }
    return createOneMCSubtargetInfoImpl(TT, CPU, CPU, FS);
}


// 这是 LLVM 用于动态加载和注册你的 One 目标架构MC 层组件的入口函数。
// 当 LLVM 加载你的 One 后端插件时，它会查找并调用这个函数。
extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeOneTargetMC() {
    TargetRegistry::RegisterMCAsmInfo(getTheOneTarget(), createOneMCAsmInfo);
    TargetRegistry::RegisterMCRegInfo(getTheOneTarget(), createOneMCRegisterInfo);
    TargetRegistry::RegisterMCInstrInfo(getTheOneTarget(), createOneMCInstrInfo);
    TargetRegistry::RegisterMCSubtargetInfo(getTheOneTarget(),
                                            createOneMCSubtargetInfo);
    TargetRegistry::RegisterMCInstPrinter(getTheOneTarget(),
                                          createOneMCInstPrinter);
}

