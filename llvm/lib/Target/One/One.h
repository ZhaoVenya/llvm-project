//
// Created by 蔡鹏 on 2024/12/1.
//

#ifndef ONE_H
#define ONE_H

#define DIV_ROUND_UP(n, d)  (((n) + (d) - 1) / (d))
#define ROUND_UP(x, align)  (DIV_ROUND_UP(x, align) * (align))


namespace llvm {

    class FunctionPass;
    class OneTargetMachine;
    class PassRegistry;

    // 这个函数的作用是创建一个特定于 OneTargetMachine 实例的指令选择Pass。
    // 当编译器需要将LLVM IR转换为机器指令时，它会调用这样的函数来获取一个执行指令选择任务的Pass。
    // 这个Pass会以函数为单位进行处理，将函数的IR转换为目标机器的指令。
    FunctionPass *createOneISelDag(OneTargetMachine &TM);

    // 这个函数的作用是在LLVM的全局Pass注册表中注册 OneDAGToDAGISel Pass。
    // 这样做是为了让LLVM的旧式Pass管理器能够识别、加载并使用这个指令选择Pass。
    // 通常，这样的初始化函数会在程序启动时被调用，以确保所有必要的Pass都被正确地注册。
    void initializeOneDAGToDAGISelLegacyPass(PassRegistry &);

} // namespace llvm
#endif // ONE_H
