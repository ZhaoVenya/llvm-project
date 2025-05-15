



#ifndef ONEMCASMINFO_H
#define ONEMCASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm{

    class Triple;
    class OneMCAsmInfo:public MCAsmInfoELF{
    public:
        explicit OneMCAsmInfo(const Triple &TargetTriple);

    };

}



#endif

