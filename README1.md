2025/05/06 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
>>Start to learn llvm backend
>>>>1.test git
>>>>2.build command
    cmake ../ -GNinja -DLLVM_OPTIMIZED_TABLEGEN=ON -DLLVM_TARGETS_TO_BUILD="X86;Mips" -DLLVM_EXPERIMENTAL_TARGETS_TO_BUILD="Cpu0" -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=ON -DLLVM_ENABLE_PROJECTS=clang
