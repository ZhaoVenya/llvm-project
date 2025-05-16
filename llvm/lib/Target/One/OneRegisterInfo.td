

class OneReg<string n> :Register<n>{
    let Namespace = "One";
}

 


def ZERO:   OneReg <"zero">,DwarfRegNum<[0]>;
def RA  :   OneReg <"ra">,DwarfRegNum<[1]>;
def SP  :   OneReg <"sp">,DwarfRegNum<[2]>;
def GP  :   OneReg <"gp">,DwarfRegNum<[3]>;
def TP  :   OneReg <"tp">,DwarfRegNum<[4]>;
def T0  :   OneReg <"t0">,DwarfRegNum<[5]>;
def T1  :   OneReg <"t1">,DwarfRegNum<[6]>;
def T2  :   OneReg <"t2">,DwarfRegNum<[7]>;
def S0  :   OneReg <"s0">,DwarfRegNum<[8]>;
def S1  :   OneReg <"s1">,DwarfRegNum<[9]>;
def A0  :   OneReg <"a0">,DwarfRegNum<[10]>;
def A1  :   OneReg <"a1">,DwarfRegNum<[11]>;
def A2  :   OneReg <"a2">,DwarfRegNum<[12]>;
def A3  :   OneReg <"a3">,DwarfRegNum<[13]>;
def A4  :   OneReg <"a4">,DwarfRegNum<[14]>;
def A5  :   OneReg <"a5">,DwarfRegNum<[15]>;
def A6  :   OneReg <"a6">,DwarfRegNum<[16]>;
def A7  :   OneReg <"a7">,DwarfRegNum<[17]>;
def S2  :   OneReg <"s2">,DwarfRegNum<[18]>;
def S3  :   OneReg <"s3 ">,DwarfRegNum<[19]>;
def S4  :   OneReg <"s4 ">,DwarfRegNum<[20]>;
def S5  :   OneReg <"s5 ">,DwarfRegNum<[21]>;
def S6  :   OneReg <"s6 ">,DwarfRegNum<[22]>;
def S7  :   OneReg <"s7 ">,DwarfRegNum<[23]>;
def S8  :   OneReg <"s8 ">,DwarfRegNum<[24]>;
def S9  :   OneReg <"s9 ">,DwarfRegNum<[25]>;
def S10 :   OneReg <"s10">,DwarfRegNum<[26]>;
def S11 :   OneReg <"s11">,DwarfRegNum<[27]>;






def GPR : RegisterClass<"One", [i32], 32, (add 
        ZERO, RA  ,SP  ,GP  ,TP  ,T0  ,T1  ,T2  ,S0  ,
        S1  ,A0  ,A1  ,A2  ,A3  ,A4  ,A5  ,A6  ,A7  ,
        S2  ,S3  ,S4  ,S5  ,S6  ,S7  ,S8  ,S9  ,S10 ,S11 
    )>;





