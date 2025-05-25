//
// Created by 蔡鹏 on 2024/12/1.
//

#include "OneMCAsmInfo.h"

using namespace llvm;

// 打印汇编的注释符号
OneMCAsmInfo::OneMCAsmInfo(const Triple &TargetTriple) { 
    CommentString = "#"; 
}