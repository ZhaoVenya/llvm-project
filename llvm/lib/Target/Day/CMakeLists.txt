
add_llvm_component_group(Day)

set(LLVM_TARGET_DEFINITIONS Day.td)

tablegen(LLVM DayGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM DayGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM DayGenSearchableTables.inc -gen-searchable-tables)
tablegen(LLVM DayGenSubtargetInfo.inc -gen-subtarget)

# tablegen(LLVM DayGenCallingConv.inc -gen-callingconv)



add_public_tablegen_target(DayCommonTableGen)

add_llvm_target(DayCodeGen
  DayTargetMachine.cpp
  DayRegisterInfo.cpp
  DayInstrInfo.cpp
  DayCallingConv.cpp
  DayAsmPrinter.cpp
  DayMachineFunctionInfo.cpp
  DaySubtarget.cpp
  DayTargetObjectFile.cpp
  

  LINK_COMPONENTS
  Support
  CodeGen
  CodeGenTypes
  Core
  MC
  DayDesc
  DayInfo
  Target

  ADD_TO_COMPONENT
  Day
)

add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
