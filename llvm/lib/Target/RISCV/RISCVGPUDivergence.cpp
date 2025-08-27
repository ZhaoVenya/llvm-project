#include "RISCVGPUDivergence.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/Transforms/Utils/Cloning.h"

using namespace llvm;



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

    unsigned br_num = 0;
    // 处理每个条件分支
    for (auto *BrInst : BranchesToProcess) {

        IRBuilder<> builder(BrInst);
        Module *M = F.getParent();
        Type *VoidTy = builder.getVoidTy();
        Type *Int1Ty = builder.getInt1Ty();
        Type *Int32Ty = builder.getInt32Ty();
        
        
        Value *Condition = BrInst->getCondition();
        Type *ConditionType = Condition->getType();
        FunctionType *FuncTy1 = FunctionType::get(VoidTy, {Int1Ty}, false);


        Function *riscv_gpu_tmask_emit = Function::Create(   FuncTy1,
                                                            Function::ExternalLinkage,
                                                            "emit_riscv_gpu_tmask",
                                                            M);
        
        FunctionType *FuncTy2 = FunctionType::get(Int32Ty, {}, false);
        Function *riscv_gpu_tmask_get = Function::Create(   FuncTy2,
                                                            Function::ExternalLinkage,
                                                            "get_riscv_gpu_tmask",
                                                            M);

        builder.CreateCall(riscv_gpu_tmask_emit, {Condition});
        Value *tMask = builder.CreateCall(riscv_gpu_tmask_get);


        FunctionType *FuncTy3 = FunctionType::get(Int1Ty, {Int32Ty}, false);
        Function *riscv_gpu_DivOrUni_get = Function::Create(    FuncTy3,
                                                                Function::ExternalLinkage,
                                                                "get_riscv_gpu_DivOrUni",
                                                                M);
        Value *DivOrUni = builder.CreateCall(riscv_gpu_DivOrUni_get, {tMask});
        

        BasicBlock *NewDivDest = BasicBlock::Create(F.getContext(), "div_" + F.getName() + "_" + std::to_string(br_num), &F);
        BasicBlock *NewUniDest = BasicBlock::Create(F.getContext(), "uni_" + F.getName() + "_" + std::to_string(br_num), &F);

        builder.CreateCondBr(DivOrUni, NewDivDest, NewUniDest);
        
        BasicBlock *OriginalTrueDest = BrInst->getSuccessor(0);
        BasicBlock *OriginalFalseDest = BrInst->getSuccessor(1);

        ///////////////////////////////////////////////////////////////////////////////////
        // 这个地方是对divergence分支进行处理，需要有以下几个步骤：
        // 1.复制if.then分支下边的内容(除了最后一行)
        // 2.反转mask
        // 3.复制if.else分支下边的内容(需要保留最后一行)
        IRBuilder<> DivBuilder(NewDivDest);
        // 创建从 NewDivDest 到 ClonedTrue 的分支
        DivBuilder.CreateBr(OriginalFalseDest);

        ///////////////////////////////////////////////////////////////////////////////////

        ///////////////////////////////////////////////////////////////////////////////////
        // 这个地方是对uniform这种纯粹的计算新添加的分支
        IRBuilder<> UniBuilder(NewUniDest);
        FunctionType *FuncTy4 = FunctionType::get(Int1Ty, {Int32Ty}, false);
        Function *riscv_gpu_TorF_get = Function::Create(    FuncTy4,
                                                            Function::ExternalLinkage,
                                                            "get_riscv_gpu_TorF",
                                                            M);
        Value *TorF = UniBuilder.CreateCall(riscv_gpu_TorF_get,{tMask});
        UniBuilder.CreateCondBr(TorF, OriginalTrueDest, OriginalFalseDest);
        ///////////////////////////////////////////////////////////////////////////////////
        
        BrInst->eraseFromParent(); //// 删除原有的跳转分支

        br_num++;




        
        // BasicBlock *ParentBB = BrInst->getParent();         // 分支指令所在的基本块
        // BasicBlock *TrueDest = BrInst->getSuccessor(0);     // 条件为真时的跳转目标
        // BasicBlock *FalseDest = BrInst->getSuccessor(1);    // 条件为假时的跳转目标

        // // 创建新的基本块
        // BasicBlock *NewTrueDest = BasicBlock::Create(F.getContext(), "div_" + TrueDest->getName(), &F, TrueDest);
        // BasicBlock *NewFalseDest = BasicBlock::Create(F.getContext(), "div_" + FalseDest->getName(), &F, FalseDest);

        // // 更新原始分支指令的目标
        // BrInst->setSuccessor(0, NewTrueDest);
        // BrInst->setSuccessor(1, NewFalseDest);

        // // 在新基本块中添加跳转到原始目标的指令
        // IRBuilder<> BuilderTrue(NewTrueDest);
        // BuilderTrue.CreateBr(TrueDest);
        
        // IRBuilder<> BuilderFalse(NewFalseDest);
        // BuilderFalse.CreateBr(FalseDest);

        // // 更新PHI节点中的前驱信息
        // for (PHINode &PN : TrueDest->phis()) {
        //     int Index = PN.getBasicBlockIndex(ParentBB);
        //     if (Index != -1) {
        //         Value *IncomingValue = PN.getIncomingValue(Index);
        //         PN.setIncomingBlock(Index, NewTrueDest);
        //         // 如果需要，可以在NewTrueDest中添加新的PHI节点
        //     }
        // }
        
        // for (PHINode &PN : FalseDest->phis()) {
        //     int Index = PN.getBasicBlockIndex(ParentBB);
        //     if (Index != -1) {
        //         Value *IncomingValue = PN.getIncomingValue(Index);
        //         PN.setIncomingBlock(Index, NewFalseDest);
        //         // 如果需要，可以在NewFalseDest中添加新的PHI节点
        //     }
        // }

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