//
//
//
//
//



/***********************************************************************************
新手必读！！！！

DayInstrInfo，顾名思义，就是定义指令信息的（instruction information）,这里会涉及到指令的具体内容。
如add,load,store等等.

指令嘛，无非就是R,I,S,B,U,J类型, 无论怎么变都是其中的类型之一。
所以我们要继承 DayInstrFormats 中定义好的类型。

说实话，我实在不喜欢RISCV的写的指令信息的tablegen文件，看起来令人头大，没有分块，后期RISCV指令集急剧扩充
所写的代码必须要有一定的兼容性，所以可能这也是没有办法的事情。
riscv IntsrInfo的tablegen读起来很难受，所以这里参考了一些简单教程的写法，希望新手能看懂吧

***********************************************************************************/

include "DayInstrFormats.td"


//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
// 这里定义ALU_rr class,主要是针对R类型描述的，入口参数OPC_OP就是instr formats文件中所定义的
// hasSideEffects = 0, mayLoad = 0, mayStore = 0，分别代表：无副作用，不加载，不存储
// 严重怀疑这段代码有些炫技...
let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in
class ALU_rr<bits<7> funct7, bits<3> funct3, string opcodestr,
             bit Commutable = 0>
    : RVInstR<funct7, funct3, OPC_OP, (outs GPR:$rd), (ins GPR:$rs1, GPR:$rs2),
              opcodestr, "$rd, $rs1, $rs2"> {
  let isCommutable = Commutable;
}
// 正常写法，如下：
/*********这段注释中是正常写法*************
class ALU_rr<bits<7> funct7, bits<3> funct3, string opcodestr,
             bit Commutable = 0>
    : RVInstR<funct7, funct3, OPC_OP, (outs GPR:$rd), (ins GPR:$rs1, GPR:$rs2),
              opcodestr, "$rd, $rs1, $rs2"> {
  let hasSideEffects = 0;   
  let mayLoad = 0;              
  let mayStore = 0;         
  let isCommutable = Commutable;
}
****************************************/

///////////////////////////////////////////////////////////
// 根据R类型的class,定义一些R类型的指令
def ADD  : ALU_rr<0b0000000, 0b000, "add", Commutable=1>,
           Sched<[WriteIALU, ReadIALU, ReadIALU]>;

def SUB  : ALU_rr<0b0100000, 0b000, "sub">,
           Sched<[WriteIALU, ReadIALU, ReadIALU]>;

def SLL  : ALU_rr<0b0000000, 0b001, "sll">,
           Sched<[WriteShiftReg, ReadShiftReg, ReadShiftReg]>;

// 这里要注意一下：
// let IsSignExtendingOpW = 1 in { ... } 的意思就是：
// 在这个花括号内部，所有定义的指令都将隐式地拥有 IsSignExtendingOpW 这个属性，并且它的值为 1
// 这里是有符号比较（slt）和无符号比较（sltu）,其结果都会进行符号扩展。
//  如果结果是 0，那么扩展后是全 0。
//  如果结果是 1，那么扩展后是全 1（即 -1 的补码表示）
//  
    /****************************************************************
    let IsSignExtendingOpW = 1 in { ... } 声明了一个作用域，
    在该作用域内定义的指令（SLT 和 SLTU）都将具有 IsSignExtendingOpW 属性并设置为 1。
    这个属性可能用于LLVM的内部处理，指示编译器在处理这些指令的输出时，
    应该考虑到其结果（0或1）在某些情况下可能需要被符号扩展，尽管 RISC-V 本身是零扩展的。
    ****************************************************************/
let IsSignExtendingOpW = 1 in {
def SLT  : ALU_rr<0b0000000, 0b010, "slt">,
           Sched<[WriteIALU, ReadIALU, ReadIALU]>;
def SLTU : ALU_rr<0b0000000, 0b011, "sltu">,
           Sched<[WriteIALU, ReadIALU, ReadIALU]>;
}

def XOR  : ALU_rr<0b0000000, 0b100, "xor", Commutable=1>,
           Sched<[WriteIALU, ReadIALU, ReadIALU]>;

def SRL  : ALU_rr<0b0000000, 0b101, "srl">,
           Sched<[WriteShiftReg, ReadShiftReg, ReadShiftReg]>;

def SRA  : ALU_rr<0b0100000, 0b101, "sra">,
           Sched<[WriteShiftReg, ReadShiftReg, ReadShiftReg]>;

def OR   : ALU_rr<0b0000000, 0b110, "or", Commutable=1>,
           Sched<[WriteIALU, ReadIALU, ReadIALU]>;

def AND  : ALU_rr<0b0000000, 0b111, "and", Commutable=1>,
           Sched<[WriteIALU, ReadIALU, ReadIALU]>;



///////////////////////////////////////////////////////////
// 根据I类型的class,定义一些I类型的指令

let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in
class ALU_ri<bits<3> funct3, string opcodestr>
    : RVInstI<funct3, OPC_OP_IMM, (outs GPR:$rd), (ins GPR:$rs1, simm12:$imm12),
              opcodestr, "$rd, $rs1, $imm12">,
      Sched<[WriteIALU, ReadIALU]>;

let isReMaterializable = 1, isAsCheapAsAMove = 1 in
def ADDI  : ALU_ri<0b000, "addi">;

let IsSignExtendingOpW = 1 in {
def SLTI  : ALU_ri<0b010, "slti">;
def SLTIU : ALU_ri<0b011, "sltiu">;
}

let isReMaterializable = 1, isAsCheapAsAMove = 1 in {
def XORI  : ALU_ri<0b100, "xori">;
def ORI   : ALU_ri<0b110, "ori">;
}

def ANDI  : ALU_ri<0b111, "andi">;





////////////////////////////////////////////////////////
// 根据B类型的class,定义一些B类型的指令
let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in
class BranchCC_rri<bits<3> funct3, string opcodestr>
    : RVInstB<funct3, OPC_BRANCH, (outs),
              (ins GPR:$rs1, GPR:$rs2, simm13_lsb0:$imm12),
              opcodestr, "$rs1, $rs2, $imm12">,
      Sched<[WriteJmp, ReadJmp, ReadJmp]> {
  let isBranch = 1;
  let isTerminator = 1;
}

def BEQ  : BranchCC_rri<0b000, "beq">;
def BNE  : BranchCC_rri<0b001, "bne">;
def BLT  : BranchCC_rri<0b100, "blt">;
def BGE  : BranchCC_rri<0b101, "bge">;
def BLTU : BranchCC_rri<0b110, "bltu">;
def BGEU : BranchCC_rri<0b111, "bgeu">;


////////////////////////////////////////////////////////
// 根据Load对应的opcode,定义一些load指令
let hasSideEffects = 0, mayLoad = 1, mayStore = 0 in {
class Load_ri<bits<3> funct3, string opcodestr, DAGOperand rty = GPR>
    : RVInstI<funct3, OPC_LOAD, (outs rty:$rd), (ins GPRMem:$rs1, simm12:$imm12),
              opcodestr, "$rd, ${imm12}(${rs1})">;

class HLoad_r<bits<7> funct7, bits<5> funct5, string opcodestr>
    : RVInstR<funct7, 0b100, OPC_SYSTEM, (outs GPR:$rd),
              (ins GPRMemZeroOffset:$rs1), opcodestr, "$rd, $rs1"> {
  let rs2 = funct5;
}

}

let IsSignExtendingOpW = 1 in {
def LB  : Load_ri<0b000, "lb">, Sched<[WriteLDB, ReadMemBase]>;
def LH  : Load_ri<0b001, "lh">, Sched<[WriteLDH, ReadMemBase]>;
def LW  : Load_ri<0b010, "lw">, Sched<[WriteLDW, ReadMemBase]>;
def LBU : Load_ri<0b100, "lbu">, Sched<[WriteLDB, ReadMemBase]>;
def LHU : Load_ri<0b101, "lhu">, Sched<[WriteLDH, ReadMemBase]>;
}


////////////////////////////////////////////////////////
// 根据store(也就是S类型指令)对应的opcode,定义一些stor指令
let hasSideEffects = 0, mayLoad = 0, mayStore = 1 in {
class Store_rri<bits<3> funct3, string opcodestr, DAGOperand rty = GPR>
    : RVInstS<funct3, OPC_STORE, (outs),
              (ins rty:$rs2, GPRMem:$rs1, simm12:$imm12),
              opcodestr, "$rs2, ${imm12}(${rs1})">;

class HStore_rr<bits<7> funct7, string opcodestr>
    : RVInstR<funct7, 0b100, OPC_SYSTEM, (outs),
              (ins GPR:$rs2, GPRMemZeroOffset:$rs1),
               opcodestr, "$rs2, $rs1"> {
  let rd = 0;
}
}

def SB : Store_rri<0b000, "sb">, Sched<[WriteSTB, ReadStoreData, ReadMemBase]>;
def SH : Store_rri<0b001, "sh">, Sched<[WriteSTH, ReadStoreData, ReadMemBase]>;
def SW : Store_rri<0b010, "sw">, Sched<[WriteSTW, ReadStoreData, ReadMemBase]>;


///////////////////////////////////////////////////////////
// 根据U,J,I类型的class,定义一些相关类型的指令，比较鱼龙混杂，instrformats中也提到过

let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in {
let isReMaterializable = 1, isAsCheapAsAMove = 1,
    IsSignExtendingOpW = 1 in
def LUI : RVInstU<OPC_LUI, (outs GPR:$rd), (ins uimm20_lui:$imm20),
                  "lui", "$rd, $imm20">, Sched<[WriteIALU]>;

def AUIPC : RVInstU<OPC_AUIPC, (outs GPR:$rd), (ins uimm20_auipc:$imm20),
                    "auipc", "$rd, $imm20">, Sched<[WriteIALU]>;

def JAL : RVInstJ<OPC_JAL, (outs GPR:$rd), (ins simm21_lsb0_jal:$imm20),
                  "jal", "$rd, $imm20">, Sched<[WriteJal]>;

def JALR : RVInstI<0b000, OPC_JALR, (outs GPR:$rd),
                   (ins GPR:$rs1, simm12:$imm12),
                   "jalr", "$rd, ${imm12}(${rs1})">,
           Sched<[WriteJalr, ReadJalr]>;
} // hasSideEffects = 0, mayLoad = 0, mayStore = 0

///////////////////////////////////////////////////////////
// 这个地方是最后那几个特殊的instruction
def FENCE : RVInstI<0b000, OPC_MISC_MEM, (outs),
                    (ins fencearg:$pred, fencearg:$succ),
                    "fence", "$pred, $succ">, Sched<[]> {
  bits<4> pred;
  bits<4> succ;

  let rs1 = 0;
  let rd = 0;
  let imm12 = {0b0000,pred,succ};
}

def FENCE_TSO : RVInstI<0b000, OPC_MISC_MEM, (outs), (ins), "fence.tso", "">, Sched<[]> {
  let rs1 = 0;
  let rd = 0;
  let imm12 = {0b1000,0b0011,0b0011};
}

def ECALL : RVInstI<0b000, OPC_SYSTEM, (outs), (ins), "ecall", "">, Sched<[WriteJmp]> {
  let rs1 = 0;
  let rd = 0;
  let imm12 = 0;
}

def EBREAK : RVInstI<0b000, OPC_SYSTEM, (outs), (ins), "ebreak", "">,
             Sched<[]> {
  let rs1 = 0;
  let rd = 0;
  let imm12 = 1;
}

//// 基本指令集到这里就差不多没有了
////////////////////////////////////////////////////////////////////////////////
//// 再往后边就是一些扩展指令了，当打开下边的文档后：
////《The RISC-V Instruction Set Manual Volume I: Unprivileged Architecture, Version 20240411》
//// 只看它的目录，发现最重要的是第2章，也就是RV32I最基础的部分
//// 第3-5章，用不上，不是一个指令集的
//// 第6-12章，有好多Z开头的扩展指令集，其实有些还是很重要的，比如第7章的Zicsr，这里先不涉及
//// 第13章，M乘法指令，不管
//// 第14章，A原子操作，不管
//// 第15-16章，不管
//// 第17章，RVWMO是什么东东？不管
//// 第18-33章，感觉也用不到啊，先不管
//// 第34章，还是有用的，最基本的opcode在这章都有定义！
//// 其余章节，不用管