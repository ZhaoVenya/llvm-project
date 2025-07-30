#include "DayRegisterInfo.h"
#include "Day.h"
#include "DaySubtarget.h"

#include "MCTargetDesc/DayMCTargetDesc.h"

#include "llvm/CodeGen/MachineFrameInfo.h"


using namespace llvm;

#define GET_REGINFO_TARGET_DESC
#include "DayGenRegisterInfo.inc"

#define DEBUG_TYPE "Day Register Info"


static cl::opt<bool>
ReserveAppRegisters("day-reserve-app-registers", cl::Hidden, cl::init(false),
                    cl::desc("Reserve application registers (%g2-%g4)"));


// DayGenRegisterInfo的入口参数是RA,
DayRegisterInfo::DayRegisterInfo() : DayGenRegisterInfo(Day::RA) {}

const MCPhysReg*
DayRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  // 返回的这个东西,它来自于inc文件,关于DayCSRs是被定义在CallingConv.td文件中
  return DayCSRs_SaveList;
}

const uint32_t *
DayRegisterInfo::getCallPreservedMask(const MachineFunction &MF,
                                        CallingConv::ID CC) const {
  // 返回DayCSRs的一个掩码,函数调用之后,用于表示某些DayCSRs的数值是保证不变的.
  return DayCSRs_RegMask;
}




BitVector DayRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());

  const DayFrameLowering *TFI = getFrameLowering(MF);
  
  Reserved.set(Day::ZERO);
  Reserved.set(Day::RA);
  Reserved.set(Day::SP);
  Reserved.set(Day::GP);
  Reserved.set(Day::TP);

  if(TFI->hasFP(MF)){
    Reserved.set(Day::S0);
  }

  return Reserved;
}


// 这个函数用于查询单个物理寄存器是否被保留
bool DayRegisterInfo::isReservedReg(const MachineFunction &MF,
                                      MCRegister Reg) const {
  return getReservedRegs(MF)[Reg];
}


/************************************************************************************************
 eliminateFrameIndex并不是消除的栈帧, 
 而是消除的那些能够转换为"以栈指针或帧指针为基地址的访存操作"的指令.
 简单来说：
 消除前： 机器指令可能长这样：LOAD R1, [FI0] (从栈帧索引0处加载数据到R1)。
 消除后： 机器指令被修改成这样：LOAD R1, [SP + 12] 或 LOAD R1, [FP - 8] (从SP/FP加减某个具体偏移量处加载数据到R1)。
 eliminateFrameIndex 是一个专门针对栈帧索引的转换过程。
 只有那些在编译早期被标记为访问栈帧上具体位置的 load 或 store 指令（其操作数是 FrameIndex 类型）才会经过这个处理。
 其他访问全局、堆或其他非栈内存的 load/store 指令，则有其自己的地址计算和编码方式，与 eliminateFrameIndex 无关。
************************************************************************************************/
bool DayRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                       int SPAdj, unsigned FIOperandNum,
                                       RegScavenger *RS) const {
  assert(SPAdj == 0 && "Unexpected");
  MachineInstr &MI = *II;
  // LLVM_DEBUG(errs() << MI);

  /// MI (Reg, FrameIndex, IMM)

  uint I = 0;
  while (!MI.getOperand(I).isFI()) {
    ++I;
    assert(I < MI.getNumOperands());
  }

  const int FI = MI.getOperand(I).getIndex();

  /// 根据 index -> 函数栈帧内部的偏移量
  const MachineFunction &MF = *MI.getParent()->getParent();
  const MachineFrameInfo &MFI = MF.getFrameInfo();
  int64_t Offset = MFI.getObjectOffset(FI);
  uint64_t STACKSIZE =
      ROUND_UP(MFI.getStackSize(), 16);
  Offset += static_cast<int64_t>(STACKSIZE);

  int64_t O = MI.getOperand(I + 1).getImm();
  Offset += O;
  MI.getOperand(I).ChangeToRegister(Day::SP, false);
  MI.getOperand(I + 1).ChangeToImmediate(Offset);
  // MI.getOperand(I + 1).setImm(Offset);
  return true;
}


// getFrameRegister用于获取Day架构的帧指针寄存器
Register DayRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  return Day::S0;
}






