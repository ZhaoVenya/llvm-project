/************************************************************\
    * File: llvm/lib/Target/Day/DayInstrInfo.td
    * Description: Instruction information for the Day architecture.
\************************************************************/

include "llvm/Target/Day/DayInstrFormats.td"
include "llvm/Target/Day/DayRegisterInfo.td"


class DayR_I_2_Op<
    bits<4> opcode,
    string mnemonic,
    SDNode OpNode
> : DayInstR<opcode, Func4.func4_int, (mnemonic # " $rd, $rs1, $rs2")> {
    // 自动生成模式
    let Pattern = [(set GPR:$rd, (OpNode GPR:$rs1, GPR:$rs2))];
}

class DayR_F_2_Op<
    bits<4> opcode,
    string mnemonic,
    SDNode OpNode
> : DayInstR<opcode, Func4.func4_float, (mnemonic # " $rd, $rs1, $rs2")> {
    // 自动生成模式
    let Pattern = [(set GPR:$rd, (OpNode GPR:$rs1, GPR:$rs2))];
}

class DayR_I_NOT<
    bits<4> opcode,
    string mnemonic,
    SDNode OpNode
> : DayInstR<opcode, Func4.func4_int, (mnemonic # " $rd, $rs1, ZERO")> {
    let Pattern = [(set GPR:$rd, (OpNode GPR:$rs1))];// 这个Pat是DAG中的，不是平台汇编的Pat
}


//////// 整数运算，逻辑运算等
def ADD : DayR_I_2_Op<0b0000, "add", add>;
def SUB : DayR_I_2_Op<0b0001, "sub", sub>;
def MUL : DayR_I_2_Op<0b0010, "mul", mul>;
def DIV : DayR_I_2_Op<0b0011, "div", sdiv>;
def SLT : DayR_I_2_Op<0b0100, "slt", setlt>;
def SLTU: DayR_I_2_Op<0b0101, "sltu", setult>;

def AND : DayR_I_2_Op<0b0110, "and", and>;
def OR  : DayR_I_2_Op<0b0111, "or", or>;
def XOR : DayR_I_2_Op<0b1000, "xor", xor>;
def NOT : DayR_I_NOT <0b1001, "not", ISD.NOT>; //// 
def SLL : DayR_I_2_Op<0b1010, "sll", shl>;
def SRL : DayR_I_2_Op<0b1011, "srl", srl>;
def SRA : DayR_I_2_Op<0b1100, "sra", sra>;

//////// 浮点数运算
def ADDF : DayR_F_2_Op<0b0000, "addf", fadd>;
def SUBF : DayR_F_2_Op<0b0001, "subf", fsub>;
def MULF : DayR_F_2_Op<0b0010, "mulf", fmul>;
def DIVF : DayR_F_2_Op<0b0011, "divf", fdiv>;
def SLTF : DayR_F_2_Op<0b0100, "sltf", setolt>;


//////// 这个地方需要定义R4类型，包括整数操作和浮点数操作
def MPAP : DayInstR4<0b0011, Func4.func4_int, "mpap $rd, $rs1, $rs2, $rs3">; // 内联汇编用到的指令，所以selectiondag中没有对应的SDNODE
def MPAN : DayInstR4<0b0100, Func4.func4_int, "mpan $rd, $rs1, $rs2, $rs3">; // 内联汇编用到的指令，所以selectiondag中没有对应的SDNODE
def MNAP : DayInstR4<0b0101, Func4.func4_int, "mnap $rd, $rs1, $rs2, $rs3">; // 内联汇编用到的指令，所以selectiondag中没有对应的SDNODE
def MNAN : DayInstR4<0b0110, Func4.func4_int, "mnan $rd, $rs1, $rs2, $rs3">; // 内联汇编用到的指令，所以selectiondag中没有对应的SDNODE
def BITSW: DayInstR4<0b0111, Func4.func4_int, "bitsw $rd, $rs1, $rs2, $rs3">; // 内联汇编用到的指令，所以selectiondag中没有对应的SDNODE
def BYTESW: DayInstR4<0b1000, Func4.func4_int, "bytesw $rd, $rs1, $rs2, $rs3">; // 内联汇编用到的指令，所以selectiondag中没有对应的SDNODE

def MPAPF : DayInstR4<0b0011, Func4.func4_float, "mpapf $rd, $rs1, $rs2, $rs3">; // 内联汇编用到的指令，所以selectiondag中没有对应的SDNODE
def MPANF : DayInstR4<0b0100, Func4.func4_float, "mpanf $rd, $rs1, $rs2, $rs3">; // 内联汇编用到的指令，所以selectiondag中没有对应的SDNODE
def MNAPF : DayInstR4<0b0101, Func4.func4_float, "mnapf $rd, $rs1, $rs2, $rs3">; // 内联汇编用到的指令，所以selectiondag中没有对应的SDNODE
def MNANF : DayInstR4<0b0110, Func4.func4_float, "mnanf $rd, $rs1, $rs2, $rs3">; // 内联汇编用到的指令，所以selectiondag中没有对应的SDNODE

//////// 这个地方需要定义I类型，包括整数操作和浮点数操作
class DayI_I_2_Op<
    bits<4> opcode,
    string mnemonic,
    SDNode OpNode
> : DayInstI<opcode, Func4.func4_int, (mnemonic # " $rd, $rs1, $imm14")>
{
    let Pattern = [(set GPR:$rd, (OpNode GPR:$rs1, simm14:$imm14))];
};

def ADDI : DayI_I_2_Op<0b0000, "addi",  add>;
def SUBI : DayI_I_2_Op<0b0001, "subi",  sub>;
def MULI : DayI_I_2_Op<0b0010, "muli",  mul>;
def DIVI : DayI_I_2_Op<0b0011, "divi",  sdiv>;
def SLTI : DayI_I_2_Op<0b0100, "slti",  setlt>;
def SLTIU: DayI_I_2_Op<0b0101, "sltiu", setult>;
def ANDI : DayI_I_2_Op<0b0110, "andi",  and>;
def ORI  : DayI_I_2_Op<0b0111, "ori",   or>;
def XORI : DayI_I_2_Op<0b1000, "xori",  xor>;


// def NOTI : DayI_I_2_Op<0b0000, "noti", add>; //// 立即数取反不要了，因为可以被生成一个常数
def JALR : DayInstI<0b1010, Func4.func4_int, "jalr $rd, $rs1, $imm14"> {
    let Pattern = [(set GPR:$rd, (add (get_pc), 4)), // 将 PC+4 写入 rd
                     (brind (add GPR:$rs1, simm14:$imm14)) // 无条件间接跳转
                    ];
    let isBranch = 1;
    let isTerminator = 1;
    let isIndirectBranch = 1;
}

// L类型指令
class DayL_I_2_Op<
    bits<4> opcode,
    string mnemonic,
    ValueType VT,
    SDNode  OpNode
> : DayInstL<opcode, Func4.func4_int, (mnemonic # " $rd, $rs1($imm14)")>
{
    let Pattern = [(set GPR:$rd, (OpNode VT, (add GPR:$rs1, simm14:$imm14)))];
};

def LB : DayL_I_2_Op<0b0000, "lb", i8,    ISD.LOAD>;
def LH : DayL_I_2_Op<0b0001, "lh", i16,   ISD.LOAD>;
def LW : DayL_I_2_Op<0b0010, "lw", i32,   ISD.LOAD>;
def LBU : DayL_I_2_Op<0b0011, "lbu", i8,  ISD.ZEXTLOAD>;
def LHU : DayL_I_2_Op<0b0100, "lhu", i16, ISD.ZEXTLOAD>;
def LWU : DayL_I_2_Op<0b0101, "lwu", i32, ISD.LOAD>;


// S类型指令
class DayS_I_2_Op<
    bits<4> opcode,
    string mnemonic,
    ValueType VT
>:DayInstS<opcode, Func4.func4_int, (mnemonic # " $rs1($imm14), $rs2")>
{
    let Pattern = [(ISD.STORE VT, GPR:$rs2, (add GPR:$rs1, simm14:$imm14))];
};

def SB : DayS_I_2_Op<0b0000, "sb", i8>;
def SH : DayS_I_2_Op<0b0001, "sh", i16>;
def SW : DayS_I_2_Op<0b0010, "sw", i32>;



// B类型指令
class DayB_I_2_Op<
    bits<4> opcode,
    string mnemonic,
    PatFrag CompareOp
>:DayInstB<opcode, Func4.func4_int, (mnemonic # " $rs1, $rs2, $target")>{
    let Pattern = [(brcond (CompareOp GPR:$rs1, GPR:$rs2), bb_operand:$target)];
};

def BEQ : DayB_I_2_Op<0b0000, "beq",  seteq>;
def BNE : DayB_I_2_Op<0b0001, "bne",  setne>;
def BLT : DayB_I_2_Op<0b0010, "blt",  setlt>;
def BGE : DayB_I_2_Op<0b0011, "bge",  setge>;
def BLTU: DayB_I_2_Op<0b0100, "bltu", setult>;
def BGEU: DayB_I_2_Op<0b0101, "bgeu", setuge>;


class DayU_I_2_Op<
    bits<4> opcode,
    string mnemonic,
    list<dag> patterns
>:DayInstU<opcode, Func4.func4_int, (mnemonic # " $rd, $imm32")>{
    let Pattern = patterns;
};

def LUI : DayU_I_2_Op<0b0000, "lui", [(set GPR:$rd, (shl (i32 simm32:$imm32), 13))]>;
def AUIPC : DayU_I_2_Op<0b0001, "auipc", [(set GPR:$rd, (add (get_pc), (shl (i32 simm32:$imm32), 13)))]>;



class DayJ_I_2_Op<
    bits<4> opcode,
    string mnemonic,
    list<dag> patterns
>:DayInstJ<opcode, Func4.func4_int, (mnemonic # " $rd, $imm32")>{
    let Pattern = patterns;
};

def JAL : DayJ_I_2_Op<0b0000, "jal", [(set GPR:$rd, (add (get_pc), 4)), (br bb_operand:$imm32)] >;// 不会写了




