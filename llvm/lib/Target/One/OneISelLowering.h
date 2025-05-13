#ifndef ONEISELLOWERING_H
#define ONEISELLOWERING_H

#include "llvm/CodeGen/TargetLowering.h"

namespace llvm{

    class OneSubtarget;

    namespace OneISD{
        enum NodeType : unsigned{
            FIRST_NUM=ISD::BUILTIN_OP_END, RET_GLUE
        };
    }

    class OneTargetLowering : public TargetLowering
    {
        const OneSubtarget &Subtarget;

    public:
        explicit OneTargetLowering(const TargetMachine &TM,
                               const OneSubtarget &STI);

        const OneSubtarget &getSubtarget() const { return Subtarget; }


        SDValue LowerFormalArguments(SDValue Chain, CallingConv::ID CallConv,
                            bool IsVarArg,
                            const SmallVectorImpl<ISD::InputArg> &Ins,
                            const SDLoc &DL, SelectionDAG &DAG,
                            SmallVectorImpl<SDValue> &InVals) const override;

        SDValue LowerReturn(SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
                      const SmallVectorImpl<ISD::OutputArg> &Outs,
                      const SmallVectorImpl<SDValue> &OutVals, const SDLoc &DL,
                      SelectionDAG &DAG) const override;
        
        
        

    };
    
}


#endif

