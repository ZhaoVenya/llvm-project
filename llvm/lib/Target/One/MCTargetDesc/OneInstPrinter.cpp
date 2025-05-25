//
// Created by 蔡鹏 on 2024/12/1.
//

#include "OneInstPrinter.h"
#include "OneInstrInfo.h"
#include "OneRegisterInfo.h"

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

#define PRINT_ALIAS_INSTR
#include "OneGenAsmWriter.inc"


// OneInstPrinter 负责显示 MCInst，将它们变成可读的字符串。
void OneInstPrinter::printRegName(raw_ostream &OS, MCRegister Reg) {
    OS << getRegisterName(Reg);
}

// 这是打印单个机器指令（MCInst）的入口点。
// 当 MCStreamer 需要将一个 MCInst 写入到汇编输出流时，它会调用这个函数。
void OneInstPrinter::printInst(const MCInst *MI, uint64_t Address,
                               StringRef Annot, const MCSubtargetInfo &STI,
                               raw_ostream &O) {
    if (!printAliasInstr(MI, Address, O))
        printInstruction(MI, Address, O);

    printAnnotation(O, Annot);
}

// 这是一个通用的操作数打印函数。它根据操作数的类型（寄存器、立即数、表达式）来决定如何将其格式化为文本。
void OneInstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                  raw_ostream &O) {
    const MCOperand &MO = MI->getOperand(OpNo);
    if (MO.isReg()) {
        printRegName(O, MO.getReg());
        return;
    }

    if (MO.isImm()) {
        printImmediate(MI, OpNo, O);
        return;
    }

    assert(MO.isExpr() && "Unknown operand kind in printOperand");
    MO.getExpr()->print(O, &MAI);
}

// printImmediate 确保立即数操作数能正确地以文本形式打印出来，无论是纯粹的数值还是一个表达式。
void OneInstPrinter::printImmediate(const MCInst *MI, unsigned opNum,
                                    raw_ostream &O) {
    const MCOperand &MO = MI->getOperand(opNum);
    if (MO.isImm())
        O << MO.getImm();
    else if (MO.isExpr()) {
        MO.getExpr()->print(O, &MAI);
    } else
        llvm_unreachable("Unknown immediate kind");
}

// 这个函数专门用于打印内存操作数。对于许多架构，
// 内存操作数通常由基址寄存器和一个偏移量组成，
// 其汇编语法可能是 offset(base_reg) 或 [base_reg + offset]。
void OneInstPrinter::printMemOperand(const MCInst *MI, unsigned OpNo, raw_ostream &O) {
    /// 先打印立即数 12(sp)
    printOperand(MI, OpNo + 1, O);
    O << "(";
    printOperand(MI, OpNo, O);
    O << ")";
}

// 这个函数用于打印指针操作数，它通常包含两个部分，可能是一个基址和另一个寄存器，或者一个基址和一个偏移。
void OneInstPrinter::printPtrOperand(const MCInst *MI, unsigned OpNo, raw_ostream &O) {
    printOperand(MI, OpNo, O);
    O << ",";
    printOperand(MI, OpNo+1, O);
}
