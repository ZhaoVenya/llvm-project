#include "RISCVGPUDivergence.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/Transforms/Utils/Cloning.h"
#include <queue>

using namespace llvm;


BasicBlock* findCommonDestination(BasicBlock *TrueDest, BasicBlock *FalseDest) {
    // 使用深度优先搜索或广度优先搜索来查找共同目标块
    SmallPtrSet<BasicBlock*, 8> TrueVisited;
    SmallPtrSet<BasicBlock*, 8> FalseVisited;
    
    // 从 TrueDest 开始搜索
    std::queue<BasicBlock*> TrueQueue;
    TrueQueue.push(TrueDest);
    while (!TrueQueue.empty()) {
        BasicBlock *BB = TrueQueue.front();
        TrueQueue.pop();
        
        if (TrueVisited.insert(BB).second) {
            for (BasicBlock *Succ : successors(BB)) {
                TrueQueue.push(Succ);
            }
        }
    }
    
    // 从 FalseDest 开始搜索
    std::queue<BasicBlock*> FalseQueue;
    FalseQueue.push(FalseDest);
    while (!FalseQueue.empty()) {
        BasicBlock *BB = FalseQueue.front();
        FalseQueue.pop();
        
        if (FalseVisited.insert(BB).second) {
            // 检查是否在 TrueVisited 中
            if (TrueVisited.count(BB)) {
                return BB; // 找到共同目标块
            }
            
            for (BasicBlock *Succ : successors(BB)) {
                FalseQueue.push(Succ);
            }
        }
    }
    
    return nullptr; // 没有找到共同目标块
}


Value* getValueBeforeTerminator(BasicBlock *BB) {
    // 获取基本块的终止指令
    Instruction *Terminator = BB->getTerminator();
    
    // 如果终止指令前面有指令，返回最后一条指令
    if (Terminator->getPrevNode()) {
        return Terminator->getPrevNode();
    }
    
    return nullptr; // 基本块为空或只有终止指令
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

    SmallVector<BasicBlock *, 8> DivDestList;
    SmallVector<BasicBlock *, 8> OrgDestList1st;
    SmallVector<BasicBlock *, 8> OrgDestList2nd;
    SmallVector<BranchInst *, 8> eraseBrList;
    Module *M = F.getParent();

    // 处理每个条件分支
    for (auto *BrInst : BranchesToProcess) {

        IRBuilder<> builder(BrInst);
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
        
        ///////////////////////////////////////////////////////////////////////////////////
        // 创建两个新的basic block
        BasicBlock *NewDivDest = BasicBlock::Create(F.getContext(), "div_" + F.getName() + "_" + std::to_string(br_num), &F);
        BasicBlock *NewUniDest = BasicBlock::Create(F.getContext(), "uni_" + F.getName() + "_" + std::to_string(br_num), &F);

        builder.CreateCondBr(DivOrUni, NewDivDest, NewUniDest); // 创建一个新的跳转节点
        
        BasicBlock *OriginalTrueDest = BrInst->getSuccessor(0);
        BasicBlock *OriginalFalseDest = BrInst->getSuccessor(1);

        ///////////////////////////////////////////////////////////////////////////////////
        // 这个地方是对divergence分支进行处理，需要有以下几个步骤：
        // 1.复制if.then分支下边的内容(除了最后一行)
        // 2.反转mask
        // 3.复制if.else分支下边的内容(需要保留最后一行)
        // 创建从 NewDivDest 到 ClonedTrue 的分支
        // DivBuilder.CreateBr(OriginalFalseDest);
        DivDestList.push_back(NewDivDest);
        OrgDestList1st.push_back(OriginalTrueDest);
        OrgDestList2nd.push_back(OriginalFalseDest);

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
        // BrInst->eraseFromParent(); //// 删除原有的跳转分支
        eraseBrList.push_back(BrInst);
        br_num++;
        
    }

    for(auto *DivBB : DivDestList){
        IRBuilder<> DivBuilder(DivBB);
        ValueToValueMapTy VMap;
        
        BasicBlock *OrgBranch1st = OrgDestList1st.back();
        OrgDestList1st.pop_back();
        BasicBlock *OrgBranch2nd = OrgDestList2nd.back();
        OrgDestList2nd.pop_back();

        BasicBlock *BrTermBB = findCommonDestination(OrgBranch1st, OrgBranch2nd);

        for (Instruction &OrigInst : *OrgBranch1st) {
            if(OrigInst.isTerminator()){
                break;
            }
            Instruction *ClonedInst = OrigInst.clone();
            DivBuilder.Insert(ClonedInst);
            VMap[&OrigInst] = ClonedInst;
        }

        Type *VoidTy = DivBuilder.getVoidTy();
        // Type *Int32Ty = DivBuilder.getInt32Ty();

        FunctionType *FuncTy1 = FunctionType::get(VoidTy, {}, false);
        Function *riscv_gpu_tmask_reverse = Function::Create(FuncTy1, Function::ExternalLinkage, "reverse_riscv_gpu_tmask", M);

        DivBuilder.CreateCall(riscv_gpu_tmask_reverse);

        for (Instruction &OrigInst : *OrgBranch2nd) {
            Instruction *ClonedInst = OrigInst.clone();
            DivBuilder.Insert(ClonedInst);
            VMap[&OrigInst] = ClonedInst;
        }
        
        // DivBuilder.CreateBr(BrTermBB); 

        for (Instruction &I : *DivBB) {
            RemapInstruction(&I, VMap, RF_IgnoreMissingLocals, nullptr);
        }

        
        Value *sss = getValueBeforeTerminator(DivBB);

        for(Instruction &I: *BrTermBB){
            if(PHINode *PN = dyn_cast<PHINode>(&I)){
                PN->addIncoming(sss, DivBB);
            }
        }
        
    }

    for(auto *br_inst : eraseBrList){
        br_inst->eraseFromParent();
        changed = true;
    }        
    
    unsigned kkkk = 0;
    kkkk++;
    
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