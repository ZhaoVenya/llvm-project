//
// Created by 蔡鹏 on 2024/12/1.
//

#include "OneAsmPrinter.h"

#include "MCTargetDesc/OneMCExpr.h"
#include "MCTargetDesc/OneMCTargetDesc.h"
#include "TargetInfo/OneTargetInfo.h"

#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

#define PRINT_ALIAS_INSTR
#include "OneGenMCPseudoLowering.inc"


// OneAsmPrinter 负责生成 MCInst，并决定它们应该如何组合成一个完整的汇编文件

// 这是 OneAsmPrinter 这个 Pass 的入口点。
// 当 LLVM 的 Pass Manager 运行 AsmPrinter Pass 时，
// 它会调用这个函数来处理当前的 MachineFunction
bool OneAsmPrinter::runOnMachineFunction(MachineFunction &MF) {
    AsmPrinter::runOnMachineFunction(MF);
    return true;
}


// 这个函数是 AsmPrinter 的核心，负责将**单个 MachineInstr 对象翻译并发射（emit）**到汇编输出流中。
void OneAsmPrinter::emitInstruction(const MachineInstr *MI) {
    // Do any auto-generated pseudo lowerings.
    if (MCInst OutInst; lowerPseudoInstExpansion(MI, OutInst)) {
        EmitToStreamer(*OutStreamer, OutInst);
        return;
    }

    MCInst TmpInst;
    lowerToMCInst(MI, TmpInst);
    EmitToStreamer(*OutStreamer, TmpInst);
}


// 这个函数负责将 LLVM 内部的MachineInstr 转换为 MC 层（Machine Code Layer）使用的 MCInst。
// MCInst 是一个更低级别、更通用的指令表示，
// 不包含 LLVM 特定的一些高级信息（如虚拟寄存器、调试位置等），它更接近于最终的机器码或汇编代码。
void OneAsmPrinter::lowerToMCInst(const MachineInstr *MI, MCInst &Out) {
    Out.setOpcode(MI->getOpcode());

    for (const MachineOperand &MO : MI->operands()) {
        MCOperand MCOp;
        lowerOperand(MO, MCOp);
        Out.addOperand(MCOp);
    }
}


// 这个函数专门负责将表示符号（Symbol）的 MachineOperand
// （例如全局变量的地址、外部函数的地址、基本块的地址）转换为 MCOperand，
// 同时处理目标特有的重定位类型（Relocation Type）。
MCOperand OneAsmPrinter::lowerSymbolOperand(const MachineOperand &MO) const {
    // auto *symbol = getSymbol(MO.getGlobal());
    // const auto &expr = MCSymbolRefExpr::create(symbol,
    // MCSymbolRefExpr::VK_None, OutContext); MCOp = MCOperand::createExpr(expr);
    OneMCExpr::Kind kind = OneMCExpr::NONE;
    const MCSymbol *symbol = nullptr;

    switch (MO.getTargetFlags()) {
        case OneMCExpr::HI: kind = OneMCExpr::HI; break;

        case OneMCExpr::LO: kind = OneMCExpr::LO; break;

        default: break;
    }

    if (MO.getType() == MachineOperand::MO_MachineBasicBlock) {
        symbol = MO.getMBB()->getSymbol();
    } else if (MO.getType() == MachineOperand::MO_ExternalSymbol) {
        symbol = GetExternalSymbolSymbol(MO.getSymbolName());
    }
    else {
        symbol = getSymbol(MO.getGlobal());
    }

    const MCExpr *Expr = MCSymbolRefExpr::create(symbol, OutContext);
    Expr = new OneMCExpr(kind, Expr);

    return MCOperand::createExpr(Expr);
}


// 这个函数是 lowerToMCInst 调用的辅助函数，
// 负责将单个 MachineOperand（机器操作数）转换为MCOperand（MC 操作数）。
bool OneAsmPrinter::lowerOperand(const MachineOperand &MO,
                                 MCOperand &MCOp) const {

    switch (MO.getType()) {

        case MachineOperand::MO_Register: {
            MCOp = MCOperand::createReg(MO.getReg());
            return true;
        }

        case MachineOperand::MO_Immediate: {
            MCOp = MCOperand::createImm(MO.getImm());
            return true;
        }

        case MachineOperand::MO_GlobalAddress:
        case MachineOperand::MO_ExternalSymbol:
        case MachineOperand::MO_MachineBasicBlock: {
            MCOp = lowerSymbolOperand(MO);
            return true;
        }

        case MachineOperand::MO_RegisterMask: {
          /// Ignore
          break;
        }

        default:
          break;
    }
  MCOp = MCOperand();
  return true;
}


// 这是一个全局的初始化函数，它是 LLVM 用来动态加载和注册你的 OneAsmPrinter Pass 的入口点。
extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeOneAsmPrinter() {
    RegisterAsmPrinter<OneAsmPrinter> X(getTheOneTarget());
}
