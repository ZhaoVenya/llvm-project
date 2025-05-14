2025/05/06 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
>>Start to learn llvm backend
>>>>1.test git
>>>>2.build project command
>>>>>>>> cmake -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DCMAKE_BUILD_TYPE=Debug -DLLVM_OPTIMIZED_TABLEGEN=ON -DLLVM_TARGETS_TO_BUILD="X86;Mips" -DLLVM_EXPERIMENTAL_TARGETS_TO_BUILD="Cpu0" -DBUILD_SHARED_LIBS=ON -DLLVM_ENABLE_PROJECTS=clang -G "Ninja" ../llvm

2025/05/06 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
>>>>3.The First ASM Command

>>>>>>>> 这个是编译llvm的指令，DLLVM_ENABLE_PROJECTS要包含clang和lld
>>>>>>>> cmake -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DCMAKE_BUILD_TYPE=Debug -DLLVM_OPTIMIZED_TABLEGEN=ON -DLLVM_TARGETS_TO_BUILD="X86;Mips;RISCV" -DLLVM_EXPERIMENTAL_TARGETS_TO_BUILD="One" -DCMAKE_LINKER=ld.lld -DBUILD_SHARED_LIBS=ON -DLLVM_ENABLE_PROJECTS="clang;lld" -G "Ninja" ../llvm

>>>>>>>> 这个是生成riscv32汇编文件的指令(在one_chip_software_test目录下运行)
>>>>>>>>../build/bin/llc -march=riscv32 build/obj/main.ll -o main.asm
>>>>>>>> .vscode folder has a launch.json, the code here is:
    {
        "version": "0.2.0",
        "configurations": [
            {
                "name": "Debug llc with RISC-V",
                "type": "cppdbg",
                "request": "launch",
                "program": "${workspaceFolder}/build/bin/llc",
                "args": [
                    "-march=one",
                    "-O0",
                    "-filetype=asm",
                    "${workspaceFolder}/one_chip_software_test/build/obj/main.ll",
                    "-o",
                    "-",
                    "--debug"
                ],
                "stopAtEntry": false,
                "cwd": "${workspaceFolder}",
                "environment": [],
                "externalConsole": false,
                "MIMode": "gdb",
                "setupCommands": [
                    {
                        "description": "Enable pretty-printing for gdb",
                        "text": "-enable-pretty-printing",
                        "ignoreFailures": true
                    }
                ]
            }
        ]
    }

>>>>>>>> 调试时使用main1.ll的代码，main1.ll的IR代码如下：
    ; ModuleID = 'test.c'
    source_filename = "test.c"
    target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128"
    target triple = "riscv32-apple-macosx14.0.0"

    ; Function Attrs: noinline nounwind optnone ssp uwtable(sync)
    define i32 @main() #0 {
    ret i32 42
    }

>>>>>>>> 在llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp的4389行添加了下边的代码，
>>>>>>>> 全局搜索“must be return false, modified by zhaowenya”查看修改了什么：
    if (!&TSI) {
      //// must be return false, modified by zhaowenya
      return false;
    }

