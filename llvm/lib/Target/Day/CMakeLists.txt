
add_llvm_component_group(Day)

set(LLVM_TARGET_DEFINITIONS Day.td)
tablegen(LLVM DayGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM DayGenInstrInfo.inc -gen-instr-info)



add_public_tablegen_target(DayCommonTableGen)

add_llvm_target(DayCodeGen
  DayTargetMachine.cpp
  DayRegisterInfo.cpp
  
  LINK_COMPONENTS
  Support

  ADD_TO_COMPONENT
  Day
)

add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)