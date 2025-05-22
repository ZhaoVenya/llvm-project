	.text
	.file	"main.c"
	.globl	test                            # -- Begin function test
	.type	test,@function
test:                                   # @test
# %bb.0:                                # %entry
	addi	sp, sp, -64
	li	a0, 7
	sw	a0, 60(sp)
	lw	a0, 60(sp)
	li	a1, 5
	add	a0, a0, a1
	sw	a0, 56(sp)
	lw	a0, 60(sp)
	li	a1, -1
	add	a0, a0, a1
	sw	a0, 52(sp)
	lw	a0, 56(sp)
	lw	a1, 52(sp)
	add	a0, a0, a1
	sw	a0, 48(sp)
	lw	a0, 56(sp)
	lw	a1, 52(sp)
	mul	a0, a0, a1
	sw	a0, 44(sp)
	lw	a0, 56(sp)
	lw	a1, 52(sp)
	div	a0, a0, a1
	sw	a0, 40(sp)
	lw	a0, 56(sp)
	lw	a1, 52(sp)
	rem	a0, a0, a1
	sw	a0, 36(sp)
	lw	a0, 56(sp)
	li	a1, 1
	and	a0, a0, a1
	sw	a0, 32(sp)
	lw	a0, 56(sp)
	or	a0, a0, a1
	sw	a0, 28(sp)
	lw	a0, 56(sp)
	li	a2, 3
	xor	a0, a0, a2
	sw	a0, 24(sp)
	lw	a0, 56(sp)
	lw	a3, 52(sp)
	xor	a0, a0, a3
	sw	a0, 20(sp)
	lw	a0, 20(sp)
	sll	a0, a0, a2
	sw	a0, 20(sp)
	lw	a0, 20(sp)
	sra	a0, a0, a2
	sw	a0, 20(sp)
	lw	a0, 20(sp)
	lw	a2, 60(sp)
	sll	a0, a0, a2
	sw	a0, 20(sp)
	lw	a0, 20(sp)
	lw	a2, 60(sp)
	sra	a0, a0, a2
	sw	a0, 20(sp)
	lw	a0, 56(sp)
	lw	a2, 52(sp)
	and	a0, a0, a2
	lw	a2, 48(sp)
	and	a0, a0, a2
	sw	a0, 16(sp)
	lw	a0, 56(sp)
	lw	a2, 52(sp)
	or	a0, a0, a2
	or	a0, a0, a1
	sw	a0, 12(sp)
	lw	a0, 60(sp)
	lw	a1, 56(sp)
	add	a0, a0, a1
	lw	a1, 52(sp)
	add	a0, a0, a1
	lw	a1, 48(sp)
	add	a0, a0, a1
	lw	a1, 44(sp)
	add	a0, a0, a1
	lw	a1, 40(sp)
	add	a0, a0, a1
	lw	a1, 36(sp)
	add	a0, a0, a1
	lw	a1, 32(sp)
	add	a0, a0, a1
	lw	a1, 28(sp)
	add	a0, a0, a1
	lw	a1, 24(sp)
	add	a0, a0, a1
	lw	a1, 20(sp)
	add	a0, a0, a1
	lw	a1, 16(sp)
	add	a0, a0, a1
	lw	a1, 12(sp)
	add	a0, a0, a1
	addi	sp, sp, 64
	ret
.Lfunc_end0:
	.size	test, .Lfunc_end0-test
                                        # -- End function
	.globl	main                            # -- Begin function main
	.type	main,@function
main:                                   # @main
# %bb.0:                                # %entry
	addi	sp, sp, -16
	sw	ra, 12(sp)
	li	a0, 0
	sw	a0, 8(sp)
	lui 	a0, %hi(test)
	addi	a0, a0, %lo(test)
	jalr ra, a0, 0
	sw	a0, 4(sp)
	lw	a0, 4(sp)
	lw	ra, 12(sp)
	addi	sp, sp, 16
	ret
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
                                        # -- End function
	.ident	"clang version 20.0.0git (https://github.com/ZhaoVenya/llvm-project.git f2b6c7b146abdcd935c54bd37a35638f4bbd122e)"
	.section	".note.GNU-stack","",@progbits
