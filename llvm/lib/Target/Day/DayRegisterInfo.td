
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// 

class DayReg<bits<16> Enc, string n, list<string> alt = []> :Register<n>{
    let Namespace = "Day";
    let HWEncoding = Enc;
    let altNames = alt;
}


def ZERO:   DayReg <0,  "zero",  ["zero"]>,DwarfRegNum<[0]>;
def RA  :   DayReg <1,  "ra"  ,  ["ra"  ]>,DwarfRegNum<[1]>;
def SP  :   DayReg <2,  "sp"  ,  ["sp"  ]>,DwarfRegNum<[2]>;
def GP  :   DayReg <3,  "gp"  ,  ["gp"  ]>,DwarfRegNum<[3]>;
def TP  :   DayReg <4,  "tp"  ,  ["tp"  ]>,DwarfRegNum<[4]>;
def T0  :   DayReg <5,  "t0"  ,  ["t0"  ]>,DwarfRegNum<[5]>;
def T1  :   DayReg <6,  "t1"  ,  ["t1"  ]>,DwarfRegNum<[6]>;
def T2  :   DayReg <7,  "t2"  ,  ["t2"  ]>,DwarfRegNum<[7]>;
def S0  :   DayReg <8,  "s0"  ,  ["s0"  ]>,DwarfRegNum<[8]>;
def S1  :   DayReg <9,  "s1"  ,  ["s1"  ]>,DwarfRegNum<[9]>;
def A0  :   DayReg <10, "a0"  ,  ["a0"  ]>,DwarfRegNum<[10]>;
def A1  :   DayReg <11, "a1"  ,  ["a1"  ]>,DwarfRegNum<[11]>;
def A2  :   DayReg <12, "a2"  ,  ["a2"  ]>,DwarfRegNum<[12]>;
def A3  :   DayReg <13, "a3"  ,  ["a3"  ]>,DwarfRegNum<[13]>;
def A4  :   DayReg <14, "a4"  ,  ["a4"  ]>,DwarfRegNum<[14]>;
def A5  :   DayReg <15, "a5"  ,  ["a5"  ]>,DwarfRegNum<[15]>;
def A6  :   DayReg <16, "a6"  ,  ["a6"  ]>,DwarfRegNum<[16]>;
def A7  :   DayReg <17, "a7"  ,  ["a7"  ]>,DwarfRegNum<[17]>;
def S2  :   DayReg <18, "s2"  ,  ["s2"  ]>,DwarfRegNum<[18]>;
def S3  :   DayReg <19, "s3"  ,  ["s3"  ]>,DwarfRegNum<[19]>;
def S4  :   DayReg <20, "s4"  ,  ["s4"  ]>,DwarfRegNum<[20]>;
def S5  :   DayReg <21, "s5"  ,  ["s5"  ]>,DwarfRegNum<[21]>;
def S6  :   DayReg <22, "s6"  ,  ["s6"  ]>,DwarfRegNum<[22]>;
def S7  :   DayReg <23, "s7"  ,  ["s7"  ]>,DwarfRegNum<[23]>;
def S8  :   DayReg <24, "s8"  ,  ["s8"  ]>,DwarfRegNum<[24]>;
def S9  :   DayReg <25, "s9"  ,  ["s9"  ]>,DwarfRegNum<[25]>;
def S10 :   DayReg <26, "s10" ,  ["s10" ]>,DwarfRegNum<[26]>;
def S11 :   DayReg <27, "s11" ,  ["s11" ]>,DwarfRegNum<[27]>;
def T3  :   DayReg <28, "t3"  ,  ["t3"  ]>,DwarfRegNum<[28]>;
def T4  :   DayReg <29, "t4"  ,  ["t4"  ]>,DwarfRegNum<[29]>;
def T5  :   DayReg <30, "t5"  ,  ["t5"  ]>,DwarfRegNum<[30]>;
def T6  :   DayReg <31, "t6"  ,  ["t6"  ]>,DwarfRegNum<[31]>;


def GPR : RegisterClass<"Day", [i32], 32, (add 
        ZERO, RA, SP, GP, TP,
        A0 , A1 ,A2 ,A3 ,A4 ,A5 ,A6 ,A7 ,
        T0 , T1 ,T2 , T3, T4, T5, T6, 
        S0 , S1 ,S2 ,S3 ,S4 ,S5 ,S6 ,S7 ,S8 ,S9 ,S10 ,S11 
    )>;

