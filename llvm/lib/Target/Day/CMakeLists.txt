
add_llvm_component_group(Day)

set(LLVM_TARGET_DEFINITIONS Day.td)

tablegen(LLVM DayGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM DayGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM DayGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM DayGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM DayGenSearchableTables.inc -gen-searchable-tables)
tablegen(LLVM DayGenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM DayGenCallingConv.inc -gen-callingconv)
tablegen(LLVM DayGenMCPseudoLowering.inc -gen-pseudo-lowering)



add_public_tablegen_target(DayCommonTableGen)

add_llvm_target(DayCodeGen
  DayAsmPrinter.cpp
  DayCallingConv.cpp
  DayFrameLowering.cpp
  DayInstrInfo.cpp
  DayISelLowering.cpp
  DayISelDAGToDAG.cpp
  DayMachineFunctionInfo.cpp
  DayMCInstLowering.cpp
  DayRegisterInfo.cpp
  DaySubtarget.cpp
  DayTargetMachine.cpp
  DayTargetObjectFile.cpp
  

  LINK_COMPONENTS
  AsmPrinter
  CodeGen
  CodeGenTypes
  Core
  MC
  SelectionDAG
  Support
  Target
  TargetParser
  DayDesc
  DayInfo

  ADD_TO_COMPONENT
  Day
)

add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
