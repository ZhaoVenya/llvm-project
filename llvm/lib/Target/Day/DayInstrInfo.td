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
> : DayInstR<opcode, Func4.func4_int, (mnemonic # " $rd, $rs1, $rs2")> {
    // 自动生成模式
    let Pattern = [(set GPR:$rd, (OpNode GPR:$rs1, GPR:$rs2))];
}


def ADD : DayR_I_2_Op<0b0000, "add", add>;
def SUB : DayR_I_2_Op<0b0001, "sub", sub>;
def MUL : DayR_I_2_Op<0b0010, "mul", mul>;
def DIV : DayR_I_2_Op<0b0011, "div", sdiv>;
def SLT : DayR_I_2_Op<0b0100, "slt", setlt>;
def SLTU: DayR_I_2_Op<0b0000, "sltu", setult>;

def AND : DayR_I_2_Op<0b0000, "and", and>;
def OR  : DayR_I_2_Op<0b0000, "or", or>;
def XOR : DayR_I_2_Op<0b0000, "xor", xor>;
// def NOT : DayR_I_2_Op<0b0000, "sltu", xori>; 这个应该是伪指令，对应的markdown文件还需要修改
def SLL : DayR_I_2_Op<0b0000, "sll", shl>;
def SRL : DayR_I_2_Op<0b0000, "srl", srl>;
def SRA : DayR_I_2_Op<0b0000, "sra", sra>;



@TODO
/////////////////////



def ADD : DayInstR<0b0000, Func4.func4_int, "add $rd, $rs1, $rs2", [(set GPR:$rd, (add GPR:$rs1, GPR:$rs2))]>;
def SUB : DayInstR<0b0001, Func4.func4_int, "sub $rd, $rs1, $rs2", [(set GPR:$rd, (add GPR:$rs1, GPR:$rs2))]>;
def MUL : DayInstR<0b0010, Func4.func4_int, "mul $rd, $rs1, $rs2", [(set GPR:$rd, (add GPR:$rs1, GPR:$rs2))]>;
def DIV : DayInstR<0b0011, Func4.func4_int, "div $rd, $rs1, $rs2", [(set GPR:$rd, (add GPR:$rs1, GPR:$rs2))]>;
def SLT : DayInstR<0b0100, Func4.func4_int, "slt $rd, $rs1, $rs2", [(set GPR:$rd, (add GPR:$rs1, GPR:$rs2))]>;
def SLTU: DayInstR<0b0101, Func4.func4_int, "sltu $rd, $rs1, $rs2", [(set GPR:$rd, (add GPR:$rs1, GPR:$rs2))]>;
def AND : DayInstR<0b0110, Func4.func4_int, "and $rd, $rs1, $rs2", [(set GPR:$rd, (add GPR:$rs1, GPR:$rs2))]>;
def OR  : DayInstR<0b0111, Func4.func4_int, "or $rd, $rs1, $rs2", [(set GPR:$rd, (add GPR:$rs1, GPR:$rs2))]>;
def XOR : DayInstR<0b1000, Func4.func4_int, "xor $rd, $rs1, $rs2", [(set GPR:$rd, (add GPR:$rs1, GPR:$rs2))]>;
def NOT : DayInstR<0b1001, Func4.func4_int, "not $rd, $rs1, $rs2", [(set GPR:$rd, (add GPR:$rs1, GPR:$rs2))]>;
def SLL : DayInstR<0b1010, Func4.func4_int, "sll $rd, $rs1, $rs2", [(set GPR:$rd, (add GPR:$rs1, GPR:$rs2))]>;
def SRL : DayInstR<0b1011, Func4.func4_int, "srl $rd, $rs1, $rs2", [(set GPR:$rd, (add GPR:$rs1, GPR:$rs2))]>;
def SRA : DayInstR<0b1100, Func4.func4_int, "sra $rd, $rs1, $rs2", [(set GPR:$rd, (add GPR:$rs1, GPR:$rs2))]>;

def ADDF : DayInstR<0b0000, Func4.func4_float, "addf $rd, $rs1, $rs2", [(set GPR:$rd, (add GPR:$rs1, GPR:$rs2))]>;
def SUBF : DayInstR<0b0001, Func4.func4_float, "subf $rd, $rs1, $rs2", [(set GPR:$rd, (add GPR:$rs1, GPR:$rs2))]>;
def MULF : DayInstR<0b0010, Func4.func4_float, "mulf $rd, $rs1, $rs2", [(set GPR:$rd, (add GPR:$rs1, GPR:$rs2))]>;
def DIVF : DayInstR<0b0011, Func4.func4_float, "divf $rd, $rs1, $rs2", [(set GPR:$rd, (add GPR:$rs1, GPR:$rs2))]>;
def SLTF : DayInstR<0b0100, Func4.func4_float, "sltf $rd, $rs1, $rs2", [(set GPR:$rd, (add GPR:$rs1, GPR:$rs2))]>;


def MPAP : DayInstR4<0b0011, Func4.func4_int, "mpap $rd, $rs1, $rs2, $rs3", [GPR, GPR, GPR, GPR]>;
def MPAN : DayInstR4<0b0100, Func4.func4_int, "mpan $rd, $rs1, $rs2, $rs3", [GPR, GPR, GPR, GPR]>;
def MNAP : DayInstR4<0b0101, Func4.func4_int, "mnap $rd, $rs1, $rs2, $rs3", [GPR, GPR, GPR, GPR]>;
def MNAN : DayInstR4<0b0110, Func4.func4_int, "mnan $rd, $rs1, $rs2, $rs3", [GPR, GPR, GPR, GPR]>;
def BITSW: DayInstR4<0b0111, Func4.func4_int, "bitsw $rd, $rs1, $rs2, $rs3", [GPR, GPR, GPR, GPR]>;
def BYTESW: DayInstR4<0b1000, Func4.func4_int, "bytesw $rd, $rs1, $rs2, $rs3", [GPR, GPR, GPR, GPR]>;

def MPAPF : DayInstR4<0b0011, Func4.func4_float, "mpapf $rd, $rs1, $rs2, $rs3", [GPR, GPR, GPR, GPR]>;
def MPANF : DayInstR4<0b0100, Func4.func4_float, "mpanf $rd, $rs1, $rs2, $rs3", [GPR, GPR, GPR, GPR]>;
def MNAPF : DayInstR4<0b0101, Func4.func4_float, "mnapf $rd, $rs1, $rs2, $rs3", [GPR, GPR, GPR, GPR]>;
def MNANF : DayInstR4<0b0110, Func4.func4_float, "mnanf $rd, $rs1, $rs2, $rs3", [GPR, GPR, GPR, GPR]>;


def ADDI : DayInstI<0b0000, imm14_val, "addi $rd, $rs1, $imm",  [GPR, GPR, i32]>;
def SUBI : DayInstI<0b0001, imm14_val, "subi $rd, $rs1, $imm",  [GPR, GPR, i32]>;
def MULI : DayInstI<0b0010, imm14_val, "muli  $rd, $rs1, $imm",  [GPR, GPR, i32]>;
def DIVI : DayInstI<0b0011, imm14_val, "divi  $rd, $rs1, $imm",  [GPR, GPR, i32]>;
def SLTI : DayInstI<0b0100, imm14_val, "slti  $rd, $rs1, $imm",  [GPR, GPR, i32]>;
def SLTIU: DayInstI<0b0101, imm14_val, "sltiu  $rd, $rs1, $imm", [GPR, GPR, i32]>;
def ANDI : DayInstI<0b0110, imm14_val, "andi  $rd, $rs1, $imm",  [GPR, GPR, i32]>;
def ORI  : DayInstI<0b0111, imm14_val, "ori  $rd, $rs1, $imm",   [GPR, GPR, i32]>;
def XORI : DayInstI<0b1000, imm14_val, "xori  $rd, $rs1, $imm",  [GPR, GPR, i32]>;
def NOTI : DayInstI<0b1001, imm14_val, "noti  $rd, $rs1, $imm",  [GPR, GPR, i32]>;
def JALR : DayInstI<0b1010, imm14_val, "jalr  $rd, $rs1, $imm",  [GPR, GPR, i32]>;

def LB1 : DayInstL<0b0000, imm16_val, "lb1", [GPR, GPR, i32]>;
def LB2 : DayInstL<0b0001, imm16_val, "lb2", [GPR, GPR, i32]>;
def LB3 : DayInstL<0b0010, imm16_val, "lb3", [GPR, GPR, i32]>;
def LB4 : DayInstL<0b0011, imm16_val, "lb4", [GPR, GPR, i32]>;
def LH1 : DayInstL<0b0100, imm16_val, "lh1", [GPR, GPR, i32]>;
def LH2 : DayInstL<0b0101, imm16_val, "lh2", [GPR, GPR, i32]>;
def LW  : DayInstL<0b0110, imm16_val, "lw" , [GPR, GPR, i32]>;


def SB1 : DayInstS<0b0000, imm16_val, "sb1", [GPR, GPR, i32]>;
def SB2 : DayInstS<0b0001, imm16_val, "sb2", [GPR, GPR, i32]>;
def SB3 : DayInstS<0b0010, imm16_val, "sb3", [GPR, GPR, i32]>;
def SB4 : DayInstS<0b0011, imm16_val, "sb4", [GPR, GPR, i32]>;
def SH1 : DayInstS<0b0100, imm16_val, "sh1", [GPR, GPR, i32]>;
def SH2 : DayInstS<0b0101, imm16_val, "sh2", [GPR, GPR, i32]>;
def SW  : DayInstS<0b0110, imm16_val, "sw" , [GPR, GPR, i32]>;


def BEQ : DayInstB<0b0000, imm16_val, "beq", [GPR, GPR, i32]>;
def BNE : DayInstB<0b0001, imm16_val, "bne", [GPR, GPR, i32]>;
def BLT : DayInstB<0b0010, imm16_val, "blt", [GPR, GPR, i32]>;
def BGE : DayInstB<0b0011, imm16_val, "bge", [GPR, GPR, i32]>;
def BLTU: DayInstB<0b0100, imm16_val, "bltu",[GPR, GPR, i32]>;
def BGEU: DayInstB<0b0101, imm16_val, "bgeu",[GPR, GPR, i32]>;

def LUI : DayInstU<0b0000, imm32_val, "lui", [GPR, GPR, i32]>;
def AUIPC : DayInstU<0b0001, imm32_val, "auipc", [GPR, GPR, i32]>;

def JAL : DayInstJ<0b0000, imm32_val, "jal", [GPR, GPR, i32]>;




