#include "RISCVGPUDivergence.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopPass.h"

using namespace llvm;


bool RISCVGPUDivergence::isLoopBr(BranchInst *BI, LoopInfo &LI){
    return true;
}


bool RISCVGPUDivergence::runOnFunction(Function &F) {
    bool changed = false;

    // 首先获取循环信息
    LoopInfo &LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();

    // 收集所有需要处理的分支指令
    SmallVector<BranchInst *, 8> BranchesToProcess;
    
    for (auto &BB : F) {
        Instruction *Terminator = BB.getTerminator();
        BranchInst *BrInst = dyn_cast<BranchInst>(Terminator);

        if (BrInst!=nullptr && BrInst->isConditional()) {
            
            if (LI.getLoopFor(&BB) != nullptr) {
                // 这个基本块属于一个循环，跳过处理
                continue;
            }
            
            // 进一步检查：确保这不是一个循环的回边
            BasicBlock *TrueDest = BrInst->getSuccessor(0);
            BasicBlock *FalseDest = BrInst->getSuccessor(1);
            
            // 如果分支的目标是循环头，则很可能是循环控制分支
            if (LI.isLoopHeader(TrueDest) || LI.isLoopHeader(FalseDest)) {
                continue;
            }
            
            BranchesToProcess.push_back(BrInst);
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


void RISCVGPUDivergence::getAnalysisUsage(AnalysisUsage &AU)const{
    AU.addRequired<LoopInfoWrapperPass>();
    AU.setPreservesAll();
}

char RISCVGPUDivergence::ID = 0;
static RegisterPass<RISCVGPUDivergence> X("riscv-gpu-div", "Splits if/else branches into new empty basic blocks", false, false);

StringRef RISCVGPUDivergence::getPassName() const {
    return "RISCV GPU Divergent Pre-Processing";
}

FunctionPass *createRISCVGPUDivergencePass() {
    return new RISCVGPUDivergence();
}