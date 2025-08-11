#include "DaySubtarget.h"

#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCSchedule.h"
#include "llvm/CodeGen/TargetSchedule.h"


using namespace llvm;

#define DEBUG_TYPE "day-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "DayGenSubtargetInfo.inc"


void DaySubtarget::anchor() { }


DaySubtarget &DaySubtarget::initializeSubtargetDependencies(
    StringRef CPU, StringRef TuneCPU, StringRef FS) {
  
  if (CPU.empty() || CPU == "generic")
    CPU = Is64Bit ? "generic" : "generic";

  if (TuneCPU.empty())
    TuneCPU = CPU;

  // Parse features string.
  ParseSubtargetFeatures(CPU, TuneCPU, FS);

  return *this;
}



DaySubtarget::DaySubtarget(const StringRef &CPU, const StringRef &TuneCPU,
                               const StringRef &FS, const TargetMachine &TM,
                               bool is64Bit)
    : DayGenSubtargetInfo(TM.getTargetTriple(), CPU, TuneCPU, FS),
      ReserveRegister(TM.getMCRegisterInfo()->getNumRegs()),
      TargetTriple(TM.getTargetTriple()), Is64Bit(is64Bit),
      InstrInfo(initializeSubtargetDependencies(CPU, TuneCPU, FS)),
      FrameLowering(*this),
      TLInfo(TM, *this),
      RegInfo(0,*this){

}

DaySubtarget::~DaySubtarget() = default;

