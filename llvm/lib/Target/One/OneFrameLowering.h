


#ifndef ONEFRAMELOWERING_H
#define ONEFRAMELOWERING_H

#include "llvm/CodeGen/TargetFrameLowering.h"

namespace llvm{
    class OneSubtarget;

    class OneFrameLowering : public TargetFrameLowering
    {
        const OneSubtarget &STI;

        public:
            explicit OneFrameLowering(const OneSubtarget &STI)
             : TargetFrameLowering(TargetFrameLowering::StackGrowsDown, Align(4),0,Align(4))
             ,STI(STI)
             {
             }
            
            void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

            void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

        protected:
            bool hasFPImpl(const MachineFunction &MF) const override;



    };
    
    



}



#endif

