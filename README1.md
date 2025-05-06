2025/05/06 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
>>Start to learn llvm backend
>>>>1.test git
>>>>2.build project command
>>>>>>>> cmake -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DCMAKE_BUILD_TYPE=Debug -DLLVM_OPTIMIZED_TABLEGEN=ON -DLLVM_TARGETS_TO_BUILD="X86;Mips" -DLLVM_EXPERIMENTAL_TARGETS_TO_BUILD="Cpu0" -DBUILD_SHARED_LIBS=ON -DLLVM_ENABLE_PROJECTS=clang -G "Ninja" ../llvm



