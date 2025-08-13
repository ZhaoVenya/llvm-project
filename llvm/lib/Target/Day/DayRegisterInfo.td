/*********************************************************************\
    * File: DayRegisterInfo.td
    * Description: Register information for the Day architecture.
\*********************************************************************/

// 定义一个命名空间 "Day"，用于组织 Day 架构的寄存器信息。
def ABIRegAltName : RegAltNameIndex;


let Namespace = "Day" in {
    class DayReg<bits<5>Enc, string n,list<string> alt = []>:Register<n>{
        let HWEncoding{4-0} = Enc;
        let AltNames = alt;
        let RegAltNameIndices = [ABIRegAltName];
    }
}

def ZERO:DayReg<0b00000, "r0",  ["zero"]>, DwarfRegNum<[0]>;
def RA:  DayReg<0b00001, "r1",  ["ra"]>, DwarfRegNum<[1]>;
def SP:  DayReg<0b00010, "r2",  ["sp"]>, DwarfRegNum<[2]>;
def GP:  DayReg<0b00011, "r3",  ["gp"]>, DwarfRegNum<[3]>;
def TP:  DayReg<0b00100, "r4",  ["tp"]>, DwarfRegNum<[4]>;
def T0:  DayReg<0b00101, "r5",  ["t0"]>, DwarfRegNum<[5]>;
def T1:  DayReg<0b00110, "r6",  ["t1"]>, DwarfRegNum<[6]>;
def T2:  DayReg<0b00111, "r7",  ["t2"]>, DwarfRegNum<[7]>;
def T3:  DayReg<0b01000, "r8",  ["t3"]>, DwarfRegNum<[8]>;
def T4:  DayReg<0b01001, "r9",  ["t4"]>, DwarfRegNum<[9]>;
def T5:  DayReg<0b01010, "r10", ["t5"]>, DwarfRegNum<[10]>;
def T6:  DayReg<0b01011, "r11", ["t6"]>, DwarfRegNum<[11]>;
def S0:  DayReg<0b01100, "r12", ["s0,fp"]>, DwarfRegNum<[12]>;
def S1:  DayReg<0b01101, "r13", ["s1"]>, DwarfRegNum<[13]>;
def S2:  DayReg<0b01110, "r14", ["s2"]>, DwarfRegNum<[14]>;
def S3:  DayReg<0b01111, "r15", ["s3"]>, DwarfRegNum<[15]>;
def S4:  DayReg<0b10000, "r16", ["s4"]>, DwarfRegNum<[16]>;
def S5:  DayReg<0b10001, "r17", ["s5"]>, DwarfRegNum<[17]>;
def S6:  DayReg<0b10010, "r18", ["s6"]>, DwarfRegNum<[18]>;
def S7:  DayReg<0b10011, "r19", ["s7"]>, DwarfRegNum<[19]>;
def S8:  DayReg<0b10100, "r20", ["s8"]>, DwarfRegNum<[20]>;
def S9:  DayReg<0b10101, "r21", ["s9"]>, DwarfRegNum<[21]>;
def S10: DayReg<0b10110, "r22", ["s10"]>, DwarfRegNum<[22]>;
def S11: DayReg<0b10111, "r23", ["s11"]>, DwarfRegNum<[23]>;
def A0:  DayReg<0b11000, "r24", ["a0"]>, DwarfRegNum<[24]>;
def A1:  DayReg<0b11001, "r25", ["a1"]>, DwarfRegNum<[25]>;
def A2:  DayReg<0b11010, "r26", ["a2"]>, DwarfRegNum<[26]>;
def A3:  DayReg<0b11011, "r27", ["a3"]>, DwarfRegNum<[27]>;
def A4:  DayReg<0b11100, "r28", ["a4"]>, DwarfRegNum<[28]>;
def A5:  DayReg<0b11101, "r29", ["a5"]>, DwarfRegNum<[29]>;
def A6:  DayReg<0b11110, "r30", ["a6"]>, DwarfRegNum<[30]>;
def A7:  DayReg<0b11111, "r31", ["a7"]>, DwarfRegNum<[31]>;


def GPR : RegisterClass<"Day", [i32],32,(add
    ZERO, RA, SP, GP, TP, T0, T1, T2, T3, T4, T5, T6,
    S0, S1, S2, S3, S4, S5, S6, S7, S8, S9,
    S10, S11, A0, A1, A2, A3, A4, A5, A6, A7)>;


def F00:  DayReg<0b00000, "f00",  ["fp00"]>, DwarfRegNum<[32]>;
def F01:  DayReg<0b00001, "f01",  ["fp01"]>, DwarfRegNum<[33]>;
def F02:  DayReg<0b00010, "f02",  ["fp02"]>, DwarfRegNum<[34]>;
def F03:  DayReg<0b00011, "f03",  ["fp03"]>, DwarfRegNum<[35]>;
def F04:  DayReg<0b00100, "f04",  ["fp04"]>, DwarfRegNum<[36]>;
def F05:  DayReg<0b00101, "f05",  ["fp05"]>, DwarfRegNum<[37]>;
def F06:  DayReg<0b00110, "f06",  ["fp06"]>, DwarfRegNum<[38]>;
def F07:  DayReg<0b00111, "f07",  ["fp07"]>, DwarfRegNum<[39]>;
def F08:  DayReg<0b01000, "f08",  ["fp08"]>, DwarfRegNum<[40]>;
def F09:  DayReg<0b01001, "f09",  ["fp09"]>, DwarfRegNum<[41]>;
def F10:  DayReg<0b01010, "f10",  ["fp10"]>, DwarfRegNum<[42]>;
def F11:  DayReg<0b01011, "f11",  ["fp11"]>, DwarfRegNum<[43]>;
def F12:  DayReg<0b01100, "f12",  ["fp12"]>, DwarfRegNum<[44]>;
def F13:  DayReg<0b01101, "f13",  ["fp13"]>, DwarfRegNum<[45]>;
def F14:  DayReg<0b01110, "f14",  ["fp14"]>, DwarfRegNum<[46]>;
def F15:  DayReg<0b01111, "f15",  ["fp15"]>, DwarfRegNum<[47]>;
def F16:  DayReg<0b10000, "f16",  ["fp16"]>, DwarfRegNum<[48]>;
def F17:  DayReg<0b10001, "f17",  ["fp17"]>, DwarfRegNum<[49]>;
def F18:  DayReg<0b10010, "f18",  ["fp18"]>, DwarfRegNum<[50]>;
def F19:  DayReg<0b10011, "f19",  ["fp19"]>, DwarfRegNum<[51]>;
def F20:  DayReg<0b10100, "f20",  ["fp20"]>, DwarfRegNum<[52]>;
def F21:  DayReg<0b10101, "f21",  ["fp21"]>, DwarfRegNum<[53]>;
def F22:  DayReg<0b10110, "f22",  ["fp22"]>, DwarfRegNum<[54]>;
def F23:  DayReg<0b10111, "f23",  ["fp23"]>, DwarfRegNum<[55]>;
def F24:  DayReg<0b11000, "f24",  ["fp24"]>, DwarfRegNum<[56]>;
def F25:  DayReg<0b11001, "f25",  ["fp25"]>, DwarfRegNum<[57]>;
def F26:  DayReg<0b11010, "f26",  ["fp26"]>, DwarfRegNum<[58]>;
def F27:  DayReg<0b11011, "f27",  ["fp27"]>, DwarfRegNum<[59]>;
def F28:  DayReg<0b11100, "f28",  ["fp28"]>, DwarfRegNum<[60]>;
def F29:  DayReg<0b11101, "f29",  ["fp29"]>, DwarfRegNum<[61]>;
def F30:  DayReg<0b11110, "f30",  ["fp30"]>, DwarfRegNum<[62]>;
def F31:  DayReg<0b11111, "f31",  ["fp31"]>, DwarfRegNum<[63]>;


def FPR : RegisterClass<"Day", [f32],32,(add
    F00, F01, F02, F03, F04, F05, F06, F07, F08, F09,
    F10, F11, F12, F13, F14, F15, F16, F17, F18, F19,
    F20, F21, F22, F23, F24, F25, F26, F27, F28, F29,
    F30, F31)>;
