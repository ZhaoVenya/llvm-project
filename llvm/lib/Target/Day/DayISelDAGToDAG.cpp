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

