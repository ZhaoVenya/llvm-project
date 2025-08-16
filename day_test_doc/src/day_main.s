	.file	"main.c"
	.text
	.globl	myadd                           ; -- Begin function myadd
	.type	myadd,@function
myadd:                                  ; @myadd
; %bb.0:                                ; %entry
	addi	sp, sp, -16
	sw	a0, sp(12)
	sw	a1, sp(8)
	lw	t0, sp(12)
	lw	t1, sp(8)
	add	t0, t0, t1
	sw	t0, sp(4)
	lw	a0, sp(4)
	addi	sp, sp, 16
	ret
.Lfunc_end0:
	.size	myadd, .Lfunc_end0-myadd
                                        ; -- End function
	.globl	main                            ; -- Begin function main
	.type	main,@function
main:                                   ; @main
; %bb.0:                                ; %entry
	addi	sp, sp, -24
	sw	ra, sp(20)                      ; 4-byte Folded Spill
	addi	t0, zero, 0
	sw	t0, sp(16)
	addi	t0, zero, 2
	sw	t0, sp(12)
	addi	t0, zero, 1
	sw	t0, sp(8)
	lw	t0, sp(12)
	lw	t1, sp(8)
	add	t0, t0, t1
	sw	t0, sp(12)
	lw	a0, sp(12)
	lw	a1, sp(8)
	jalr	ra, myadd, 0
	sw	a0, sp(4)
	lw	a0, sp(4)
	lw	ra, sp(20)                      ; 4-byte Folded Reload
	addi	sp, sp, 24
	ret
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
                                        ; -- End function
	.ident	"clang version 20.1.7 (https://github.com/ZhaoVenya/llvm-project.git 82a634b7671f0d0861631511ea2f471959ea3d59)"
	.section	".note.GNU-stack","",@progbits
