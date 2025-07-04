/*********************************************************************\
    * File: DayRegisterInfo.td
    * Description: Register information for the Day architecture.
\*********************************************************************/

// 定义一个命名空间 "Day"，用于组织 Day 架构的寄存器信息。
let Namespace = "Day" in {
    class DayReg<bits<5>Enc, string n,list<string> alt = []>:Register<n>{
        let HWEncoding = Enc;
        let AltNames = alt;
        let Size = 4; // Assuming 32-bit registers
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

