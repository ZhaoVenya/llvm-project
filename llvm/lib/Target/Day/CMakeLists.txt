add_llvm_component_group(Day)

set(LLVM_TARGET_DEFINITIONS Day.td)

# tablegen(LLVM OneGenAsmWriter.inc -gen-asm-writer)
# tablegen(LLVM OneGenDAGISel.inc -gen-dag-isel)
# tablegen(LLVM OneGenCallingConv.inc -gen-callingconv)
# tablegen(LLVM OneGenInstrInfo.inc -gen-instr-info)
# tablegen(LLVM OneGenRegisterInfo.inc -gen-register-info)
# tablegen(LLVM OneGenSubtargetInfo.inc -gen-subtarget)
# tablegen(LLVM OneGenMCPseudoLoweringInfo.inc -gen-pseudo-lowering)

# add_public_tablegen_target(DayCommonTableGen)


add_llvm_target(DayCodeGen
        DayTargetMachine.cpp
        DayInstrInfo.cpp
        DayRegisterInfo.cpp
        DaySubtarget.cpp
        DayFrameLowering.cpp
        DayISelLowering.cpp
        DayISelDAGToDAG.cpp
        DayAsmPrinter.cpp
        

        LINK_COMPONENTS
        AsmPrinter
        CodeGen
        CodeGenTypes
        Core
        MC
        SelectionDAG
        DayDesc
        DayInfo
        Support
        Target
        TargetParser

        ADD_TO_COMPONENT
        Day
)


add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)