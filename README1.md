2025/05/06 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
>>Start to learn llvm backend
>>>>1.test git
>>>>2.build project command
>>>>>>>> cmake -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DCMAKE_BUILD_TYPE=Debug -DLLVM_OPTIMIZED_TABLEGEN=ON -DLLVM_TARGETS_TO_BUILD="X86;Mips" -DLLVM_EXPERIMENTAL_TARGETS_TO_BUILD="Cpu0" -DBUILD_SHARED_LIBS=ON -DLLVM_ENABLE_PROJECTS=clang -G "Ninja" ../llvm

2025/05/06 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
>>>>3.The First ASM Command

>>>>>>>> 这个是编译llvm的指令，DLLVM_ENABLE_PROJECTS要包含clang和lld
>>>>>>>> cmake -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DCMAKE_BUILD_TYPE=Debug -DLLVM_OPTIMIZED_TABLEGEN=ON -DLLVM_TARGETS_TO_BUILD="X86;Mips;RISCV" -DLLVM_EXPERIMENTAL_TARGETS_TO_BUILD="One" -DBUILD_SHARED_LIBS=ON -DLLVM_ENABLE_PROJECTS="clang;lld" -G "Ninja" ../llvm

>>>>>>>> 这个是生成riscv32汇编文件的指令(在one_chip_software_test目录下运行)
>>>>>>>>../build/bin/llc -march=riscv32 build/obj/main.ll -o main.asm

