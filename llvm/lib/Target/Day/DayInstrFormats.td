/************************************************************\
    * File: llvm/lib/Target/Day/DayInstrFormats.td
    * Description: Instruction formats for the Day architecture.
\************************************************************/

// 定义InstFormat,代表的是不同指令类型

class InstFormat<bits<4> val>{
    bits<4> Value = val;
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


class DayInstCommon<dag outs, dag ins, string asmstr, string argstr, list<dag> pattern, InstFormat format>:
    Instruction
{
    let Namespace = "Day";
    dag OutOperandList = outs;
    dag InOperandList = ins;
    let AsmString = asmstr # !if(!empty(argstr),"","\t" # argstr);
    let Pattern = pattern;
    let isPseudo = 0; // 是否是伪指令
    let isCommutable = 0;

    let TSFlags{3-0} = format.Value; // 指令格式的类型字段码

}

class DayInst<  dag outs, dag ins, string asmstr, string argstr,
                list<dag> pattern, InstFormat format>
    : DayInstCommon<outs, ins, asmstr, argstr , 
                    pattern, format>
{
    // 定义指令的操作码
    field bits<32> Inst;
    field bits<32> SoftFail = 0;
    let Size = 4; // 其他位可以根据需要设置为0或其他值
}

//////////////////////////////////////////////////////////////////////////////////
//// 定义R类型指令的格式
//////////////////////////////////////////////////////////////////////////////////
class DayInstRBase< bits<4> func4_val, bits<5> func5_val,
                    dag outs, dag ins, 
                    string asmstr, string argstr>:
        DayInst<outs, ins, asmstr, argstr, [], InstFormatR>
{
    bits<5> rs2;
    bits<5> rs1;
    bits<5> rd;
    bits<4> f4 = func4_val;
    bits<5> f5 = func5_val;

    let Inst{31-28} = f4; // func4_val
    let Inst{27-23} = f5; // func5_val
    let Inst{22-18} = rs2; // 第二个输入寄存器
    let Inst{17-13} = rs1; // 第一个输入寄存器
    let Inst{12-8}  = rd; // 输出寄存器
    let Inst{3-0} = InstFormatR.Value; // R类型指令的类型字段码
}

class DayInstR_I<dag outs, dag ins, 
                 string asmstr, string argstr,
                 bits<4> opcode> : 
        DayInstRBase< Func4.func4_int, Func5.val, outs, ins, asmstr, argstr> 
{
    let Inst{7-4} = opcode; // 操作码
}

class DayInstR_F<dag outs, dag ins, 
                string asmstr, string argstr, 
                bits<4> opcode> :
        DayInstRBase< Func4.func4_float, Func5.val, outs, ins, asmstr, argstr> 
{
    let Inst{7-4} = opcode; // 操作码
}


//////////////////////////////////////////////////////////////////////////////////
//// 定义R4类型指令的格式
//////////////////////////////////////////////////////////////////////////////////

class DayInstR4Base< bits<4> func4_val,
                    dag outs, dag ins, 
                    string asmstr, string argstr>:
        DayInst<outs, ins, asmstr, argstr, [], InstFormatR4>
{
    bits<5> rs3; // 第三个输入寄存器
    bits<5> rs2;
    bits<5> rs1;
    bits<5> rd;

    let Inst{31-28} = func4_val; // func4_val
    let Inst{27-23} = rs3; // func5_val
    let Inst{22-18} = rs2; // 第二个输入寄存器
    let Inst{17-13} = rs1; // 第一个输入寄存器
    let Inst{12-8}  = rd; // 输出寄存器
    let Inst{3-0} = InstFormatR4.Value; // R类型指令的类型字段码
    
    // 关键属性声明
    let hasSideEffects = 0;
    let mayLoad = 0;
    let mayStore = 0;
    let isCodeGenOnly = 1;
    let Pattern = [];
}

class DayInstR4_I<  dag outs, dag ins, 
                    string asmstr, string argstr,
                    bits<4> opcode> : 
        DayInstR4Base< Func4.func4_int, outs, ins, asmstr, argstr> {
    let Inst{7-4} = opcode; // 操作码
}

class DayInstR4_F<dag outs, dag ins, string asmstr, string argstr, bits<4> opcode> : 
        DayInstR4Base< Func4.func4_float, outs, ins, asmstr, argstr> {
    let Inst{7-4} = opcode; // 操作码
}

//////////////////////////////////////////////////////////////////////////////////
//// 定义I类型指令的格式
//////////////////////////////////////////////////////////////////////////////////
class DayInstIBase<dag outs, dag ins, string asmstr, string argstr>: 
    DayInst<outs, ins, asmstr, argstr, [], InstFormatI>
{
    bits<14> im14; // 立即数值
    bits<5> rs1; // 第一个输入寄存器
    bits<5> rd; // 输出寄存器

    let Inst{31-18} = im14; // 立即数值
    let Inst{17-13} = rs1; // 第一个输入寄存器
    let Inst{12-8}  = rd; // 输出寄存器
    let Inst{3-0} = InstFormatI.Value; // I类型指令的类型字段码
}


class DayInstI<dag outs, dag ins, string asmstr, string argstr, bits<4> opcode> : 
    DayInstIBase<outs, ins, asmstr, argstr> {

    let Inst{7-4} = opcode; // 操作码
}

//////////////////////////////////////////////////////////////////////////////////////
//// 定义L类型指令的格式
//////////////////////////////////////////////////////////////////////////////////////
class DayInstLBase<dag outs, dag ins, string asmstr, string argstr>: 
    DayInst<outs, ins, asmstr, argstr, [], InstFormatL>
{
    bits<14> im14; // 立即数值
    bits<5> rs1; // 第一个输入寄存器
    bits<5> rd; // 输出寄存器

    let Inst{31-18} = im14; // 立即数值
    let Inst{17-13} = rs1; // 第一个输入寄存器
    let Inst{12-8}  = rd; // 输出寄存器
    let Inst{3-0} = InstFormatL.Value; // L类型指令的类型字段码
}


class DayInstL<dag outs, dag ins, string asmstr, string argstr, bits<4> opcode> : 
    DayInstLBase<outs, ins, asmstr, argstr> {
    let Inst{7-4} = opcode; // 操作码
}

//////////////////////////////////////////////////////////////////////////////////////
//// 定义S类型指令的格式
//////////////////////////////////////////////////////////////////////////////////////
class DayInstSBase<dag outs, dag ins, string asmstr, string argstr>: 
    DayInst<outs, ins, asmstr, argstr, [], InstFormatS>
{
    bits<14> im14; // 立即数值
    bits<5> rs1; // 第一个输入寄存器
    bits<5> rs2; // 第二个输入寄存器
    // S类型指令通常没有输出寄存器
    let Inst{31-18} = im14; // 立即数值
    let Inst{17-13} = rs1; // 第一个输入寄存器
    let Inst{12-8}  = rs2; // 第二个输入寄存器
    let Inst{3-0} = InstFormatS.Value; // S类型指令的类型字段码
}

class DayInstS<dag outs, dag ins, string asmstr, string argstr, bits<4> opcode> : 
    DayInstSBase<outs, ins, asmstr, argstr> {
    let Inst{7-4} = opcode; // 操作码
}

//////////////////////////////////////////////////////////////////////////////////////
//// 定义B类型指令的格式
//////////////////////////////////////////////////////////////////////////////////////
class DayInstBBase<dag outs, dag ins, string asmstr, string argstr>: 
    DayInst<outs, ins, asmstr, argstr, [], InstFormatB>
{
    bits<16> im16; // 立即数值
    bits<5> rs1; // 第一个输入寄存器
    bits<5> rs2; // 第二个输入寄存器
    // B类型指令通常没有输出寄存器
    let Inst{31-18} = im16{15-2}; // 立即数值
    let Inst{17-13} = rs1; // 第一个输入寄存器
    let Inst{12-8}  = rs2; // 第二个输入寄存器
    let Inst{3-0} = InstFormatB.Value; // B类型指令的类型字段码
}

class DayInstB<dag outs, dag ins, string asmstr, string argstr, bits<4> opcode> : 
    DayInstBBase<outs, ins, asmstr, argstr> {
    let Inst{7-4} = opcode; // 操作码
    let Constraints = "";  // 关键：无操作数绑定
}

//////////////////////////////////////////////////////////////////////////////////////
//// 定义U类型指令的格式
//////////////////////////////////////////////////////////////////////////////////////
class DayInstUBase<dag outs, dag ins, string asmstr, string argstr>: 
    DayInst<outs, ins, asmstr, argstr, [], InstFormatU>
{
    bits<32> im32; // 立即数值
    bits<5> rd; // 输出寄存器

    let Inst{31} = 0b0;
    let Inst{30-13} = im32{31-14}; // 立即数值
    let Inst{12-8}  = rd; // 输出寄存器
    let Inst{3-0} = InstFormatU.Value; // U类型指令的类型字段码
}

class DayInstU<dag outs, dag ins, string asmstr, string argstr, bits<4> opcode> : 
    DayInstUBase<outs, ins, asmstr, argstr> {
    let Inst{7-4} = opcode; // 操作码
}

//////////////////////////////////////////////////////////////////////////////////////
//// 定义J类型指令的格式
//////////////////////////////////////////////////////////////////////////////////////
class DayInstJBase<dag outs, dag ins, string asmstr, string argstr>: 
    DayInst<outs, ins, asmstr, argstr, [], InstFormatJ>
{
    bits<32> im32; // 立即数值
    bits<5> rd; // 输出寄存器

    let Inst{31-13} = im32{24-6};
    let Inst{12-8}  = rd; // 输出寄存器
    let Inst{7-4} = im32{5-2}; // 立即数值的低4位
    let Inst{3-0} = InstFormatJ.Value; // J类型指令的类型字段码
}

class DayInstJ<dag outs, dag ins, string asmstr, string argstr> : 
    DayInstJBase<outs, ins, asmstr, argstr>;
