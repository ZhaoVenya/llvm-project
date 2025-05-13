#ifndef ONE_H
#define ONE_H

namespace llvm
{
    class FunctionPass;
    class OneTargetMachine;

    
    FunctionPass *llvm::createOneISelDag(OneTargetMachine &TM);
} // namespace name


#endif
