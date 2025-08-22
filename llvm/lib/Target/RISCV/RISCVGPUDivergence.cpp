#include "RISCVGPUDivergence.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"

using namespace llvm;

bool RISCVGPUDivergence::runOnFunction(Function &F) {
    bool changed = false;

    // 收集所有需要处理的分支指令
    SmallVector<BranchInst *, 8> BranchesToProcess;
    
    for (auto &BB : F) {
        Instruction *Terminator = BB.getTerminator();
        if (BranchInst *BrInst = dyn_cast<BranchInst>(Terminator)) {
            if (BrInst->isConditional()) {
                BranchesToProcess.push_back(BrInst);
            }
        }
    }

    // 处理每个条件分支
    for (auto *BrInst : BranchesToProcess) {
        BasicBlock *ParentBB = BrInst->getParent();
        BasicBlock *TrueDest = BrInst->getSuccessor(0);
        BasicBlock *FalseDest = BrInst->getSuccessor(1);

        // 创建新的基本块
        BasicBlock *NewTrueDest = BasicBlock::Create(F.getContext(), "div_" + TrueDest->getName(), &F, TrueDest);
        BasicBlock *NewFalseDest = BasicBlock::Create(F.getContext(), "div_" + FalseDest->getName(), &F, FalseDest);

        // 更新原始分支指令的目标
        BrInst->setSuccessor(0, NewTrueDest);
        BrInst->setSuccessor(1, NewFalseDest);

        // 在新基本块中添加跳转到原始目标的指令
        IRBuilder<> BuilderTrue(NewTrueDest);
        BuilderTrue.CreateBr(TrueDest);
        
        IRBuilder<> BuilderFalse(NewFalseDest);
        BuilderFalse.CreateBr(FalseDest);

        // 更新PHI节点中的前驱信息
        for (PHINode &PN : TrueDest->phis()) {
            int Index = PN.getBasicBlockIndex(ParentBB);
            if (Index != -1) {
                Value *IncomingValue = PN.getIncomingValue(Index);
                PN.setIncomingBlock(Index, NewTrueDest);
                // 如果需要，可以在NewTrueDest中添加新的PHI节点
            }
        }
        
        for (PHINode &PN : FalseDest->phis()) {
            int Index = PN.getBasicBlockIndex(ParentBB);
            if (Index != -1) {
                Value *IncomingValue = PN.getIncomingValue(Index);
                PN.setIncomingBlock(Index, NewFalseDest);
                // 如果需要，可以在NewFalseDest中添加新的PHI节点
            }
        }

        changed = true;
    }
    
    return changed;
}

char RISCVGPUDivergence::ID = 0;
static RegisterPass<RISCVGPUDivergence> X("riscv-gpu-div", "Splits if/else branches into new empty basic blocks", false, false);

StringRef RISCVGPUDivergence::getPassName() const {
    return "RISCV GPU Divergent Pre-Processing";
}

FunctionPass *createRISCVGPUDivergencePass() {
    return new RISCVGPUDivergence();
}