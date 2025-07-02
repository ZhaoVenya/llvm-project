add_llvm_component_library(LLVMDayDesc
        DayMCTargetDesc.cpp

        LINK_COMPONENTS
        Support
        CodeGenTypes
        MC
)