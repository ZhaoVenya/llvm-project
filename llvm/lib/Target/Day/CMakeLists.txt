
add_llvm_component_group(Day)

add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)

add_llvm_target(DayCodeGen
  DayTargetMachine.cpp
  
  LINK_COMPONENTS
  Support
)