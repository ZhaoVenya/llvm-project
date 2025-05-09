add_llvm_component_library(LLVMOneDesc
        OneMCTargetDesc.cpp
        OneMCAsmInfo.cpp

        LINK_COMPONENTS
        CodeGenTypes
        MC
        Support
        CodeGen

        

)
