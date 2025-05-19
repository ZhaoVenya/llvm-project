


#ifndef ONEFRAMELOWERING_H
#define ONEFRAMELOWERING_H

// #include "OneSubtarget.h"
// #include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
// #include <cstdint>

namespace llvm{
    class OneSubtarget;

    class OneFrameLowering : public TargetFrameLowering
    {
        const OneSubtarget &STI;

        public:
            explicit OneFrameLowering(const OneSubtarget &STI)
             : TargetFrameLowering(TargetFrameLowering::StackGrowsDown, Align(16),0,Align(16))
             ,STI(STI)
             {
             }
            
            void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

            void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

        protected:
            bool hasFPImpl(const MachineFunction &MF) const override;


        private:
            uint64_t computeStateSize(MachineFunction &MF) const;
    };
    
    



}


#endif

