// /************************************************************\
//     * File: llvm/lib/Target/Day/DayInstrInfo.td
//     * Description: Instruction information for the Day architecture.
// \************************************************************/

include "DayInstrFormats.td"

def Day_PC : SDNode<"Day_PC", SDTNone, [SDNPOutGlue, SDNPInGlue, SDNPHasChain]>;

/////////////////////////////////////////////////////////////////////////////////////
//// R指令对应的Pattern
/////////////////////////////////////////////////////////////////////////////////////
class ALU_RR_I<string asmstr, bits<4> opcode, SDNode OpNode> : 
        DayInstR_I<(outs GPR:$rd), (ins GPR:$rs1, GPR:$rs2), 
        asmstr, "$rd, $rs1, $rs2", 
        opcode>{
    let Pattern = [(set GPR:$rd, (OpNode GPR:$rs1, GPR:$rs2))];
}

class ALU_RR_I_PatFrag<string asmstr, bits<4> opcode, PatFrag OpNode> : 
        DayInstR_I<(outs GPR:$rd), (ins GPR:$rs1, GPR:$rs2), 
        asmstr, "$rd, $rs1, $rs2", 
        opcode>{
    let Pattern = [(set GPR:$rd, (OpNode GPR:$rs1, GPR:$rs2))];
}

class ALU_RR_F<string asmstr, bits<4> opcode, SDNode OpNode> : 
        DayInstR_F<(outs FPR:$rd), (ins FPR:$rs1, FPR:$rs2), 
        asmstr, "$rd, $rs1, $rs2", 
        opcode>{
    let Pattern = [(set FPR:$rd, (OpNode FPR:$rs1, FPR:$rs2))];
}

class ALU_RR_F_PatFrag<string asmstr, bits<4> opcode, PatFrag OpNode> : 
        DayInstR_F<(outs FPR:$rd), (ins FPR:$rs1, FPR:$rs2), 
        asmstr, "$rd, $rs1, $rs2", 
        opcode>{
    let Pattern = [(set FPR:$rd, (OpNode FPR:$rs1, FPR:$rs2))];
}

class ALU_RR_NOT<string asmstr, bits<4> opcode> :
        DayInstR_I<(outs GPR:$rd), (ins GPR:$rs1),
                   asmstr, "$rd, $rs1",
                   opcode> {
  bits<5> dummy_rs2 = 0;
  let Inst{22-18} = dummy_rs2;
  let Pattern = [(set GPR:$rd, (not GPR:$rs1))];
}

def fsetolt : PatFrag<  (ops node:$lhs, node:$rhs), 
                        (setolt node:$lhs, node:$rhs),
                        [{return N->getOperand(0).getValueType().isFloatingPoint();}]>;

class ALU_RR_RF_PatFrag<string asmstr, bits<4> opcode, PatFrag OpNode> : 
        DayInstR_F<(outs GPR:$rd), (ins FPR:$rs1, FPR:$rs2), 
        asmstr, "$rd, $rs1, $rs2", 
        opcode>{
    let Pattern = [(set GPR:$rd, (OpNode FPR:$rs1, FPR:$rs2))];
}

/////////////////////////////////////////////////////////////////////////////////////
//// R4指令对应的Pattern
/////////////////////////////////////////////////////////////////////////////////////
class ALU_R4_I<string asmstr, bits<4> opcode> : 
        DayInstR4_I<(outs GPR:$rd), (ins GPR:$rs1, GPR:$rs2, GPR:$rs3), 
        asmstr, "$rd, $rs1, $rs2, $rs3", 
        opcode>;

class ALU_R4_F<string asmstr, bits<4> opcode> : 
        DayInstR4_F<(outs FPR:$rd), (ins FPR:$rs1, FPR:$rs2, FPR:$rs3), 
        asmstr, "$rd, $rs1, $rs2, $rs3", 
        opcode>;

/////////////////////////////////////////////////////////////////////////////////////
//// I指令对应的Pattern
/////////////////////////////////////////////////////////////////////////////////////
def simm14 : Operand<i32>, ImmLeaf<i32, [{ return isInt<14>(Imm);}]>;
def simm14j: Operand<i32>;

class ALU_I<string asmstr, bits<4> opcode, SDNode OpNode> : 
        DayInstI<(outs GPR:$rd), (ins GPR:$rs1, simm14:$im14), 
        asmstr, "$rd, $rs1, $im14", 
        opcode>{
        let Pattern = [(set GPR:$rd, (OpNode GPR:$rs1, simm14:$im14))];
}

class ALU_I_I_PatFrag<string asmstr, bits<4> opcode, PatFrag OpNode> : 
        DayInstI<(outs GPR:$rd), (ins GPR:$rs1, simm14:$im14), 
        asmstr, "$rd, $rs1, $im14", 
        opcode>{
    let Pattern = [(set GPR:$rd, (OpNode GPR:$rs1, simm14:$im14))];
}


class ALU_I_JALR<string asmstr, bits<4> opcode> : 
        DayInstI<(outs GPR:$rd), (ins GPR:$rs1, simm14j:$im14), 
        asmstr, "$rd, $rs1, $im14", 
        opcode>{
    let hasSideEffects = 1;
    let mayLoad = 0;
    let mayStore = 0;
    let Inst{21-8} = 0;
    let Pattern = [];
}

/////////////////////////////////////////////////////////////////////////////////////
//// L指令对应的Pattern
/////////////////////////////////////////////////////////////////////////////////////
def LoadTypeProfile : SDTypeProfile<1, 1, [SDTCisVT<0, i32>, SDTCisVT<1, iPTR>]>;

def M_LOAD : SDNode<
        "M_LOAD",
        LoadTypeProfile,
        [SDNPHasChain,SDNPOutGlue,SDNPInGlue,
        SDNPMayLoad,SDNPMemOperand]>;

class ALU_L<string asmstr, bits<4> opcode, PatFrag OpNode> : 
        DayInstL<(outs GPR:$rd), (ins GPR:$rs1, simm14:$im14), 
        asmstr, "$rd, $rs1($im14)", 
        opcode>{
    let Pattern = [(set GPR:$rd, (OpNode (M_LOAD (add GPR:$rs1, simm14:$im14))))];
}

/////////////////////////////////////////////////////////////////////////////////////
//// S指令对应的Pattern
/////////////////////////////////////////////////////////////////////////////////////

def StoreTypeProfile : SDTypeProfile<0, 2, [SDTCisVT<0, i32>, SDTCisVT<1, iPTR>]>;

def M_STORE : SDNode<
        "M_STORE",
        StoreTypeProfile,
        [SDNPHasChain,SDNPOutGlue,SDNPInGlue,
        SDNPMayStore,SDNPMemOperand]>;

class ALU_S<string asmstr, bits<4> opcode, PatFrag OpNode> : 
        DayInstS<(outs), (ins GPR:$rs1, GPR:$rs2, simm14:$im14), 
        asmstr, "$rs1, $rs2($im14)",
        opcode>{
    let Pattern = [(OpNode GPR:$rs1, (add GPR:$rs2, simm14:$im14))];
}

/////////////////////////////////////////////////////////////////////////////////////
//// B指令对应的Pattern
/////////////////////////////////////////////////////////////////////////////////////
def Imm16AsmOperand : AsmOperandClass {
  let Name = "Imm16";
  let PredicateMethod = "isImm16";
  let ParserMethod = "parseImm16";
}

// def simm16_br : Operand<i32> {
//   let ParserMatchClass = Imm16AsmOperand;
//   let EncoderMethod = "getBranch16TargetOpValue";
//   let OperandType = "OPERAND_PCREL";
//   let DecoderMethod = "decodeSimm16";
// }

def brtarget : Operand<OtherVT>;

class ALU_B<string asmstr, bits<4> opcode> : 
        DayInstB<(outs), (ins GPR:$rs1, GPR:$rs2, brtarget:$im16), 
        asmstr, "$rs1, $rs2, $im16", 
        opcode> {
    let isTerminator = 1;
    let Pattern = [];
}

/////////////////////////////////////////////////////////////////////////////////////
//// U指令对应的Pattern
/////////////////////////////////////////////////////////////////////////////////////

def simm32 : Operand<i32>, ImmLeaf<i32, [{ return isInt<32>(Imm);}]>;

class ALU_U<string asmstr, bits<4> opcode> : 
        DayInstU<(outs GPR:$rd), (ins simm32:$im32), 
        asmstr, "$rd, $im32",
        opcode>;

/////////////////////////////////////////////////////////////////////////////////////
//// J指令对应的Pattern
/////////////////////////////////////////////////////////////////////////////////////
class ALU_J<string asmstr> : 
        DayInstJ<(outs GPR:$rd), (ins simm32:$im32), 
        asmstr, "$rd, $im32">;

//////////////////////////////////////////////////////////////////////////////////////

def ADD : ALU_RR_I<"add", 0b0000, add>;
def SUB : ALU_RR_I<"sub", 0b0001, sub>;
def MUL : ALU_RR_I<"mul", 0b0010, mul>;
def DIV : ALU_RR_I<"div", 0b0011, sdiv>;
def SLT : ALU_RR_I_PatFrag<"slt", 0b0100, setlt>;
def SLTU: ALU_RR_I_PatFrag<"sltu",0b0101, setult>;
def AND : ALU_RR_I<"and", 0b0110, and>;
def OR  : ALU_RR_I<"or",  0b0111, or>;
def XOR : ALU_RR_I<"xor", 0b1000, xor>;
def NOT : ALU_RR_NOT<"not", 0b1010>;
def SLL : ALU_RR_I<"sll", 0b1010, shl>;
def SRL : ALU_RR_I<"srl", 0b1011, srl>;
def SRA : ALU_RR_I<"sra", 0b1100, sra>;

def ADDF: ALU_RR_F<"addf", 0b0000, fadd>;
def SUBF: ALU_RR_F<"subf", 0b0001, fsub>;
def MULF: ALU_RR_F<"mulf", 0b0010, fmul>;
def DIVF: ALU_RR_F<"divf", 0b0011, fdiv>;
def SLTF : ALU_RR_RF_PatFrag<"sltf", 0b0100, fsetolt>;

def MPAP: ALU_R4_I<"mpap", 0b0011>;
def MPAN: ALU_R4_I<"mpan", 0b0100>;
def MNAP: ALU_R4_I<"mnap", 0b0101>;
def MNAN: ALU_R4_I<"mnan", 0b0110>;
def BITSW: ALU_R4_I<"bitsw", 0b0111>;
def BYTESW: ALU_R4_I<"bytesw", 0b1000>;

def MPAPF: ALU_R4_F<"mpapf", 0b0011>;
def MPANF: ALU_R4_F<"mpanf", 0b0100>;
def MNAPF: ALU_R4_F<"mnapf", 0b0101>;
def MNANF: ALU_R4_F<"mnanf", 0b0110>;

def ADDI: ALU_I<"addi", 0b0000, add>;
def SUBI: ALU_I<"subi", 0b0001, sub>;
def MULI: ALU_I<"muli", 0b0010, mul>;
def DIVI: ALU_I<"divi", 0b0011, sdiv>;
def SLTI: ALU_I_I_PatFrag<"slti", 0b0100, setlt>;
def SLTIU: ALU_I_I_PatFrag<"sltiu", 0b0101, setult>;
def ANDI: ALU_I<"andi", 0b0110, and>;
def ORI : ALU_I<"ori",  0b0111, or>;
def XORI: ALU_I<"xor", 0b1000, xor>;
def JALR : ALU_I_JALR<"jalr", 0b1010>; // 这个地方需要定义Pat

def LB: ALU_L<"lb", 0b0000, sextloadi8 >;
def LH: ALU_L<"lh", 0b0001, sextloadi16>;
def LW: ALU_L<"lw", 0b0010, sextloadi32>;
def LBU: ALU_L<"lbu", 0b0011, extloadi8>;
def LHU: ALU_L<"lhu", 0b0100, extloadi16>;
def LWU: ALU_L<"lwu", 0b0101, extloadi32>;

def SB: ALU_S<"sb", 0b0000, truncstorei8>;
def SH: ALU_S<"sh", 0b0001, truncstorei16>;
def SW: ALU_S<"sw", 0b0010, truncstorei32>;

def BEQ: ALU_B<"beq",   0b0000>;
def BNE: ALU_B<"bne",   0b0001>;
def BLT: ALU_B<"blt",   0b0010>;
def BGE: ALU_B<"bge",   0b0011>;
def BLTU: ALU_B<"bltu", 0b0100>;
def BGEU: ALU_B<"bgeu", 0b0101>;
def : Pat<(brcond (i32 (seteq  GPR:$rs1, GPR:$rs2)), bb:$dst), (BEQ GPR:$rs1,  GPR:$rs2,  brtarget:$dst)>;
def : Pat<(brcond (i32 (setne  GPR:$rs1, GPR:$rs2)), bb:$dst), (BNE GPR:$rs1,  GPR:$rs2,  brtarget:$dst)>;
def : Pat<(brcond (i32 (setlt  GPR:$rs1, GPR:$rs2)), bb:$dst), (BLT GPR:$rs1,  GPR:$rs2,  brtarget:$dst)>;
def : Pat<(brcond (i32 (setge  GPR:$rs1, GPR:$rs2)), bb:$dst), (BGE GPR:$rs1,  GPR:$rs2,  brtarget:$dst)>;
def : Pat<(brcond (i32 (setult GPR:$rs1, GPR:$rs2)), bb:$dst), (BLTU GPR:$rs1, GPR:$rs2,  brtarget:$dst)>;
def : Pat<(brcond (i32 (setuge GPR:$rs1, GPR:$rs2)), bb:$dst), (BGEU GPR:$rs1, GPR:$rs2,  brtarget:$dst)>;

def LUI: ALU_U<"lui", 0b0000>{
    let mayLoad = 1;
    let mayStore = 0;
    let hasSideEffects = 0;
}
def AUIPC: ALU_U<"auipc", 0b0001>{
    let mayLoad = 1;
    let mayStore = 0;
    let hasSideEffects = 1;
}

/////////////////


def JAL: ALU_J<"jal">{
    let hasSideEffects = 1;
    let mayStore = 0;
    let mayLoad = 0;
}

