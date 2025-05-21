def RetCC_Day : CallingConv<[
    CCIfType<[i32], CCAssignToReg<[A0,A1]>>,
]>;

def CC_Day : CallingConv<[
    CCIfType<[i32], CCAssignToReg<[A0,A1]>>,
    CCAssignToStack<4,4>
]>;

def CSR : CalleeSavedRegs<(add RA, S0, S1)>;

