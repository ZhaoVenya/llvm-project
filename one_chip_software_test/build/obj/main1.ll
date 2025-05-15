    ; ModuleID = 'test.c'
    source_filename = "test.c"
    target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128"
    target triple = "riscv32-apple-macosx14.0.0"

    ; Function Attrs: noinline nounwind optnone ssp uwtable(sync)
    define i32 @main() #0 {
    ret i32 42
    }