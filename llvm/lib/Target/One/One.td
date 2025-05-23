include "llvm/Target/Target.td"

include "OneRegisterInfo.td"
include "OneInstrInfo.td"
include "OneCallingConv.td"

//// 这个One.td文件是整个One架构的一个集合，包括：
//// OneRegisterInfo.td 寄存器信息描述
//// OneInstrInfo.td 指令信息描述
//// OneCallingConv.td 调用信息描述


// 它定义了一个名为 "one" 的处理器模型，
// 并明确指出这个处理器没有复杂的指令调度模型和调度资源。
// 这对于一个简单或新的目标机器来说是很常见的初始设置，
// 意味着编译过程将不会对指令的执行顺序进行深入的微架构优化。
def : ProcessorModel<"one", NoSchedModel, []>;


// 它定义了一个名为 OneInstrInfo 的对象，这个对象将作为**“One”目标机器指令集信息**的容器。
// 它继承自 InstrInfo 基类，这意味着它将包含所有通用指令信息。
// 后续的 .td 文件中，具体的“One”指令（如 ADD, SUB, LOAD, STORE 等）将通过继承 OneInstrInfo 或与它关联来定义它们的行为。
// 这个定义最终会生成 C++ 代码中的 OneGenInstrInfo.inc 或类似的头文件。
def OneInstrInfo : InstrInfo;

// 定义了一个名为 One 的编译目标，并将 OneInstrInfo 对象指定为其指令集。
// 这是将处理器模型、指令信息等各种描述关联到一个完整的编译目标上的关键一步。
def One : Target {
    let InstructionSet = OneInstrInfo;
}

