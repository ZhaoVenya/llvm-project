add_llvm_component_group(One)

set(LLVM_TARGET_DEFINITIONS One.td)

tablegen(LLVM OneGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM OneGenCallingConv.inc -gen-callingconv)
tablegen(LLVM OneGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM OneGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM OneGenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM OneGenMCPseudoLoweringInfo.inc -gen-pseudo-lowering)

add_public_tablegen_target(OneCommonTableGen)


add_llvm_target(OneCodeGen
        OneTargetMachine.cpp

        LINK_COMPONENTS
        AsmPrinter
        CodeGen
        CodeGenTypes
        Core
        MC
        SlectionDAG
        OneDesc
        OneInfo
        Support
        Target
        TargetParser

        ADD_TO_COMPONENT
        One
)


add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)