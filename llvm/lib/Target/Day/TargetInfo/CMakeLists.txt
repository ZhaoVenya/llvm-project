add_llvm_component_library(LLVMDayInfo
        DayTargetInfo.cpp

        LINK_COMPONENTS
        Support
        MC
)