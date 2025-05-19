	.attribute	4, 16
	.attribute	5, "rv32i2p1_m2p0_a2p1_f2p2_d2p2_c2p0_zicsr2p0_zmmul1p0_zaamo1p0_zalrsc1p0"
	.file	"main.c"
	.option	push
	.option	arch, +a, +c, +d, +f, +m, +zaamo, +zalrsc, +zicsr, +zmmul
	.text
	.globl	main                            # -- Begin function main
	.p2align	1
	.type	main,@function
main:                                   # @main
# %bb.0:                                # %entry
	addi	sp, sp, -16
	sw	ra, 12(sp)                      # 4-byte Folded Spill
	sw	s0, 8(sp)                       # 4-byte Folded Spill
	addi	s0, sp, 16
	sw	zero, -12(s0)
	li	a0, 42
	sw	a0, -16(s0)
	lw	a0, -16(s0)
	lw	ra, 12(sp)                      # 4-byte Folded Reload
	lw	s0, 8(sp)                       # 4-byte Folded Reload
	addi	sp, sp, 16
	ret
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
                                        # -- End function
	.option	pop
	.ident	"clang version 20.1.5 (https://github.com/ZhaoVenya/llvm-project.git 1c3aea36db21639c43349127be2b34455b3b1984)"
	.section	".note.GNU-stack","",@progbits
