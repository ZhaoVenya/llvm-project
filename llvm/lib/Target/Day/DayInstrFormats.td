// 
// 
// 


/***********************************************************************************
新手必读！！！！
DayInstrFormat.td文件是最基础的tablegen文件之一，该文件没有以来任何的其他的tablegen文件，完全属于自定义的。
当然，在自定义的过程中，是需要继承一些类的，这些类中本来就存在一些属性/初始状态，我们拿过来直接用或者修改就可以了。
比如：Instruction本来就是被定义好的class, 只需要继承它就型了，里边也有对应的属性，如AsmString/Pattern等。
具体写的过程还有详细的解释。

DayInstrFormat，顾名思义，就是定义指令格式的（instruction format）,只是涉及到指令格式，但不会涉及到指令的具体内容。
关于指令的具体内容会在另一个文件中定义, 也就是DayInstrInfo, DayInstrInfo.td文件中会定义具体的指令，如add,load,store等等

开始详细阅读第一份比较大的tablegen文件的注释吧！
***********************************************************************************/


// 定义Format, 是一个Class，它是用来简单的说明不同指令类型的。
class InstFormat<bits<4> val>
{
  bits<4> Value = val;
}

// 这里是不同的指令对应的不同编号，
// 因为我们用的是RISCV结构，
// 所以这里按照RISCV的指令类型进行定义，
// 也就是定义RV32I指令类型，RV32I基本指令标准参见以下连接（非特权模式的就行，感觉会简单些）：
// https://github.com/riscv/riscv-isa-manual/releases

// 伪指令，就是碰到一些内核本身不支持的指令，可以翻译成单个/多个指令的集合，例如，架构不支持mul,就转换成许多加法/位移等
// R 类型(Register-Register),格式：opcode rd,rs1,rs2,一般的and,add,sub都属于R类型
// I 类型(Immediate), 立即数运算/加载,但最高只支持12位的立即数，例如：addi rd,rs1,100
// S 类型(Store)，将数据从寄存器存储到内存（无 rd 字段），例如： sw x1,8(x2) #将 x1 的值存储到 Memory[x2+8]
// B 类型(Branch)，条件分支跳转（基于寄存器比较），例如：beq x1, x2, label # 若 x1 == x2，跳转到 label
// U 类型(Upper Immediate)，加载 20 位高位立即数（见之前解释）例如：lui x1, 0x12345 # x1 = 0x12345000
// J 类型(Jump)，长距离无条件跳转，如函数调用，如：jal x1, func # 跳转到 func，返回地址存 x1
// 这里只是用不同的数值描述不同的类型，就是一个编号而已，具体信息还需要单独定义。
def InstFormatPseudo    : InstFormat<0>; 
def InstFormatR         : InstFormat<1>; 
def InstFormatI         : InstFormat<2>; 
def InstFormatS         : InstFormat<3>; 
def InstFormatB         : InstFormat<4>; 
def InstFormatU         : InstFormat<5>; 
def InstFormatJ         : InstFormat<6>; 


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// 这里定义一个DayOpcode,用于方便定义操作码值
class DayOpcode<string name, bits<7> val> {
  string Name = name;
  bits<7> Value = val;
}
// 这里定义不同类型的操作码值，这里定义的不同操作码值，只是对应 RV32I Base Instruction Set
// 详见《The RISC-V Instruction Set Manual Volume I》的 Chapter 35. RV32/64G Instruction Set Listings 章节
// 没错，尽管一些opcode和指令类型是一一对应的，但是并非全部对应，这就很难受了。
// 有些类型的指令竟然占了好几个操作码
// 此外，还有一些不知道是什么类型的操作码，比如 ecall和ebreak，fence.os和pause...
def OPC_LOAD      : DayOpcode<"LOAD",      0b0000011>;// Load指令相关的东西
def OPC_MISC_MEM  : DayOpcode<"MISC_MEM",  0b0001111>;// fence.os和pause
def OPC_OP_IMM    : DayOpcode<"OP_IMM",    0b0010011>;// I类型指令
def OPC_AUIPC     : DayOpcode<"AUIPC",     0b0010111>;// AUPIC, 这个是U型的
def OPC_STORE     : DayOpcode<"STORE",     0b0100011>;// S型指令
def OPC_OP        : DayOpcode<"OP",        0b0110011>;// R型指令
def OPC_LUI       : DayOpcode<"LUI",       0b0110111>;// LUI, 这个是U型的
def OPC_BRANCH    : DayOpcode<"BRANCH",    0b1100011>;// B型指令
def OPC_JALR      : DayOpcode<"JALR",      0b1100111>;// JALR I型指令
def OPC_JAL       : DayOpcode<"JAL",       0b1101111>;// JAL J型指令
def OPC_SYSTEM    : DayOpcode<"SYSTEM",    0b1110011>;// ecall和ebreak

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// 这里定义DayInst，输出，输入，汇编字符串，pattern
class DayInst<dag outs, dag ins, string opcodestr, string argstr, 
            list<dag> pattern, InstFormat Form> : Instruction
{
    
    let Namespace = "Day";// DayInst的namespace

    /*********************************************************
    把对应的入口参数赋值给对应的属性
    let开头的是重新赋值
    dag/bits/field似乎是重新创建了一些属性。
    如果所继承的Instruction有OutOperandList InOperandList这两个属性，
    也不必担心会产生冲突，而是会覆盖掉原来的信息。
    **********************************************************/
    dag OutOperandList = outs;
    dag InOperandList  = ins;
    let AsmString = opcodestr # !if(!empty(argstr), "", "\t" # argstr);
    let Pattern   = pattern;
    let TSFlags{3-0} = Form.Value;

    //// 这里定义了一个指令的位宽，其中低7bit是操作符
    field bits<32> Inst;
    bits<7> Opcode = 0;
    let Inst{6-0} = Opcode;

    //// DayInst的namespace
    let DecoderNamespace = "Day";
    let Size = 4; // 指令的长度是4Bytes,(当然如此，因为是32bits的指令嘛)
    field bits<32> SoftFail = 0; // 默认不触发软失败(我也不太懂这个地方是什么意思)
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
//// 这里定义RISCV R类型的基本指令集的格式，因为 R类型指令有很多变体，所以有一个RBase还有一个R
class RVInstRBase<bits<3> funct3, RISCVOpcode opcode, dag outs,
                  dag ins, string opcodestr, string argstr>
    : RVInst<outs, ins, opcodestr, argstr, [], InstFormatR> {
  bits<5> rs2;
  bits<5> rs1;
  bits<5> rd;

  let Inst{24-20} = rs2;
  let Inst{19-15} = rs1;
  let Inst{14-12} = funct3;
  let Inst{11-7} = rd;
  let Inst{6-0} = opcode.Value;
}

class RVInstR<bits<7> funct7, bits<3> funct3, RISCVOpcode opcode, dag outs,
              dag ins, string opcodestr, string argstr>
    : RVInstRBase<funct3, opcode, outs, ins, opcodestr, argstr> {
  let Inst{31-25} = funct7;
}

//// 这里定义RISCV I类型的基本指令集的格式，因为I类型指令有很多变体，所以有一个IBase还有一个I
class RVInstIBase<bits<3> funct3, RISCVOpcode opcode, dag outs, dag ins,
                  string opcodestr, string argstr>
    : RVInst<outs, ins, opcodestr, argstr, [], InstFormatI> {
  bits<5> rs1;
  bits<5> rd;

  let Inst{19-15} = rs1;
  let Inst{14-12} = funct3;
  let Inst{11-7} = rd;
  let Inst{6-0} = opcode.Value;
}

class RVInstI<bits<3> funct3, RISCVOpcode opcode, dag outs, dag ins,
              string opcodestr, string argstr>
    : RVInstIBase<funct3, opcode, outs, ins, opcodestr, argstr> {
  bits<12> imm12;

  let Inst{31-20} = imm12;
}

//// 这里定义RISCV S类型的基本指令集的格式, 因为S类型指令比较简单粗暴，所以就没有定义SBase
class RVInstS<bits<3> funct3, RISCVOpcode opcode, dag outs, dag ins,
              string opcodestr, string argstr>
    : RVInst<outs, ins, opcodestr, argstr, [], InstFormatS> {
  bits<12> imm12;
  bits<5> rs2;
  bits<5> rs1;

  let Inst{31-25} = imm12{11-5};
  let Inst{24-20} = rs2;
  let Inst{19-15} = rs1;
  let Inst{14-12} = funct3;
  let Inst{11-7} = imm12{4-0};
  let Inst{6-0} = opcode.Value;
}

//// 这里定义RISCV B类型的基本指令集的格式, 因为B类型指令比较简单粗暴，所以就没有定义BBase
class RVInstB<bits<3> funct3, RISCVOpcode opcode, dag outs, dag ins,
              string opcodestr, string argstr>
    : RVInst<outs, ins, opcodestr, argstr, [], InstFormatB> {
  bits<12> imm12;
  bits<5> rs2;
  bits<5> rs1;

  let Inst{31} = imm12{11};
  let Inst{30-25} = imm12{9-4};
  let Inst{24-20} = rs2;
  let Inst{19-15} = rs1;
  let Inst{14-12} = funct3;
  let Inst{11-8} = imm12{3-0};
  let Inst{7} = imm12{10};
  let Inst{6-0} = opcode.Value;
}

//// 这里定义RISCV U类型的基本指令集的格式 因为U类型指令比较简单粗暴，所以就没有定义UBase
class RVInstU<RISCVOpcode opcode, dag outs, dag ins, string opcodestr,
              string argstr>
    : RVInst<outs, ins, opcodestr, argstr, [], InstFormatU> {
  bits<20> imm20;
  bits<5> rd;

  let Inst{31-12} = imm20;
  let Inst{11-7} = rd;
  let Inst{6-0} = opcode.Value;
}

//// 这里定义RISCV J类型的基本指令集的格式 因为J类型指令比较简单粗暴，所以就没有定义JBase
class RVInstJ<RISCVOpcode opcode, dag outs, dag ins, string opcodestr,
              string argstr>
    : RVInst<outs, ins, opcodestr, argstr, [], InstFormatJ> {
  bits<20> imm20;
  bits<5> rd;

  let Inst{31} = imm20{19};
  let Inst{30-21} = imm20{9-0};
  let Inst{20} = imm20{10};
  let Inst{19-12} = imm20{18-11};
  let Inst{11-7} = rd;
  let Inst{6-0} = opcode.Value;
}



