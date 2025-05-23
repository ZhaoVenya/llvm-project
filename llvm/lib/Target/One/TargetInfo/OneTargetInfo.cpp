//
// Created by 蔡鹏 on 2024/11/26.
//

#include "OneTargetInfo.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

/*********************
getTheOneTarget这个函数中有一个static的Target类型的对象TheOneTarget,
TheOneTarget 前边是static修饰的，代表着这个对象贯穿了整个程序的生命周期。
由于函数返回的是一个引用，代表这个返回的数值还是那个TheOneTarget对象。
就能保证TheOneTarget只在初始化的时候被创建一次，且只有一个TheOneTarget。
*********************/
Target &llvm::getTheOneTarget() {
  static Target TheOneTarget;
  return TheOneTarget;
}

// extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeOneTargetInfo()
// 这个函数在main函数执行前就被执行了，也就初始化了TheOneTarget这个对象
extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeOneTargetInfo() {
  /*
  这个地方是重点：
  1.首先我们知道，getTheOneTarget()返回的 &TheOneTarget是唯一的。
  2.RegisterTarget<>X()是一个模板结构体，X()里的内容要匹配构造函数 RegisterTarget
  3.C++中strcut也是能写函数的，在 C++ 中，struct 和 class 在功能上几乎是完全等价的
  4.struct 的成员（包括数据成员和函数成员）默认是 public 的， 继承方式是public继承
  5.class 的成员默认是 private 的，class 默认的继承方式是 private 继承

  追踪这个函数的底层，就可以发现，最终目的是为TheOneTarget这个唯一的对象设置一些初始值。
  这些初始值包括Name, Desc, BackendName.

  */
  RegisterTarget<Triple::one, /*HasJIT=*/false> X(
      getTheOneTarget(), "one", "ONE (32 little endian)", "One");
}