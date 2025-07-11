/*********************************************************************\
    * File: DayRegisterInfo.td
    * Description: Register information for the Day architecture.
\*********************************************************************/

// 定义一个命名空间 "Day"，用于组织 Day 架构的寄存器信息。
let Namespace = "Day" in {
    class DayReg<bits<5>Enc, string n,list<string> alt = []>:Register<n>{
        let HWEncoding{4-0} = Enc;
        let AltNames = alt;
    }
}

def ZERO:DayReg<0b00000, "r0",  ["zero"]>;
def RA:  DayReg<0b00001, "r1",  ["ra"]>;
def SP:  DayReg<0b00010, "r2",  ["sp"]>;
def GP:  DayReg<0b00011, "r3",  ["gp"]>;
def TP:  DayReg<0b00100, "r4",  ["tp"]>;
def T0:  DayReg<0b00101, "r5",  ["t0"]>;
def T1:  DayReg<0b00110, "r6",  ["t1"]>;
def T2:  DayReg<0b00111, "r7",  ["t2"]>;
def T3:  DayReg<0b01000, "r8",  ["t3"]>;
def T4:  DayReg<0b01001, "r9",  ["t4"]>;
def T5:  DayReg<0b01010, "r10", ["t5"]>;
def T6:  DayReg<0b01011, "r11", ["t6"]>;
def S0:  DayReg<0b01100, "r12", ["s0,fp"]>;
def S1:  DayReg<0b01101, "r13", ["s1"]>;
def S2:  DayReg<0b01110, "r14", ["s2"]>;
def S3:  DayReg<0b01111, "r15", ["s3"]>;
def S4:  DayReg<0b10000, "r16", ["s4"]>;
def S5:  DayReg<0b10001, "r17", ["s5"]>;
def S6:  DayReg<0b10010, "r18", ["s6"]>;
def S7:  DayReg<0b10011, "r19", ["s7"]>;
def S8:  DayReg<0b10100, "r20", ["s8"]>;
def S9:  DayReg<0b10101, "r21", ["s9"]>;
def S10: DayReg<0b10110, "r22", ["s10"]>;
def S11: DayReg<0b10111, "r23", ["s11"]>;
def A0:  DayReg<0b11000, "r24", ["a0"]>;
def A1:  DayReg<0b11001, "r25", ["a1"]>;
def A2:  DayReg<0b11010, "r26", ["a2"]>;
def A3:  DayReg<0b11011, "r27", ["a3"]>;
def A4:  DayReg<0b11100, "r28", ["a4"]>;
def A5:  DayReg<0b11101, "r29", ["a5"]>;
def A6:  DayReg<0b11110, "r30", ["a6"]>;
def A7:  DayReg<0b11111, "r31", ["a7"]>;


def GPR : RegisterClass<"Day", [i32],32,(add
    ZERO, RA, SP, GP, TP, T0, T1, T2, T3, T4, T5, T6,
    S0, S1, S2, S3, S4, S5, S6, S7, S8, S9,
    S10, S11, A0, A1, A2, A3, A4, A5, A6, A7)>;


def F00:  DayReg<0b00000, "f00",  ["fp00"]>;
def F01:  DayReg<0b00001, "f01",  ["fp01"]>;
def F02:  DayReg<0b00010, "f02",  ["fp02"]>;
def F03:  DayReg<0b00011, "f03",  ["fp03"]>;
def F04:  DayReg<0b00100, "f04",  ["fp04"]>;
def F05:  DayReg<0b00101, "f05",  ["fp05"]>;
def F06:  DayReg<0b00110, "f06",  ["fp06"]>;
def F07:  DayReg<0b00111, "f07",  ["fp07"]>;
def F08:  DayReg<0b01000, "f08",  ["fp08"]>;
def F09:  DayReg<0b01001, "f09",  ["fp09"]>;
def F10:  DayReg<0b01010, "f10",  ["fp10"]>;
def F11:  DayReg<0b01011, "f11",  ["fp11"]>;
def F12:  DayReg<0b01100, "f12",  ["fp12"]>;
def F13:  DayReg<0b01101, "f13",  ["fp13"]>;
def F14:  DayReg<0b01110, "f14",  ["fp14"]>;
def F15:  DayReg<0b01111, "f15",  ["fp15"]>;
def F16:  DayReg<0b10000, "f16",  ["fp16"]>;
def F17:  DayReg<0b10001, "f17",  ["fp17"]>;
def F18:  DayReg<0b10010, "f18",  ["fp18"]>;
def F19:  DayReg<0b10011, "f19",  ["fp19"]>;
def F20:  DayReg<0b10100, "f20",  ["fp20"]>;
def F21:  DayReg<0b10101, "f21",  ["fp21"]>;
def F22:  DayReg<0b10110, "f22",  ["fp22"]>;
def F23:  DayReg<0b10111, "f23",  ["fp23"]>;
def F24:  DayReg<0b11000, "f24",  ["fp24"]>;
def F25:  DayReg<0b11001, "f25",  ["fp25"]>;
def F26:  DayReg<0b11010, "f26",  ["fp26"]>;
def F27:  DayReg<0b11011, "f27",  ["fp27"]>;
def F28:  DayReg<0b11100, "f28",  ["fp28"]>;
def F29:  DayReg<0b11101, "f29",  ["fp29"]>;
def F30:  DayReg<0b11110, "f30",  ["fp30"]>;
def F31:  DayReg<0b11111, "f31",  ["fp31"]>;


def FPR : RegisterClass<"Day", [f32],32,(add
    F00, F01, F02, F03, F04, F05, F06, F07, F08, F09,
    F10, F11, F12, F13, F14, F15, F16, F17, F18, F19,
    F20, F21, F22, F23, F24, F25, F26, F27, F28, F29,
    F30, F31)>;
