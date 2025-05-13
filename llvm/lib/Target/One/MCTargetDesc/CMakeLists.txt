
add_llvm_component_library(LLVMOneDesc
        OneMCTargetDesc.cpp
        OneMCAsmInfo.cpp

        LINK_COMPONENTS
        CodeGenTypes
        MC
        Support
        CodeGen


        # 添加 LLVMOneInfo 依赖
        LINK_LIBS
        LLVMOneInfo

        ADD_TO_COMPONENT
        One

        

)
