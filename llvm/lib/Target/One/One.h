#ifndef ONE_H
#define ONE_H


namespace llvm
{
    class FunctionPass;
    class OneTargetMachine;
    class PassRegistry;
    
    FunctionPass *createOneISelDag(OneTargetMachine &TM);

    void initializeOneDAGToDAGISelLegacyPass(PassRegistry &);
} // namespace name


#endif
