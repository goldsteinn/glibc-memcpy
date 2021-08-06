/*
    1. BPU (BHT and/or BPU)
    2. Uop Cache
    3. Uops Delivered
    4. Branch Density
    5. rep movsb upper bound
    6. address aliasing
*/

#include "asm-common.h"
#include "../evex-vecs.h"
	// Tigerlake/Icelake
#define L3_CACHE_SIZE	(12288	*	1024)
#define NCORES	8


#ifndef PAGE_SIZE
# define PAGE_SIZE	4096
#endif

#if PAGE_SIZE != 4096
# error Unsupported PAGE_SIZE
#endif


#ifndef SECTION
# error SECTION is not defined!
#endif



	.section SECTION(.text), "ax", @progbits
ENTRY(MEMCPY):
	movq	%rdi, %rax
	cmpq	$(VEC_SIZE * 1), %rdx
	jb	L(copy_0_31)

	VMOVU	(VEC_SIZE * 0)(%rsi), %VEC(0)
	VMOVU	(VEC_SIZE * -1)(%rsi, %rdx), %VEC(15)
	cmpq	$(VEC_SIZE * 2), %rdx
	ja	L(more_64)
	/* L(copy_32_64).  */

	VMOVU	%VEC(0), (VEC_SIZE * 0)(%rdi)
	VMOVU	%VEC(15), (VEC_SIZE * -1)(%rdi, %rdx)
	VZEROUPPER_RETURN

	.p2align 4
L(copy_0_7):
	cmpl	$4, %edx
	jae	L(copy_4_7)
	cmpl	$1, %edx
	je	L(copy_1)
	jb	L(copy_0)
	movzwl	(%rsi), %ecx
	movzbl	-1(%rsi, %rdx), %esi
	movw	%cx, (%rdi)
	movb	%sil, -1(%rdi, %rdx)
L(copy_0):
	ret
L(copy_1):
	movzbl	(%rsi), %ecx
	movb	%cl, (%rdi)
	ret

	.p2align 4
L(copy_4_7):
	movl	(%rsi), %ecx
	movl	-4(%rsi, %rdx), %esi
	movl	%ecx, (%rdi)
	movl	%esi, -4(%rdi, %rdx)
	ret

	.p2align 4
L(copy_8_15):
	movq	(%rsi), %rcx
	movq	-8(%rsi, %rdx), %rsi
	movq	%rcx, (%rdi)
	movq	%rsi, -8(%rdi, %rdx)
	ret

	.p2align 4
L(copy_0_31):
	cmpl	$8, %edx
	jb	L(copy_0_7)

	cmpl	$16, %edx
	jb	L(copy_8_15)

	vmovdqu	(%rsi), %xmm0
	vmovdqu	-16(%rsi, %rdx), %xmm1
	vmovdqu	%xmm0, (%rdi)
	vmovdqu	%xmm1, -16(%rdi, %rdx)
	ret

	NOP16
	.p2align 4
L(more_64):
	VMOVU	(VEC_SIZE * 1)(%rsi), %VEC(1)
	VMOVU	(VEC_SIZE * -2)(%rsi, %rdx), %VEC(14)
	cmpq	x86_rep_movsb_threshold(%rip), %rdx
	ja	L(movsb)

	cmpq	$(VEC_SIZE * 8), %rdx
	ja	L(more_256)

	cmpl	$(VEC_SIZE * 4), %edx
	jbe	L(copy_65_128)

	/* L(copy_129_256).  */
	VMOVU	(VEC_SIZE * 2)(%rsi), %VEC(2)
	VMOVU	(VEC_SIZE * 3)(%rsi), %VEC(3)

	VMOVU	(VEC_SIZE * -3)(%rsi, %rdx), %VEC(13)
	VMOVU	(VEC_SIZE * -4)(%rsi, %rdx), %VEC(12)


	VMOVU	%VEC(0), (VEC_SIZE * 0)(%rdi)
	VMOVU	%VEC(1), (VEC_SIZE * 1)(%rdi)
	VMOVU	%VEC(2), (VEC_SIZE * 2)(%rdi)
	VMOVU	%VEC(3), (VEC_SIZE * 3)(%rdi)

	VMOVU	%VEC(15), (VEC_SIZE * -1)(%rdi, %rdx)
	VMOVU	%VEC(14), (VEC_SIZE * -2)(%rdi, %rdx)
	VMOVU	%VEC(13), (VEC_SIZE * -3)(%rdi, %rdx)
	VMOVU	%VEC(12), (VEC_SIZE * -4)(%rdi, %rdx)
	VZEROUPPER_RETURN

	.p2align 4
L(copy_65_128):
	VMOVU	%VEC(0), (VEC_SIZE * 0)(%rdi)
	VMOVU	%VEC(1), (VEC_SIZE * 1)(%rdi)

	VMOVU	%VEC(15), (VEC_SIZE * -1)(%rdi, %rdx)
	VMOVU	%VEC(14), (VEC_SIZE * -2)(%rdi, %rdx)
L(nop):
	VZEROUPPER_RETURN

	NOP16
	.p2align 4
L(more_256):
	cmpq	%rsi, %rdi
	ja	L(more_256_backward)
	je	L(nop)
	VMOVU	(VEC_SIZE * -3)(%rsi, %rdx), %VEC(13)
	VMOVU	(VEC_SIZE * -4)(%rsi, %rdx), %VEC(12)


	subq	%rdi, %rsi
	leaq	(VEC_SIZE * -4)(%rdi, %rdx), %rdx
	orq	$(VEC_SIZE * 2 - 1), %rdi
	leaq	1(%rdi, %rsi), %rsi
	incq	%rdi

	.p2align 4
L(loop_4x_vec_forward):
	VMOVU	(VEC_SIZE * 0)(%rsi), %VEC(4)
	VMOVU	(VEC_SIZE * 1)(%rsi), %VEC(5)
	VMOVU	(VEC_SIZE * 2)(%rsi), %VEC(6)
	VMOVU	(VEC_SIZE * 3)(%rsi), %VEC(7)
	subq	$(VEC_SIZE * -4), %rsi
	VMOVA	%VEC(4), (VEC_SIZE * 0)(%rdi)
	VMOVA	%VEC(5), (VEC_SIZE * 1)(%rdi)
	VMOVA	%VEC(6), (VEC_SIZE * 2)(%rdi)
	VMOVA	%VEC(7), (VEC_SIZE * 3)(%rdi)

	subq	$(VEC_SIZE * -4), %rdi

	cmpq	%rdi, %rdx
	ja	L(loop_4x_vec_forward)

	VMOVU	%VEC(12), (VEC_SIZE * 0)(%rdx)
	VMOVU	%VEC(13), (VEC_SIZE * 1)(%rdx)
	VMOVU	%VEC(14), (VEC_SIZE * 2)(%rdx)
	VMOVU	%VEC(15), (VEC_SIZE * 3)(%rdx)

	VMOVU	%VEC(0), (VEC_SIZE * 0)(%rax)
	VMOVU	%VEC(1), (VEC_SIZE * 1)(%rax)
	VZEROUPPER_RETURN

	.p2align 4
	NOP16
L(more_256_movsb_entry):
	VMOVU	(VEC_SIZE * -1)(%rsi, %rdx), %VEC(15)
	VMOVU	(VEC_SIZE * -2)(%rsi, %rdx), %VEC(14)
L(more_256_backward):

	VMOVU	(VEC_SIZE * 2)(%rsi), %VEC(2)
	VMOVU	(VEC_SIZE * 3)(%rsi), %VEC(3)

	subq	%rdi, %rsi
	leaq	(VEC_SIZE * -4 + -1)(%rdi, %rdx), %rdi
	andq	$(VEC_SIZE * -2), %rdi
	addq	%rdi, %rsi

	.p2align 4
L(loop_4x_vec_backward):
	VMOVU	(VEC_SIZE * 3)(%rsi), %VEC(4)
	VMOVU	(VEC_SIZE * 2)(%rsi), %VEC(5)
	VMOVU	(VEC_SIZE * 1)(%rsi), %VEC(6)
	VMOVU	(VEC_SIZE * 0)(%rsi), %VEC(7)

	addq	$(VEC_SIZE * -4), %rsi

	VMOVA	%VEC(4), (VEC_SIZE * 3)(%rdi)
	VMOVA	%VEC(5), (VEC_SIZE * 2)(%rdi)
	VMOVA	%VEC(6), (VEC_SIZE * 1)(%rdi)
	VMOVA	%VEC(7), (VEC_SIZE * 0)(%rdi)
	addq	$(VEC_SIZE * -4), %rdi

	cmpq	%rdi, %rax
	jb	L(loop_4x_vec_backward)

	VMOVU	%VEC(3), (VEC_SIZE * 3)(%rax)
	VMOVU	%VEC(2), (VEC_SIZE * 2)(%rax)
	VMOVU	%VEC(1), (VEC_SIZE * 1)(%rax)
	VMOVU	%VEC(0), (VEC_SIZE * 0)(%rax)

	VMOVU	%VEC(14), (VEC_SIZE * -2)(%rax, %rdx)
	VMOVU	%VEC(15), (VEC_SIZE * -1)(%rax, %rdx)
	VZEROUPPER_RETURN

	.p2align 4
L(movsb):
#if 0
	// Have forward / backward align
	cmpq	x86_shared_non_temporal_threshold(%rip), %rdx
	jae	L(large_memcpy)
	movq	%rdi, %rcx
	subq	%rsi, %rcx
	jb	L(movsb_no_backward)
	je	L(nop_movsb)
	cmpq	%rcx, %rdx
	jb	L(more_256_backward_movsb_entry)
	negq	%rcx
L(movsb_no_backward):
	cmpl	$64, %ecx
	jb	L(more_256_no_movsb)


	subq	%rdi, %rsi
	leaq	-1(%rdi, %rdx), %rcx
	orq	$(VEC_SIZE * 2 - 1), %rdi
	leaq	1(%rdi, %rsi), %rsi
	subq	%rdi, %rcx
	incq	%rdi

	rep	movsb
	VMOVU	%VEC(0), (VEC_SIZE * 0)(%rax)
	VMOVU	%VEC(1), (VEC_SIZE * 1)(%rax)
L(nop_movsb):
#endif
	ret

L(large_memcpy):
	// ret


	.section SECTION(.rodata), "aM", @progbits, 8
	.p2align 5
x86_rep_movsb_threshold:
	.quad	(2048 * ((VEC_SIZE) / 16))
x86_shared_non_temporal_threshold:
	.quad	((3 * (L3_CACHE_SIZE)) / ((NCORES) * 4))
x86_rep_movsb_stop_threshold:
	.quad	((3 * (L3_CACHE_SIZE)) / ((NCORES) * 4))
	.previous
END(MEMCPY)
