#include "DayInstrInfo.h"
#include "Day.h"
#include "DayMachineFunctionInfo.h"
#include "DaySubtarget.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineMemOperand.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

#define GET_INSTRINFO_CTOR_DTOR
#include "DayGenInstrInfo.inc"



static cl::opt<unsigned> BPccDisplacementBits(
    "day-bpcc-offset-bits", cl::Hidden, cl::init(19),
    cl::desc("Restrict range of BPcc/FBPfcc instructions (DEBUG)"));

static cl::opt<unsigned>
    BPrDisplacementBits("day-bpr-offset-bits", cl::Hidden, cl::init(16),
                        cl::desc("Restrict range of BPr instructions (DEBUG)"));

// Pin the vtable to this file.
void DayInstrInfo::anchor() {}

DayInstrInfo::DayInstrInfo(DaySubtarget &ST)
    : DayGenInstrInfo(Day::ADJCALLSTACKDOWN, Day::ADJCALLSTACKUP), RI(),
      Subtarget(ST) {}


/*************************************************************
 正常来说,从内存中加载一个32bit数据, 格式为:lw rd, rs(imm14)
 也就是说:从地址(rs+imm14)加载一个数据到rd中.
 如果(rs+imm14)属于栈内存的一部分,那么就是从栈中加载一个数据.
 如果此时rs中的地址是栈内存的一部分,imm14恰好是0,那么就是从栈槽中加载数据
 isLoadFromStackSlot函数就是判断这个加载指令是否为"从栈槽中加载数据".
 
 根据加载数据的定义:
 class ALU_L<string asmstr, bits<4> opcode, PatFrag OpNode> : 
        DayInstL<(outs GPR:$rd), (ins GPR:$rs1, simm14:$im14), 
        asmstr, "$rd, $rs1($im14)", 
        opcode>{
    let Pattern = [(set GPR:$rd, (OpNode (M_LOAD (add GPR:$rs1, simm14:$im14))))];
}
 那么rd是第0个操作数,rs是第1个操作数据,imm14是第2个操作数据.
 在上述指令格式中,只有rs才有isFI的属性.
 返回值 Register: 表示栈槽中的数据存储到了哪个寄存器中.
*************************************************************/
Register DayInstrInfo::isLoadFromStackSlot(const MachineInstr &MI,
                                             int &FrameIndex) const {
  
  if (MI.getOpcode() == Day::LB || MI.getOpcode() == Day::LBU ||
      MI.getOpcode() == Day::LH || MI.getOpcode() == Day::LHU ||
      MI.getOpcode() == Day::LW || MI.getOpcode() == Day::LWU) {

    if (MI.getOperand(1).isFI() && MI.getOperand(2).isImm() &&
        MI.getOperand(2).getImm() == 0) {
      FrameIndex = MI.getOperand(1).getIndex();
      return MI.getOperand(0).getReg();
    }
  }

  return 0;
}


/*************************************************************
 正常来说,往内存中存储一个32bit数据, 格式为:sw rs1, rs2(imm14)
 也就是说:往地址(rs+imm14)存储一个数据,数据来源是rs1
 如果(rs2+imm14)属于栈内存的一部分,那么就是往栈中存储一个数据.
 如果此时rs2中的地址是栈内存的一部分,imm14恰好是0,那么就是从栈槽中加载数据
 isStoreFromStackSlot函数就是判断这个加载指令是否为"从栈槽中加载数据".
 
 根据加载数据的定义:
 class ALU_S<string asmstr, bits<4> opcode, PatFrag OpNode> : 
        DayInstL<(outs), (ins GPR:$rs1, GPR:$rs2, simm14:$im14), 
        asmstr, "$rs1, $rs2($im14)", 
        opcode>{
    ...
}
 那么rs1是第0个操作数,rs2是第1个操作数据,imm14是第2个操作数据.
 在上述指令格式中,只有rs2才有isFI的属性.
 返回值 Register: 这个返回值表示哪个寄存器的数据被存储到了栈槽中
*************************************************************/
Register DayInstrInfo::isStoreToStackSlot(const MachineInstr &MI,
                                            int &FrameIndex) const {
  if (MI.getOpcode() == Day::SB || MI.getOpcode() == Day::SH || MI.getOpcode() == Day::SW) {
    if (MI.getOperand(1).isFI() && MI.getOperand(2).isImm() &&
        MI.getOperand(2).getImm() == 0) {
      FrameIndex = MI.getOperand(1).getIndex();
      return MI.getOperand(0).getReg();
    }
  }
  return 0;
}


void DayInstrInfo::storeRegToStackSlot(MachineBasicBlock &MBB,
                                         MachineBasicBlock::iterator I,
                                         Register SrcReg, bool isKill, int FI,
                                         const TargetRegisterClass *RC,
                                         const TargetRegisterInfo *TRI,
                                         Register VReg,
                                         MachineInstr::MIFlag Flags) const {
                                      
     
    MachineFunction *MF = MBB.getParent();
    MachineFrameInfo &MFI = MF->getFrameInfo();

    unsigned Opcode;
    bool IsScalableVector = true;
    unsigned Size = MFI.getObjectSize(FI);

    if (Day::GPRRegClass.hasSubClassEq(RC)) {
      
      switch(Size){
        case 1: 
          Opcode = Day::SB;
        break;

        case 2:
          Opcode = Day::SH;
        break;

        case 4:
          Opcode = Day::SW;
        break;

        default:
          llvm_unreachable("Unsupported GPR store size to stack slot");
        break;
      }

      IsScalableVector = false;
    }

    MachineMemOperand *MMO = MF->getMachineMemOperand(
    MachinePointerInfo::getFixedStack(*MF, FI), MachineMemOperand::MOStore,
    MFI.getObjectSize(FI), MFI.getObjectAlign(FI));

    BuildMI(MBB, I, DebugLoc(), get(Opcode))
        .addReg(SrcReg, getKillRegState(isKill))
        .addFrameIndex(FI)
        .addImm(0)
        .addMemOperand(MMO)
        .setMIFlag(Flags);
    
}

void DayInstrInfo::loadRegFromStackSlot(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator I, Register DestReg,
    int FI, const TargetRegisterClass *RC, const TargetRegisterInfo *TRI,
    Register VReg, MachineInstr::MIFlag Flags) const {
    
    DebugLoc DL = Flags & MachineInstr::FrameDestroy ? MBB.findDebugLoc(I) : DebugLoc();

    MachineFunction *MF = MBB.getParent();
    MachineFrameInfo &MFI = MF->getFrameInfo();

    unsigned Opcode;
    bool IsScalableVector = true;
    unsigned Size = MFI.getObjectSize(FI);

    if (Day::GPRRegClass.hasSubClassEq(RC)) {
      switch(Size){
        case 1: 
          Opcode = Day::LB;
        break;

        case 2:
          Opcode = Day::LH;
        break;

        case 4:
          Opcode = Day::LW;
        break;

        default:
          llvm_unreachable("Unsupported GPR load size from stack slot");
        break;
      }
      IsScalableVector = false;
    }

    MachineMemOperand *MMO = MF->getMachineMemOperand(
    MachinePointerInfo::getFixedStack(*MF, FI), MachineMemOperand::MOLoad,
    MFI.getObjectSize(FI), MFI.getObjectAlign(FI));

    BuildMI(MBB, I, DL, get(Opcode), DestReg)
        .addFrameIndex(FI)
        .addImm(0)
        .addMemOperand(MMO)
        .setMIFlag(Flags);
}


void DayInstrInfo::copyPhysReg(MachineBasicBlock &MBB,
                                 MachineBasicBlock::iterator I,
                                 const DebugLoc &DL, MCRegister DestReg,
                                 MCRegister SrcReg, bool KillSrc,
                                 bool RenamableDest, bool RenamableSrc) const {

}


Register DayInstrInfo::getGlobalBaseReg(MachineFunction *MF) const {
  return;
}

unsigned DayInstrInfo::getInstSizeInBytes(const MachineInstr &MI) const {
 return 0;
}

bool DayInstrInfo::expandPostRAPseudo(MachineInstr &MI) const {
    
  return false;
}



