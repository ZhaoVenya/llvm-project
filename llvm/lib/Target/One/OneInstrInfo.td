include "OneInstrFormats.td"

def retglue : SDNode<"OneISD::RET_GLUE", SDTNone, 
                    [SDNPHasChain, SDNPOptInGlue,SDNPVariadic]>;


def imm12 : Operand<i32>, ImmLeaf<i32, [{return isInt<12>(Imm);}]>;
// def imm12Pat : ImmLeaf<i32, [{return isInt<12>(Imm);}]>;
// def imm12Pat : PatLeaf<(imm), [{return isInt<12>(N->getSExtValue());}]>;


// load store
// 操作的是地址
// 对地址建模 -> 内存操作

def mem : Operand<iPTR>{
    let MIOperandInfo = (ops GPR, imm12);
    // 指明自定义的打印函数
    let PrintMethod = "printMemOperand";
}

// def SDNPWantParent : SDNodeProperty;

// def memPat : ComplexPattern<iPTR, 2, "SelectAddrFI", [frameindex], [SDNPWantParent]>;
def memPat : ComplexPattern<iPTR, 2, "SelectAddrFI", [frameindex]>;


def LOAD : OneInst<(outs GPR:$ra), (ins mem: $addr), "lw \t$ra,$addr", []>;
def : Pat <(load memPat:$addr), (LOAD memPat: $addr)>;

def STORE : OneInst<(outs), (ins GPR:$rd, mem: $addr), "sw \t$rd,$addr", []>;
def : Pat <(store GPR:$rd, memPat:$addr), (STORE GPR:$rd, memPat: $addr)>;

def ADDI : I<(outs GPR:$rd), 
            (ins GPR:$rs1, imm12:$imm12), 
            "addi $rd, $rs1, $imm12", []>;



def : Pat <(i32 (imm12:$imm12)), (ADDI ZERO, imm12:$imm12)>;

let isCall = 1 in {
    def JALR : I <(outs GPR: $rd), (ins GPR:$rs1, imm12: $imm12),
    "jalr $rd, $rs1, $imm12", []>;

}


let isBarrier = 1, isReturn = 1, isTerminator = 1 in{
    def PseudoRET : PseudoInst<
        (outs),
        (ins),
        [(retglue)]>,
        PseudoInstExpansion<(JALR ZERO, RA, 0)>;
}



def : InstAlias<"ret", (JALR ZERO, RA, 0)>;
def : InstAlias<"li $rd, $imm", (ADDI GPR:$rd, ZERO, imm12:$imm)>;
