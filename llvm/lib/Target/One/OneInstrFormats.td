// class OneOpcode <string name, bits<7> val, bits<3> funct3> {
//     string Name = name;
//     bits<7> Value = val;
//     bits<3> FUNCT3 = funct3;
// }

// def OPC_ADDI : OneOpcode<"ADDI",0b0010011, 0b000>;
// def OPC_JALR : OneOpcode<"JALR",0b1101111, 0b000>;

class OneInst<dag outs, dag ins, string asmstr, list<dag> pattern> : Instruction {
    field bits<32> SoftFail = 0;

    let Size = 4;

    let Namespace = "One";

    dag OutOperandList = outs;
    dag InOperandList = ins;
    let AsmString = asmstr;
    let Pattern = pattern;
}

class PseudoInst<dag outs, dag ins, list<dag> pattern, string asmstr = "">
    : OneInst<outs, ins, asmstr, pattern> {
    let isPseudo = 1;
    let isCodeGenOnly = 1;
}

class R<dag outs, dag ins, string asmstr, list<dag> pattern>
    : OneInst<outs, ins, asmstr, pattern> {
}

class I<dag outs, dag ins, string asmstr, list<dag> pattern>
    : OneInst<outs, ins, asmstr, pattern> {
}

class U<dag outs, dag ins, string asmstr, list<dag> pattern>
    : OneInst<outs, ins, asmstr, pattern> {
}

class J<dag outs, dag ins, string asmstr, list<dag> pattern>
    : OneInst<outs, ins, asmstr, pattern> {
}

class B<dag outs, dag ins, string asmstr, list<dag> pattern>
    : OneInst<outs, ins, asmstr, pattern> {
}

class S<dag outs, dag ins, string asmstr, list<dag> pattern>
    : OneInst<outs, ins, asmstr, pattern> {
}
