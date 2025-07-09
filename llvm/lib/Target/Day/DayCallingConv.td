


// DayCallingConv.td - Day Calling Conventions Definition.
//
// This file defines the calling conventions for the Day architecture.
//
//===----------------------------------------------------------------------===//

// Include the base TargetCallingConv.td for common definitions
include "llvm/Target/TargetCallingConv.td"

include "DayRegisterInfo.td"

//// CSRs: Callee Saved Registers.
def DayCSRs : CalleeSavedRegs<(add
    RA,  // ra - Return address (callee must save if making further calls)
    S0,  // s0/fp
    S1,  // s1
    S2,  // s2
    S3,  // s3
    S4,  // s4
    S5,  // s5
    S6,  // s6
    S7,  // s7
    S8,  // s8
    S9,  // s9
    S10, // s10
    S11  // s11
)>;

def Day_Standard_CC : CallingConv<
    [CCDelegateTo<DayCSRs>]
>;

def Day_VarArg_CC : VarArgCallingConv<
    [CCAssignToStack<4, 4>]
>;


