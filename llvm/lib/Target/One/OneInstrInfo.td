include "OneInstrFormats.td"
// include "llvm/Target/TargetSelectionDAG.td"

def retglue : SDNode<"OneISD::RET_GLUE", SDTNone, 
                    [SDNPHasChain, SDNPOptInGlue,SDNPVariadic]>;


def ADDI : I<OPC_ADDI, (outs GPR:$rd), 
            (ins GPR:$rs1, i32imm:$imm12), 
            "addi $rd, $rs1, $imm12", []>;



def : Pat <(i32 (imm:$imm12)), (ADDI X0, imm:$imm12)>;

let isCall = 1 in {
    def JALR : I <OPC_JALR, (outs GPR: $rd), (ins GPR:$rs1, i32imm: $imm12),
    "jarl $rd, $rs1, &imm12", []>;

}


let isBarrier = 1, isReturn = 1, isTerminator = 1 in{
    def PseudoRET : PseudoInst<
        (outs),
        (ins),
        [(retglue)]>,
        PseudoInstExpansion<(JALR X0, X1, 0)>;
}

