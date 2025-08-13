	.file	"main.c"
	.text
	.globl	main                            ; -- Begin function main
	.type	main,@function
main:                                   ; @main
; %bb.0:                                ; %entry
	addi	r5, r0, 0
	sw	r5, r2(12)
	addi	r5, r0, 2
	sw	r5, r2(8)
	addi	r5, r0, 1
	sw	r5, r2(4)
	lw	r5, r2(8)
	lw	r6, r2(4)
	add	r5, r5, r6
	sw	r5, r2(8)
	addi	r24, r0, 10
	ret
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
                                        ; -- End function
	.ident	"clang version 20.1.7 (https://github.com/ZhaoVenya/llvm-project.git 3d1f79fb54d9443db253b0f7509e7a5ca03c60ce)"
	.section	".note.GNU-stack","",@progbits
