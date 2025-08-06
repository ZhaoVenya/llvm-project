add_llvm_component_library(LLVMDayDesc
        DayMCTargetDesc.cpp
        DayInstPrinter.cpp
        DayMCAsmInfo.cpp

        LINK_COMPONENTS
        Support
        MC
        TargetParser
        DayInfo

        ADD_TO_COMPONENT
        Day
)