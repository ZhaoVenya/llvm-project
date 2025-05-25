//
// Created by 蔡鹏 on 2024/12/1.
//
#include "MCTargetDesc/OneMCTargetDesc.h"
#include "One.h"
#include "OneSubtarget.h"
#include "OneTargetMachine.h"
#include "llvm/CodeGen/SelectionDAGISel.h"

using namespace llvm;

#define DEBUG_TYPE "one-isel"
#define PASS_NAME "One DAG->DAG Pattern Instruction Selection"

class OneDAGToDAGISel : public SelectionDAGISel {
    public:
        OneDAGToDAGISel() = delete;
        explicit OneDAGToDAGISel(OneTargetMachine &TM, CodeGenOptLevel OL)
            : SelectionDAGISel(TM, OL), Subtarget(nullptr) {}
        bool runOnMachineFunction(MachineFunction &MF) override;
        bool SelectAddrFI(SDNode *Parent, SDValue AddrFI, SDValue &BASE, SDValue &Offset);
    private:
        const OneSubtarget *Subtarget;

#include "OneGenDAGISel.inc"

      /// getTargetMachine - Return a reference to the TargetMachine, casted
      /// to the target-specific type.
      const OneTargetMachine &getTargetMachine() {
          return static_cast<const OneTargetMachine &>(TM);
      }

      void Select(SDNode *N) override;
};


// 这是 OneDAGToDAGISel Pass 的入口点。
// 每个 LLVM 后端 Pass 都有一个 runOnMachineFunction 或 runOnFunction 方法，
// 这是 LLVM 编译管理器调用该 Pass 的地方。
bool OneDAGToDAGISel::runOnMachineFunction(MachineFunction &MF) {
    Subtarget = &MF.getSubtarget<OneSubtarget>();
    return SelectionDAGISel::runOnMachineFunction(MF);
}


// 这是模式匹配的核心调度函数。
// SelectionDAGISel::runOnMachineFunction 在遍历 SelectionDAG 时，
// 会为每一个需要被选择的 SDNode 调用这个 Select 方法。
void OneDAGToDAGISel::Select(SDNode *Node) {
    if (Node->isMachineOpcode()) {
        Node->setNodeId(-1);
        return;
    }
    SDLoc DL(Node);

    LLVM_DEBUG(dbgs() << "Selecting: "; Node->dump(CurDAG); dbgs() << '\n');

    SelectCode(Node);
}



// 这是一个辅助函数，专门用于选择涉及帧索引（Frame Index）的寻址模式。
// 它通常在 SelectCode 内部被调用，或者在自定义的模式匹配代码中被使用。
bool OneDAGToDAGISel::SelectAddrFI(SDNode *Parent, SDValue AddrFI, SDValue &Base, SDValue &Offset) {
    /// FrameIndex -> TargetFrameIndex
    if (FrameIndexSDNode *FIN = dyn_cast<FrameIndexSDNode>(AddrFI)) {
        Base = CurDAG->getTargetFrameIndex(FIN->getIndex(), AddrFI.getValueType());
        Offset = CurDAG->getTargetConstant(0, SDLoc(AddrFI), AddrFI.getValueType());
        return true;
    }
    if (CurDAG->isBaseWithConstantOffset(AddrFI)) {
        ConstantSDNode *CN = dyn_cast<ConstantSDNode>(AddrFI.getOperand(1));

        if (FrameIndexSDNode *FS = dyn_cast<FrameIndexSDNode>(AddrFI.getOperand(0))) {
            Base = CurDAG->getTargetFrameIndex(FS->getIndex(), AddrFI.getValueType());
        }else {
            Base = AddrFI.getOperand(0);
        }
        Offset = CurDAG->getTargetConstant(CN->getZExtValue(), SDLoc(AddrFI), AddrFI.getValueType());
        return true;
    }
    return false;
}



class OneDAGToDAGISelLegacy : public SelectionDAGISelLegacy {
  public:
      static char ID;
      // explicit 只能用于构造函数，当只能显式的调用被构造的函数。
      explicit OneDAGToDAGISelLegacy(OneTargetMachine &TM) : SelectionDAGISelLegacy(ID, std::make_unique<OneDAGToDAGISel>(TM, TM.getOptLevel())) {
        
      }
};

char OneDAGToDAGISelLegacy::ID;

// INITIALIZE_PASS 是 LLVM 提供的一个宏，用于简化 Pass 的注册和初始化过程
INITIALIZE_PASS(OneDAGToDAGISelLegacy, DEBUG_TYPE, PASS_NAME, false, false)


//// 
FunctionPass *llvm::createOneISelDag(OneTargetMachine &TM) {
    return new OneDAGToDAGISelLegacy(TM);
}
