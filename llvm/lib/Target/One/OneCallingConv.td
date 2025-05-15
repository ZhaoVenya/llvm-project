def RetCC_One : CallingConv<[
    CCIfType<[i32], CCAssignToReg<[X10,X11]>>,
]>;

