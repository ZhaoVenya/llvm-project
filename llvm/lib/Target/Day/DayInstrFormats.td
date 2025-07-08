/************************************************************\
    * File: llvm/lib/Target/Day/DayInstrFormats.td
    * Description: Instruction formats for the Day architecture.
\************************************************************/

// 定义InstFormat,代表的是不同指令类型

class InstFormat<bits<4> val>{
    bits<4> Value =val;
}
// 这里就是对不同的指令形式进行简单的编号，和指令码具体内容相关不是很大
def InstFormatPseudo : InstFormat<0b0000>;
def InstFormatR      : InstFormat<0b0001>;
def InstFormatR4     : InstFormat<0b0010>;
def InstFormatI      : InstFormat<0b0011>;
def InstFormatL      : InstFormat<0b0100>;
def InstFormatS      : InstFormat<0b0101>;
def InstFormatB      : InstFormat<0b0110>;
def InstFormatU      : InstFormat<0b0111>;
def InstFormatJ      : InstFormat<0b1000>;


// 这里定义不同数据类型对应的字段码，比如整数、浮点数等
class Func4Constants {
  field bits<4> func4_int;
  field bits<4> func4_float;
  let func4_int   = 0b0000;
  let func4_float = 0b0001;
}
def Func4 : Func4Constants;

class Func5Constants {
  field bits<5> val;  
  let val   = 0b00000;
}
def Func5 : Func5Constants;

// 定义指令类型
class DayTypeCode<string typename, bits<4> typecode> {
    string TypeName;
    bits<4> Code;
    let TypeName = typename;
    let Code = typecode;
}

// 不同类型指令对应的“类型字段”的码值
def DayTypeR      : DayTypeCode<"R",  0b0001>;
def DayTypeR4     : DayTypeCode<"R4", 0b0010>;
def DayTypeI      : DayTypeCode<"I",  0b0011>;
def DayTypeL      : DayTypeCode<"L",  0b0100>;
def DayTypeS      : DayTypeCode<"S",  0b0101>;
def DayTypeB      : DayTypeCode<"B",  0b0110>;
def DayTypeU      : DayTypeCode<"U",  0b0111>;
def DayTypeJ      : DayTypeCode<"J",  0b1000>;

// 
class DayInst<dag outs, dag ins, string asmstr, list<dag> pattern, DayTypeCode daytype, InstFormat instrformat>:Instruction{

    field bits<32> SoftFail = 0;
    field bits<32> Inst;
    field bits<4> InstrFormatValue = instrformat.Value;// 只是用于标记，并非是实际指令码指中的一部分

    let Size = 4; // Assuming 32-bit instructions
    let Namespace = "Day";
    dag OutOperandList = outs;
    dag InOperandList = ins;
    let AsmString = asmstr;
    let Pattern = pattern;
}


// 定义R类型指令的格式
class R_Form<bits<4> opcode, bits<4> func4_val, bits<5> func5_val>
    : DayInst<(outs GPR:$rd), (ins GPR:$rs1, GPR:$rs2), "", [], DayTypeR, InstFormatR> {
    // 定义R_Form特有的字段
    field bits<4> func4 = func4_val;
    field bits<5> func5 = func5_val;
    field bits<5> rs2;
    field bits<5> rs1;
    field bits<5> rd;
    field bits<4> Opcode = opcode; // R类型指令的操作码

    // 这里的 Inst{} 赋值是核心，它定义了二进制指令的布局
    let Inst{31-28} = func4;
    let Inst{27-23} = func5;
    let Inst{22-18} = rs2;
    let Inst{17-13} = rs1;
    let Inst{12-8}  = rd;
    let Inst{7-4}   = Opcode;
    // 在这里将 DayInstrType 映射到 Inst 的位上
    let Inst{3-0}   = DayTypeR.Code; //
}

// 定义R4类型指令的格式
class R4_Form<bits<4> opcode, bits<4> func4_val>
    : DayInst<(outs GPR:$rd), (ins GPR:$rs1, GPR:$rs2, GPR:$rs3), "", [], DayTypeR4, InstFormatR4> {
    // 定义R4_Form特有的字段
    field bits<4> func4 = func4_val;
    field bits<5> rs3;
    field bits<5> rs2;
    field bits<5> rs1;
    field bits<5> rd;
    field bits<4> Opcode = opcode; // R4类型指令的操作码
    // 这里的 Inst{} 赋值是核心，它定义了二进制指令的布局
    let Inst{31-28} = func4;
    let Inst{27-23} = rs3;
    let Inst{22-18} = rs2;
    let Inst{17-13} = rs1;
    let Inst{12-8}  = rd;
    let Inst{7-4}   = Opcode;
    // 在这里将 DayInstrType 映射到 Inst 的位上
    let Inst{3-0}   = DayTypeR4.Code; // R4类型指令的类型字段码
}

class I_Form<bits<4> opcode, bits<14> imm14_val>
    : DayInst<(outs GPR:$rd), (ins GPR:$rs1), "", [], DayTypeI, InstFormatI> {
    // 定义I_Form特有的字段
    field bits<14> imm = imm14_val; // 立即数值
    field bits<5> rs1;
    field bits<5> rd;
    field bits<4> Opcode = opcode; // I类型指令的操作码

    // 这里的 Inst{} 赋值是核心，它定义了二进制指令的布局
    let Inst{31-18} = imm;
    let Inst{17-13} = rs1;
    let Inst{12-8}  = rd;
    let Inst{7-4}   = Opcode;
    // 在这里将 DayInstrType 映射到 Inst 的位上
    let Inst{3-0}   = DayTypeI.Code; // I类型指令的类型字段码
};

class L_Form<bits<4> opcode, bits<14> imm14_val>
    : DayInst<(outs GPR:$rd), (ins GPR:$rs1), "", [], DayTypeL, InstFormatL> {
    // 定义L_Form特有的字段
    field bits<14> imm = imm14_val{13-0}; // 立即数值
    field bits<5> rs1;
    field bits<5> rd;
    field bits<4> Opcode = opcode; // L类型指令的操作码

    // 这里的 Inst{} 赋值是核心，它定义了二进制指令的布局
    let Inst{31-18} = imm;
    let Inst{17-13} = rs1;
    let Inst{12-8}  = rd;
    let Inst{7-4}   = Opcode;
    // 在这里将 DayInstrType 映射到 Inst 的位上
    let Inst{3-0}   = DayTypeL.Code; // L类型指令的类型字段码
};

//
class S_Form<bits<4> opcode, bits<14> imm14_val>
    : DayInst<(outs), (ins GPR:$rs1, GPR:$rs2), "", [], DayTypeS, InstFormatS> {
    // 定义S_Form特有的字段
    field bits<14> imm = imm14_val{13-0}; // 立即数值
    field bits<5> rs1;
    field bits<5> rs2;
    field bits<4> Opcode = opcode; // S类型指令的操作码

    // 这里的 Inst{} 赋值是核心，它定义了二进制指令的布局
    let Inst{31-18} = imm;
    let Inst{17-13} = rs1;
    let Inst{12-8}  = rs2;
    let Inst{7-4}   = Opcode;
    // 在这里将 DayInstrType 映射到 Inst 的位上
    let Inst{3-0}   = DayTypeS.Code; // S类型指令的类型字段码
};

class B_Form<bits<4> opcode, bits<16> imm16_val>
    : DayInst<(outs), (ins GPR:$rs1, GPR:$rs2), "", [], DayTypeB, InstFormatB> {
    // 定义B_Form特有的字段
    field bits<14> imm = imm16_val{15-2}; // 立即数值
    field bits<5> rs1;
    field bits<5> rs2;
    field bits<4> Opcode = opcode; // B类型指令的操作码

    // 这里的 Inst{} 赋值是核心，它定义了二进制指令的布局
    let Inst{31-18} = imm;
    let Inst{17-13} = rs1;
    let Inst{12-8}  = rs2;
    let Inst{7-4}   = Opcode;
    // 在这里将 DayInstrType 映射到 Inst 的位上
    let Inst{3-0}   = DayTypeB.Code; // B类型指令的类型字段码
};

//// 需要修改U指令和J指令的定义

class U_Form<bits<4> opcode, bits<32> imm32_val>
    : DayInst<(outs GPR:$rd), (), "", [], DayTypeU, InstFormatU> {
    // 定义U_Form特有的字段
    field bits<18> imm = imm32_val{31-14}; // 立即数值
    field bits<5> rd;
    field bits<4> Opcode = opcode; // U类型指令的操作码

    // 这里的 Inst{} 赋值是核心，它定义了二进制指令的布局
    let Inst{31} = 0b0;// U类型指令的最高位通常为0
    let Inst{30-13} = imm;
    let Inst{12-8}  = rd;
    let Inst{7-4}   = Opcode;
    // 在这里将 DayInstrType 映射到 Inst 的位上
    let Inst{3-0}   = DayTypeU.Code; // U类型指令的类型字段码
};


class J_Form<bits<4> opcode, bits<32> imm32_val>
    : DayInst<(outs GPR:$rd), (), "", [], DayTypeJ, InstFormatJ> {
    // 定义J_Form特有的字段
    field bits<25> imm = imm32_val{25-0}; // 立即数值
    field bits<5> rd;
    field bits<4> Opcode = opcode; // J类型指令的操作码

    // 这里的 Inst{} 赋值是核心，它定义了二进制指令的布局
    let Inst{31-13} = imm{24-6};
    let Inst{12-8}  = rd;
    let Inst{7-4}   = imm{5-2};
    // 在这里将 DayInstrType 映射到 Inst 的位上
    let Inst{3-0}   = DayTypeJ.Code; // J类型指令的类型字段码
};


class DayInstR<bits<4> opcode, bits<4> func4_val, string asmstr> : \
    R_Form<opcode, func4_val, Func5.val>{
    let AsmString = asmstr;
};

class DayInstR4<bits<4> opcode, bits<4> func4_val, string asmstr> : \
    R4_Form<opcode, func4_val>{
    let AsmString = asmstr;
    let Pattern = [];
};

class DayInstI<bits<4> opcode, bits<14> imm14_val, string asmstr>
    : I_Form<opcode, imm14_val>{
    let AsmString = asmstr;
};

class DayInstL<bits<4> opcode, bits<14> imm14_val, string asmstr>
    : L_Form<opcode, imm14_val>{
    let AsmString = asmstr;
    let Defs = [GPR:$rd];
    let Uses = [GPR:$rs1];
};

class DayInstS<bits<4> opcode, bits<14> imm14_val, string asmstr>
    : S_Form<opcode, imm14_val>{
    let AsmString = asmstr;
    let Defs = [];
    let Uses = [GPR:$rs1, GPR:$rs2];
};

class DayInstB<bits<4> opcode, bits<16> imm16_val, string asmstr>
    : B_Form<opcode, imm16_val>{
    let AsmString = asmstr;
    let Defs = []; // 分支指令通常没有输出寄存器
    let Uses = [GPR:$rs1, GPR:$rs2]; // 比较操作数
};

class DayInstU<bits<4> opcode, bits<32> imm32_val, string asmstr>
    : U_Form<opcode, imm32_val>{
    let AsmString = asmstr;
};

class DayInstJ<bits<4> opcode, bits<32> imm32_val, string asmstr>
    : J_Form<opcode, imm32_val>{
    let AsmString = asmstr;
};







