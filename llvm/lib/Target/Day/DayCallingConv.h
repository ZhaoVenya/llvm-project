#ifndef DAY_CALLING_CONV_H_
#define DAY_CALLING_CONV_H_

// #include "MCTargetDesc/"
#include "llvm/CodeGen/CallingConvLower.h"

namespace llvm {

/*

 ValNo: 值编号。它是一个从 0 开始递增的整数，用于标识当前正在处理的是第几个参数或返回值。
 ValTV: 值虚拟类型 (Value Virtual Type)。代表参数或返回值的原始 LLVM 类型，例如 MVT::i32 (32位整数) 或 MVT::f64 (64位浮点数)。
 LocVT: 分配位置的虚拟类型,这是编译器实际用于分配的类型。
 LocInfo: 分配位置信息。
 ArgFlags: 参数标志。
 CCState: 调用约定状态。这是一个引用参数，它包含了整个函数调用约定的当前状态。函数使用它来分配寄存器和栈空间，并记录分配结果。
 IsFixed: 函数入口参数是否为固定的.
 IsRet: 是不是返回数值
 OrigTy: 原始类型,
*/
bool CC_Day(unsigned ValNo, MVT ValVT, MVT LocVT,
              CCValAssign::LocInfo LocInfo, ISD::ArgFlagsTy ArgFlags,
              CCState &State, bool IsFixed, bool IsRet, Type *OrigTy);


} // end namespace llvm



#endif
