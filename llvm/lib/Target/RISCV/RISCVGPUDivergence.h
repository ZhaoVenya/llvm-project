/*************************************************************************\
 * FileName:    RISCVGPUDivergence.h
 * Thread Mask will be generated in Warp Scheduler.
 * 1.首先要判断一个函数中有没有br这种节点，并且br这种节点是if...else..这种语句中的节点
 * 2.替换br跳转的两个分支，这两个basic block加上div_前缀
 * 3.新建这两个对应的BB，但是新建的这两个BB中的内容是空的
\*************************************************************************/


#ifndef RISCVGPU_DIVERGENCE_H_
#define RISCVGPU_DIVERGENCE_H_

#include "llvm/Pass.h"
#include "llvm/Analysis/LoopInfo.h"

namespace llvm {


struct RISCVGPUDivergence : public FunctionPass{

public:
    static char ID;

    RISCVGPUDivergence():FunctionPass(ID){}

    StringRef getPassName() const override;

    bool isLoopBr(BranchInst *BI, LoopInfo &LI);

    bool runOnFunction(Function &F) override;

    void getAnalysisUsage(AnalysisUsage &AU)const override;
    
};
 
}



#endif
