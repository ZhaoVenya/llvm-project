; ModuleID = 'src/main.c'
source_filename = "src/main.c"
target datalayout = "e-m:e-p:32:32-i64:64-n32-S128"
target triple = "riscv32-unknown-unknown-elf"

; Function Attrs: noinline nounwind optnone
define dso_local i32 @test() #0 {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  %d = alloca i32, align 4
  %e = alloca i32, align 4
  %f = alloca i32, align 4
  %g = alloca i32, align 4
  %h = alloca i32, align 4
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %k = alloca i32, align 4
  %l = alloca i32, align 4
  %m = alloca i32, align 4
  store i32 7, ptr %a, align 4
  %0 = load i32, ptr %a, align 4
  %add = add nsw i32 %0, 5
  store i32 %add, ptr %b, align 4
  %1 = load i32, ptr %a, align 4
  %sub = sub nsw i32 %1, 1
  store i32 %sub, ptr %c, align 4
  %2 = load i32, ptr %b, align 4
  %3 = load i32, ptr %c, align 4
  %add1 = add nsw i32 %2, %3
  store i32 %add1, ptr %d, align 4
  %4 = load i32, ptr %b, align 4
  %5 = load i32, ptr %c, align 4
  %mul = mul nsw i32 %4, %5
  store i32 %mul, ptr %e, align 4
  %6 = load i32, ptr %b, align 4
  %7 = load i32, ptr %c, align 4
  %div = sdiv i32 %6, %7
  store i32 %div, ptr %f, align 4
  %8 = load i32, ptr %b, align 4
  %9 = load i32, ptr %c, align 4
  %rem = srem i32 %8, %9
  store i32 %rem, ptr %g, align 4
  %10 = load i32, ptr %b, align 4
  %and = and i32 %10, 1
  store i32 %and, ptr %h, align 4
  %11 = load i32, ptr %b, align 4
  %or = or i32 %11, 1
  store i32 %or, ptr %i, align 4
  %12 = load i32, ptr %b, align 4
  %xor = xor i32 %12, 3
  store i32 %xor, ptr %j, align 4
  %13 = load i32, ptr %b, align 4
  %14 = load i32, ptr %c, align 4
  %xor2 = xor i32 %13, %14
  store i32 %xor2, ptr %k, align 4
  %15 = load i32, ptr %k, align 4
  %shl = shl i32 %15, 3
  store i32 %shl, ptr %k, align 4
  %16 = load i32, ptr %k, align 4
  %shr = ashr i32 %16, 3
  store i32 %shr, ptr %k, align 4
  %17 = load i32, ptr %k, align 4
  %18 = load i32, ptr %a, align 4
  %shl3 = shl i32 %17, %18
  store i32 %shl3, ptr %k, align 4
  %19 = load i32, ptr %k, align 4
  %20 = load i32, ptr %a, align 4
  %shr4 = ashr i32 %19, %20
  store i32 %shr4, ptr %k, align 4
  %21 = load i32, ptr %b, align 4
  %22 = load i32, ptr %c, align 4
  %and5 = and i32 %21, %22
  %23 = load i32, ptr %d, align 4
  %and6 = and i32 %and5, %23
  store i32 %and6, ptr %l, align 4
  %24 = load i32, ptr %b, align 4
  %25 = load i32, ptr %c, align 4
  %or7 = or i32 %24, %25
  %or8 = or i32 %or7, 1
  store i32 %or8, ptr %m, align 4
  %26 = load i32, ptr %a, align 4
  %27 = load i32, ptr %b, align 4
  %add9 = add nsw i32 %26, %27
  %28 = load i32, ptr %c, align 4
  %add10 = add nsw i32 %add9, %28
  %29 = load i32, ptr %d, align 4
  %add11 = add nsw i32 %add10, %29
  %30 = load i32, ptr %e, align 4
  %add12 = add nsw i32 %add11, %30
  %31 = load i32, ptr %f, align 4
  %add13 = add nsw i32 %add12, %31
  %32 = load i32, ptr %g, align 4
  %add14 = add nsw i32 %add13, %32
  %33 = load i32, ptr %h, align 4
  %add15 = add nsw i32 %add14, %33
  %34 = load i32, ptr %i, align 4
  %add16 = add nsw i32 %add15, %34
  %35 = load i32, ptr %j, align 4
  %add17 = add nsw i32 %add16, %35
  %36 = load i32, ptr %k, align 4
  %add18 = add nsw i32 %add17, %36
  %37 = load i32, ptr %l, align 4
  %add19 = add nsw i32 %add18, %37
  %38 = load i32, ptr %m, align 4
  %add20 = add nsw i32 %add19, %38
  ret i32 %add20
}

; Function Attrs: noinline nounwind optnone
define dso_local i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %a = alloca i32, align 4
  store i32 0, ptr %retval, align 4
  %call = call i32 @test()
  store i32 %call, ptr %a, align 4
  %0 = load i32, ptr %a, align 4
  ret i32 %0
}

attributes #0 = { noinline nounwind optnone "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic-rv32" "target-features"="+32bit,+relax,-a,-b,-c,-d,-e,-experimental-smctr,-experimental-ssctr,-experimental-zalasr,-experimental-zicfilp,-experimental-zicfiss,-experimental-zvbc32e,-experimental-zvkgs,-f,-h,-m,-sha,-shcounterenw,-shgatpa,-shtvala,-shvsatpa,-shvstvala,-shvstvecd,-smaia,-smcdeleg,-smcsrind,-smdbltrp,-smepmp,-smmpm,-smnpm,-smrnmi,-smstateen,-ssaia,-ssccfg,-ssccptr,-sscofpmf,-sscounterenw,-sscsrind,-ssdbltrp,-ssnpm,-sspm,-ssqosid,-ssstateen,-ssstrict,-sstc,-sstvala,-sstvecd,-ssu64xl,-supm,-svade,-svadu,-svbare,-svinval,-svnapot,-svpbmt,-svvptc,-v,-xcvalu,-xcvbi,-xcvbitmanip,-xcvelw,-xcvmac,-xcvmem,-xcvsimd,-xsfcease,-xsfvcp,-xsfvfnrclipxfqf,-xsfvfwmaccqqq,-xsfvqmaccdod,-xsfvqmaccqoq,-xsifivecdiscarddlone,-xsifivecflushdlone,-xtheadba,-xtheadbb,-xtheadbs,-xtheadcmo,-xtheadcondmov,-xtheadfmemidx,-xtheadmac,-xtheadmemidx,-xtheadmempair,-xtheadsync,-xtheadvdot,-xventanacondops,-xwchc,-za128rs,-za64rs,-zaamo,-zabha,-zacas,-zalrsc,-zama16b,-zawrs,-zba,-zbb,-zbc,-zbkb,-zbkc,-zbkx,-zbs,-zca,-zcb,-zcd,-zce,-zcf,-zcmop,-zcmp,-zcmt,-zdinx,-zfa,-zfbfmin,-zfh,-zfhmin,-zfinx,-zhinx,-zhinxmin,-zic64b,-zicbom,-zicbop,-zicboz,-ziccamoa,-ziccif,-zicclsm,-ziccrse,-zicntr,-zicond,-zicsr,-zifencei,-zihintntl,-zihintpause,-zihpm,-zimop,-zk,-zkn,-zknd,-zkne,-zknh,-zkr,-zks,-zksed,-zksh,-zkt,-zmmul,-ztso,-zvbb,-zvbc,-zve32f,-zve32x,-zve64d,-zve64f,-zve64x,-zvfbfmin,-zvfbfwma,-zvfh,-zvfhmin,-zvkb,-zvkg,-zvkn,-zvknc,-zvkned,-zvkng,-zvknha,-zvknhb,-zvks,-zvksc,-zvksed,-zvksg,-zvksh,-zvkt,-zvl1024b,-zvl128b,-zvl16384b,-zvl2048b,-zvl256b,-zvl32768b,-zvl32b,-zvl4096b,-zvl512b,-zvl64b,-zvl65536b,-zvl8192b" }

!llvm.module.flags = !{!0, !1, !2, !4, !5}
!llvm.ident = !{!6}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"target-abi", !"ilp32"}
!2 = !{i32 6, !"riscv-isa", !3}
!3 = !{!"rv32i2p1"}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{i32 8, !"SmallDataLimit", i32 0}
!6 = !{!"clang version 20.0.0git (https://github.com/ZhaoVenya/llvm-project.git f2b6c7b146abdcd935c54bd37a35638f4bbd122e)"}
