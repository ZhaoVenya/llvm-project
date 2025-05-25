//
// Created by 蔡鹏 on 2024/12/1.
//

#ifndef ONEISELLOWERING_H
#define ONEISELLOWERING_H

#include "llvm/CodeGen/TargetLowering.h"

namespace llvm {
    class OneSubtarget;
    namespace OneISD {
        enum NodeType : unsigned { FIRST_NUMBER = ISD::BUILTIN_OP_END, RET_GLUE, Call, HI, LO };
    } // namespace OneISD

    class OneTargetLowering : public TargetLowering {
        const OneSubtarget &Subtarget;

        public:
            explicit OneTargetLowering(const TargetMachine &TM, const OneSubtarget &STI);

            // 这是一个简单的 getter 函数，返回对 OneSubtarget 成员的常量引用。
            const OneSubtarget &getSubtarget() const { return Subtarget; }

            SDValue LowerCall(CallLoweringInfo &CLI, SmallVectorImpl<SDValue> &InVals) const override;

            // 这个函数负责处理形式参数（Formal Arguments）的降低，即被调用函数（callee）如何接收从调用者传递过来的参数。
            SDValue LowerFormalArguments(SDValue Chain, CallingConv::ID CallConv,
                                        bool IsVarArg,
                                        const SmallVectorImpl<ISD::InputArg> &Ins,
                                        const SDLoc &DL, SelectionDAG &DAG,
                                        SmallVectorImpl<SDValue> &InVals) const override;

            SDValue LowerReturn(SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
                                const SmallVectorImpl<ISD::OutputArg> &Outs,
                                const SmallVectorImpl<SDValue> &OutVals, const SDLoc &DL,
                                SelectionDAG &DAG) const override;

            SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const override;

            const char *getTargetNodeName(unsigned Opcode) const override;
        private:
            SDValue LowerGlobalAddress(SDValue Op, SelectionDAG &DAG) const;
            SDValue LowerConstant(SDValue Op, SelectionDAG &DAG) const;
    };
} // namespace llvm

#endif // ONEISELLOWERING_H
