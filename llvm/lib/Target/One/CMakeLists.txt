add_llvm_component_group(One)

add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)

add_llvm_target(OneCodeGen
        OneTargetMachine.cpp

        LINK_COMPONENTS
        Support
        OneInfo
        OneDesc
)
