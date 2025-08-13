#include "DayISelDAGToDAG.h"

using namespace llvm;

#define DEBUG_TYPE "day-isel"
#define PASS_NAME "Day DAG->DAG Pattern Instruction Selection"

char DayDAGToDAGISelLegacy::ID = 0;

void DayDAGToDAGISel::Select(SDNode *Node) { SelectCode(Node); }

FunctionPass *llvm::createDayISelDag(DayTargetMachine &TM,
                                        CodeGenOptLevel OptLevel) {
  return new DayDAGToDAGISelLegacy(TM, OptLevel);
}


bool DayDAGToDAGISel::SelectAddrFrameIndex(SDValue Addr, SDValue &Base,
                                             SDValue &Offset) {
  if (auto *FIN = dyn_cast<FrameIndexSDNode>(Addr)) {
    Base = CurDAG->getTargetFrameIndex(FIN->getIndex(), Subtarget->getXLenVT());
    Offset = CurDAG->getTargetConstant(0, SDLoc(Addr), Subtarget->getXLenVT());
    return true;
  }

  return false;
}


bool DayDAGToDAGISel::SelectAddrRegImm(SDValue Addr, SDValue &Base,
                                         SDValue &Offset) {
  
  if (SelectAddrFrameIndex(Addr, Base, Offset))
    return true;
  
  return false;
}

