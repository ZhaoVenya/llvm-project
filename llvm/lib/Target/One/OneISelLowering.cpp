
#include "OneISelLowering.h"
#include "MCTargetDesc/OneMCTargetDesc.h"
#include "OneSubtarget.h"
#include "llvm/CodeGen/CallingConvLower.h"
// #include "llvm/CodeGen/SelectionDAG.h"


using namespace llvm;

#include "OneGenCallingConv.inc"

OneTargetLowering::OneTargetLowering(const TargetMachine &TM,const OneSubtarget &STI)
 : TargetLowering(TM), Subtarget(STI)
{
    /// 注册RegisterClass
    /// 还需要处理合法化（类型和操作）

    addRegisterClass(MVT::i32, &One::GPRRegClass);
    computeRegisterProperties(STI.getRegisterInfo());


}

SDValue OneTargetLowering::LowerCall(CallLoweringInfo &CLI, SmallVectorImpl<SDValue>& InVals) const {
    
  SelectionDAG &DAG = CLI.DAG;
  SDLoc &DL = CLI.DL;
  SmallVectorImpl<ISD::OutputArg> &Outs = CLI.Outs;
  SmallVectorImpl<SDValue> &OutVals = CLI.OutVals;
  SmallVectorImpl<ISD::InputArg> &Ins = CLI.Ins;
  SDValue Chain = CLI.Chain;
  SDValue Callee = CLI.Callee;
  CallingConv::ID CallConv = CLI.CallConv;
  bool &IsTailCall = CLI.IsTailCall;
  bool IsVarArg = CLI.IsVarArg;
    // 1. 处理实参，根据调用约定 （通过寄存器，内存栈来传递参数）
    // 2. 根据参数的寄存器个数，来生成对应的copyFromReg
    // 3. 生成节点
    // 4. 处理Call的返回值，根据Ins ,填充InVals

    GlobalAddressSDNode *N = dyn_cast<GlobalAddressSDNode> (Callee);
    Callee = DAG.getTargetGlobalAddress(N->getGlobal(), DL, getPointerTy(DAG.getDataLayout()));

    SmallVector<SDValue, 8> Ops(1, Chain);
    Ops.push_back(Callee);

    SDValue Glue;

    const TargetRegisterInfo *TRI = Subtarget.getRegisterInfo();
    const uint32_t *Mask = TRI->getCallPreservedMask(DAG.getMachineFunction(), CallConv);
    Ops.push_back(DAG.getRegisterMask(Mask));

    if(Glue.getNode()){
        Ops.push_back(Glue);
    }

    SDVTList NodeTYpe = DAG.getVTList(MVT::Other, MVT::Glue);
    DAG.getNode(OneISD::Call, DL, NodeTYpe, Ops);

    {
        /// 处理第四步
        SmallVector<CCValAssign, 2> RVLos;    CCState CCInfo(CallConv, IsVarArg, DAG.getMachineFunction(), RVLos,
                    *DAG.getContext());
        CCInfo.AnalyzeCallResult(Ins, RetCC_One);

        for (unsigned i = 0, e = RVLos.size(); i != e; ++i) {
        CCValAssign &VA = RVLos[i];
        EVT vt = RVLos[i].getLocVT();
        assert(VA.isRegLoc());
        unsigned RVReg = VA.getLocReg();
        SDValue Val = DAG.getCopyFromReg(Chain, DL, RVReg, vt, Glue);
        Chain = Val.getValue(1);
        Glue = Val.getValue(2);
        InVals.push_back(Val);
        }
    }



  return Chain;
}

SDValue OneTargetLowering::LowerFormalArguments(SDValue Chain, CallingConv::ID CallConv,
                    bool IsVarArg,
                    const SmallVectorImpl<ISD::InputArg> &Ins,
                    const SDLoc &DL, SelectionDAG &DAG,
                    SmallVectorImpl<SDValue> &InVals) const {
    return Chain;
}

SDValue OneTargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
                const SmallVectorImpl<ISD::OutputArg> &Outs,
                const SmallVectorImpl<SDValue> &OutVals, const SDLoc &DL,
                SelectionDAG &DAG) const {
    /// 返回物理寄存器
    SmallVector<CCValAssign, 16> RVLocs;
    CCState CCInfo(CallConv, IsVarArg, DAG.getMachineFunction(), RVLocs, *DAG.getContext());
    CCInfo.AnalyzeReturn(Outs,RetCC_One);

    SDValue Glue;
    SmallVector<SDValue, 4> RetOps(1, Chain);

    for (unsigned i =0, e=RVLocs.size(); i<e; ++i){
        CCValAssign &VA = RVLocs[i];
        assert(VA.isRegLoc() && "Can only return in register!");
        Chain = DAG.getCopyToReg(Chain, DL, VA.getLocReg(), OutVals[i], Glue);
        Glue = Chain.getValue(1);
        RetOps.push_back(DAG.getRegister(VA.getLocReg(), VA.getLocVT()));
    }
    RetOps[0] = Chain;

    if(Glue.getNode()){
        RetOps.push_back(Glue);
    }
    return DAG.getNode(OneISD::RET_GLUE, DL, MVT::Other, RetOps);
}



 const char *OneTargetLowering::getTargetNodeName(unsigned int Opcode) const {

    switch(Opcode){

        case OneISD::RET_GLUE:    
            return "OneISD::RET_GLUE";
        
        case OneISD::Call:
            return "OneISD::Call";
        
        default:
            return nullptr;

    }

 }
