	.text
	.attribute	4, 16
	.attribute	5, "rv32i2p1"
	.file	"main.c"
	.globl	test                            # -- Begin function test
	.p2align	2
	.type	test,@function
test:                                   # @test
# %bb.0:                                # %entry
	addi	sp, sp, -32
	sw	ra, 28(sp)                      # 4-byte Folded Spill
	sw	s0, 24(sp)                      # 4-byte Folded Spill
	addi	s0, sp, 32
	li	a0, 170
	sw	a0, -12(s0)
	li	a0, 12
	sw	a0, -16(s0)
	lw	a0, -12(s0)
	lw	a1, -16(s0)
	#APP
	bsw	a0, a0, a1
	#NO_APP
	sw	a0, -20(s0)
	lw	a0, -20(s0)
	lw	ra, 28(sp)                      # 4-byte Folded Reload
	lw	s0, 24(sp)                      # 4-byte Folded Reload
	addi	sp, sp, 32
	ret
.Lfunc_end0:
	.size	test, .Lfunc_end0-test
                                        # -- End function
	.globl	main                            # -- Begin function main
	.p2align	2
	.type	main,@function
main:                                   # @main
# %bb.0:                                # %entry
	addi	sp, sp, -16
	sw	ra, 12(sp)                      # 4-byte Folded Spill
	sw	s0, 8(sp)                       # 4-byte Folded Spill
	addi	s0, sp, 16
	li	a0, 0
	sw	a0, -12(s0)
	call	test
	sw	a0, -16(s0)
	lw	a0, -16(s0)
	lw	ra, 12(sp)                      # 4-byte Folded Reload
	lw	s0, 8(sp)                       # 4-byte Folded Reload
	addi	sp, sp, 16
	ret
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
                                        # -- End function
	.ident	"clang version 20.0.0git (https://github.com/ZhaoVenya/llvm-project.git 33117f492c1c3463d5bd022e208d338823741fb7)"
	.section	".note.GNU-stack","",@progbits
