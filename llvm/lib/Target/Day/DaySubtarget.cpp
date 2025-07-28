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
  // Determine default and user specified characteristics
  std::string CPUName = std::string(CPU);

  std::string TuneCPUName = std::string(TuneCPU);

  // Parse features string.
  ParseSubtargetFeatures(CPUName, TuneCPUName, FS);

  return *this;
}



DaySubtarget::DaySubtarget(const StringRef &CPU, const StringRef &TuneCPU,
                               const StringRef &FS, const TargetMachine &TM,
                               bool is64Bit)
    : DayGenSubtargetInfo(TM.getTargetTriple(), CPU, TuneCPU, FS),
      ReserveRegister(TM.getMCRegisterInfo()->getNumRegs()),
      TargetTriple(TM.getTargetTriple()), Is64Bit(is64Bit),
      InstrInfo(initializeSubtargetDependencies(CPU, TuneCPU, FS)),
      TLInfo(TM, *this), FrameLowering(*this) {

}


