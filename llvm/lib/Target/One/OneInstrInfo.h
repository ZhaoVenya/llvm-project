


#ifndef ONEINSTSRINFO_H
#define ONEINSTSRINFO_H

#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "OneGenInstrInfo.inc"

namespace llvm{
    class OneInstrInfo : public OneGenInstrInfo{
        public:
            explicit OneInstrInfo();

    };


}


#endif
