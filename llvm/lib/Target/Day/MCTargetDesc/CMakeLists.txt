
add_llvm_component_library(LLVMOneDesc
        DayMCTargetDesc.cpp
        DayMCAsmInfo.cpp
        DayInstPrinter.cpp

        LINK_COMPONENTS
        CodeGenTypes
        MC
        Support
        CodeGen


        # 添加 LLVMOneInfo 依赖
        LINK_LIBS
        LLVMOneInfo

        ADD_TO_COMPONENT
        Day

        

)
