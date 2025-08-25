; ModuleID = 'kernel1.ll'
source_filename = "kernel1.cl"
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-unknown"

; Function Attrs: convergent nofree norecurse nounwind memory(argmem: readwrite)
define dso_local spir_kernel void @matrix_mul(ptr nocapture noundef readonly align 4 %A, ptr nocapture noundef readonly align 4 %B, ptr nocapture noundef writeonly align 4 %C, i32 noundef %M, i32 noundef %K, i32 noundef %N) local_unnamed_addr #0 !kernel_arg_addr_space !8 !kernel_arg_access_qual !9 !kernel_arg_type !10 !kernel_arg_base_type !10 !kernel_arg_type_qual !11 {
entry:
  %call = tail call i64 @_Z13get_global_idj(i32 noundef signext 0) #3
  %conv = trunc i64 %call to i32
  %call1 = tail call i64 @_Z13get_global_idj(i32 noundef signext 1) #3
  %conv2 = trunc i64 %call1 to i32
  %cmp = icmp sgt i32 %M, %conv
  %cmp4 = icmp sgt i32 %N, %conv2
  %or.cond = select i1 %cmp, i1 %cmp4, i1 false
  br i1 %or.cond, label %div_for.cond.preheader, label %div_if.end

div_for.cond.preheader:                           ; preds = %entry
  br label %for.cond.preheader

for.cond.preheader:                               ; preds = %div_for.cond.preheader
  %cmp628 = icmp sgt i32 %K, 0
  br i1 %cmp628, label %div_for.body.lr.ph, label %div_for.cond.cleanup

div_for.body.lr.ph:                               ; preds = %for.cond.preheader
  br label %for.body.lr.ph

for.body.lr.ph:                                   ; preds = %div_for.body.lr.ph
  %mul = mul nsw i32 %K, %conv
  %0 = sext i32 %mul to i64
  %1 = sext i32 %N to i64
  %sext = shl i64 %call1, 32
  %wide.trip.count = zext nneg i32 %K to i64
  %invariant.gep = getelementptr float, ptr %A, i64 %0
  %2 = ashr exact i64 %sext, 30
  %invariant.gep34 = getelementptr i8, ptr %B, i64 %2
  br label %for.body

div_for.cond.cleanup:                             ; preds = %for.cond.preheader
  br label %for.cond.cleanup

for.cond.cleanup:                                 ; preds = %div_for.cond.cleanup, %for.body
  %sum.0.lcssa = phi float [ 0.000000e+00, %div_for.cond.cleanup ], [ %6, %for.body ]
  %mul13 = mul nsw i32 %N, %conv
  %add14 = add nsw i32 %mul13, %conv2
  %idxprom15 = sext i32 %add14 to i64
  %arrayidx16 = getelementptr inbounds float, ptr %C, i64 %idxprom15
  store float %sum.0.lcssa, ptr %arrayidx16, align 4, !tbaa !12
  br label %if.end

for.body:                                         ; preds = %for.body, %for.body.lr.ph
  %indvars.iv = phi i64 [ 0, %for.body.lr.ph ], [ %indvars.iv.next, %for.body ]
  %sum.029 = phi float [ 0.000000e+00, %for.body.lr.ph ], [ %6, %for.body ]
  %gep = getelementptr float, ptr %invariant.gep, i64 %indvars.iv
  %3 = load float, ptr %gep, align 4, !tbaa !12
  %4 = mul nsw i64 %indvars.iv, %1
  %gep35 = getelementptr float, ptr %invariant.gep34, i64 %4
  %5 = load float, ptr %gep35, align 4, !tbaa !12
  %6 = tail call float @llvm.fmuladd.f32(float %3, float %5, float %sum.029)
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, %wide.trip.count
  br i1 %exitcond.not, label %for.cond.cleanup, label %for.body

div_if.end:                                       ; preds = %entry
  br label %if.end

if.end:                                           ; preds = %div_if.end, %for.cond.cleanup
  ret void
}

; Function Attrs: convergent mustprogress nofree nounwind willreturn memory(none)
declare dso_local i64 @_Z13get_global_idj(i32 noundef signext) local_unnamed_addr #1

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare float @llvm.fmuladd.f32(float, float, float) #2

attributes #0 = { convergent nofree norecurse nounwind memory(argmem: readwrite) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic-rv64" "target-features"="+64bit,+a,+c,+m,+relax,+zaamo,+zalrsc,+zmmul,-b,-d,-e,-experimental-sdext,-experimental-sdtrig,-experimental-smctr,-experimental-ssctr,-experimental-svukte,-experimental-xqcia,-experimental-xqciac,-experimental-xqcicli,-experimental-xqcicm,-experimental-xqcics,-experimental-xqcicsr,-experimental-xqciint,-experimental-xqcilo,-experimental-xqcilsm,-experimental-xqcisls,-experimental-zalasr,-experimental-zicfilp,-experimental-zicfiss,-experimental-zvbc32e,-experimental-zvkgs,-f,-h,-sha,-shcounterenw,-shgatpa,-shtvala,-shvsatpa,-shvstvala,-shvstvecd,-smaia,-smcdeleg,-smcsrind,-smdbltrp,-smepmp,-smmpm,-smnpm,-smrnmi,-smstateen,-ssaia,-ssccfg,-ssccptr,-sscofpmf,-sscounterenw,-sscsrind,-ssdbltrp,-ssnpm,-sspm,-ssqosid,-ssstateen,-ssstrict,-sstc,-sstvala,-sstvecd,-ssu64xl,-supm,-svade,-svadu,-svbare,-svinval,-svnapot,-svpbmt,-svvptc,-v,-xcvalu,-xcvbi,-xcvbitmanip,-xcvelw,-xcvmac,-xcvmem,-xcvsimd,-xmipscmove,-xmipslsp,-xsfcease,-xsfvcp,-xsfvfnrclipxfqf,-xsfvfwmaccqqq,-xsfvqmaccdod,-xsfvqmaccqoq,-xsifivecdiscarddlone,-xsifivecflushdlone,-xtheadba,-xtheadbb,-xtheadbs,-xtheadcmo,-xtheadcondmov,-xtheadfmemidx,-xtheadmac,-xtheadmemidx,-xtheadmempair,-xtheadsync,-xtheadvdot,-xventanacondops,-xwchc,-za128rs,-za64rs,-zabha,-zacas,-zama16b,-zawrs,-zba,-zbb,-zbc,-zbkb,-zbkc,-zbkx,-zbs,-zca,-zcb,-zcd,-zce,-zcf,-zcmop,-zcmp,-zcmt,-zdinx,-zfa,-zfbfmin,-zfh,-zfhmin,-zfinx,-zhinx,-zhinxmin,-zic64b,-zicbom,-zicbop,-zicboz,-ziccamoa,-ziccif,-zicclsm,-ziccrse,-zicntr,-zicond,-zicsr,-zifencei,-zihintntl,-zihintpause,-zihpm,-zimop,-zk,-zkn,-zknd,-zkne,-zknh,-zkr,-zks,-zksed,-zksh,-zkt,-ztso,-zvbb,-zvbc,-zve32f,-zve32x,-zve64d,-zve64f,-zve64x,-zvfbfmin,-zvfbfwma,-zvfh,-zvfhmin,-zvkb,-zvkg,-zvkn,-zvknc,-zvkned,-zvkng,-zvknha,-zvknhb,-zvks,-zvksc,-zvksed,-zvksg,-zvksh,-zvkt,-zvl1024b,-zvl128b,-zvl16384b,-zvl2048b,-zvl256b,-zvl32768b,-zvl32b,-zvl4096b,-zvl512b,-zvl64b,-zvl65536b,-zvl8192b" "uniform-work-group-size"="true" }
attributes #1 = { convergent mustprogress nofree nounwind willreturn memory(none) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic-rv64" "target-features"="+64bit,+a,+c,+m,+relax,+zaamo,+zalrsc,+zmmul,-b,-d,-e,-experimental-sdext,-experimental-sdtrig,-experimental-smctr,-experimental-ssctr,-experimental-svukte,-experimental-xqcia,-experimental-xqciac,-experimental-xqcicli,-experimental-xqcicm,-experimental-xqcics,-experimental-xqcicsr,-experimental-xqciint,-experimental-xqcilo,-experimental-xqcilsm,-experimental-xqcisls,-experimental-zalasr,-experimental-zicfilp,-experimental-zicfiss,-experimental-zvbc32e,-experimental-zvkgs,-f,-h,-sha,-shcounterenw,-shgatpa,-shtvala,-shvsatpa,-shvstvala,-shvstvecd,-smaia,-smcdeleg,-smcsrind,-smdbltrp,-smepmp,-smmpm,-smnpm,-smrnmi,-smstateen,-ssaia,-ssccfg,-ssccptr,-sscofpmf,-sscounterenw,-sscsrind,-ssdbltrp,-ssnpm,-sspm,-ssqosid,-ssstateen,-ssstrict,-sstc,-sstvala,-sstvecd,-ssu64xl,-supm,-svade,-svadu,-svbare,-svinval,-svnapot,-svpbmt,-svvptc,-v,-xcvalu,-xcvbi,-xcvbitmanip,-xcvelw,-xcvmac,-xcvmem,-xcvsimd,-xmipscmove,-xmipslsp,-xsfcease,-xsfvcp,-xsfvfnrclipxfqf,-xsfvfwmaccqqq,-xsfvqmaccdod,-xsfvqmaccqoq,-xsifivecdiscarddlone,-xsifivecflushdlone,-xtheadba,-xtheadbb,-xtheadbs,-xtheadcmo,-xtheadcondmov,-xtheadfmemidx,-xtheadmac,-xtheadmemidx,-xtheadmempair,-xtheadsync,-xtheadvdot,-xventanacondops,-xwchc,-za128rs,-za64rs,-zabha,-zacas,-zama16b,-zawrs,-zba,-zbb,-zbc,-zbkb,-zbkc,-zbkx,-zbs,-zca,-zcb,-zcd,-zce,-zcf,-zcmop,-zcmp,-zcmt,-zdinx,-zfa,-zfbfmin,-zfh,-zfhmin,-zfinx,-zhinx,-zhinxmin,-zic64b,-zicbom,-zicbop,-zicboz,-ziccamoa,-ziccif,-zicclsm,-ziccrse,-zicntr,-zicond,-zicsr,-zifencei,-zihintntl,-zihintpause,-zihpm,-zimop,-zk,-zkn,-zknd,-zkne,-zknh,-zkr,-zks,-zksed,-zksh,-zkt,-ztso,-zvbb,-zvbc,-zve32f,-zve32x,-zve64d,-zve64f,-zve64x,-zvfbfmin,-zvfbfwma,-zvfh,-zvfhmin,-zvkb,-zvkg,-zvkn,-zvknc,-zvkned,-zvkng,-zvknha,-zvknhb,-zvks,-zvksc,-zvksed,-zvksg,-zvksh,-zvkt,-zvl1024b,-zvl128b,-zvl16384b,-zvl2048b,-zvl256b,-zvl32768b,-zvl32b,-zvl4096b,-zvl512b,-zvl64b,-zvl65536b,-zvl8192b" }
attributes #2 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #3 = { convergent nounwind willreturn memory(none) }

!llvm.module.flags = !{!0, !1, !2, !4, !5}
!opencl.ocl.version = !{!6}
!llvm.ident = !{!7}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"target-abi", !"lp64"}
!2 = !{i32 6, !"riscv-isa", !3}
!3 = !{!"rv64i2p1_m2p0_a2p1_c2p0_zmmul1p0_zaamo1p0_zalrsc1p0"}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{i32 8, !"SmallDataLimit", i32 0}
!6 = !{i32 1, i32 2}
!7 = !{!"clang version 20.1.7 (https://github.com/ZhaoVenya/llvm-project.git 49cb60475ff465f379d055a7a459712a4a0f1d2a)"}
!8 = !{i32 1, i32 1, i32 1, i32 0, i32 0, i32 0}
!9 = !{!"none", !"none", !"none", !"none", !"none", !"none"}
!10 = !{!"float*", !"float*", !"float*", !"int", !"int", !"int"}
!11 = !{!"const", !"const", !"", !"", !"", !""}
!12 = !{!13, !13, i64 0}
!13 = !{!"float", !14, i64 0}
!14 = !{!"omnipotent char", !15, i64 0}
!15 = !{!"Simple C/C++ TBAA"}
