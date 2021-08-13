/*
    1. BPU (BHT and/or BPU)
    2. Uop Cache
    3. Uops Delivered
    4. Branch Density
    5. rep movsb upper bound
    6. address aliasing
    7. Exact organization of v12???????????????????????
*/

#include "asm-common.h"
#include "../avx-vecs.h"
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
ENTRY(MEMCPY)
	movq	%rdi, %rax
	cmpq	$(VEC_SIZE * 1), %rdx
	jb	L(less_vec)

	VMOVU	(VEC_SIZE * 0)(%rsi), %VEC(0)
	VMOVU	(VEC_SIZE * -1)(%rsi, %rdx), %VEC(15)
	cmpq	$(VEC_SIZE * 2), %rdx
	ja	L(more_2x)
	/* L(copy_32_64).  */

	VMOVU	%VEC(0), (VEC_SIZE * 0)(%rdi)
	VMOVU	%VEC(15), (VEC_SIZE * -1)(%rdi, %rdx)
	VZEROUPPER_RETURN
L(copy_1):
	movzbl	(%rsi), %ecx
	movb	%cl, (%rdi)
	ret
    
	.p2align 4
L(copy_8_15):
	movq	(%rsi), %rcx
	movq	-8(%rsi, %rdx), %rsi
	movq	%rcx, (%rdi)
	movq	%rsi, -8(%rdi, %rdx)
	ret

    .p2align 4
L(copy_4_7):
	movl	(%rsi), %ecx
	movl	-4(%rsi, %rdx), %esi
	movl	%ecx, (%rdi)
	movl	%esi, -4(%rdi, %rdx)
	ret
    .p2align 4    
L(less_vec):
	cmpl	$8, %edx
	jae	L(copy_8_15)
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

    .p2align 4
L(more_2x):
	VMOVU	(VEC_SIZE * 1)(%rsi), %VEC(1)
	VMOVU	(VEC_SIZE * -2)(%rsi, %rdx), %VEC(14)
	cmpq	$(VEC_SIZE * 4), %rdx
	jbe	L(copy_leq_4x)
	VMOVU	(VEC_SIZE * -3)(%rsi, %rdx), %VEC(13)
	VMOVU	(VEC_SIZE * -4)(%rsi, %rdx), %VEC(12)
    
	VMOVU	(VEC_SIZE * 2)(%rsi), %VEC(2)
	VMOVU	(VEC_SIZE * 3)(%rsi), %VEC(3)
	cmpq	$(VEC_SIZE * 8), %rdx
	ja	L(more_8x)
	VMOVU	%VEC(0), (VEC_SIZE * 0)(%rdi)
	VMOVU	%VEC(1), (VEC_SIZE * 1)(%rdi)
	VMOVU	%VEC(2), (VEC_SIZE * 2)(%rdi)
	VMOVU	%VEC(3), (VEC_SIZE * 3)(%rdi)

	VMOVU	%VEC(15), (VEC_SIZE * -1)(%rdi, %rdx)
	VMOVU	%VEC(14), (VEC_SIZE * -2)(%rdi, %rdx)
	VMOVU	%VEC(13), (VEC_SIZE * -3)(%rdi, %rdx)
	VMOVU	%VEC(12), (VEC_SIZE * -4)(%rdi, %rdx)
	VZEROUPPER_RETURN
	// 0x960
	.p2align 4
L(copy_leq_4x):
	VMOVU	%VEC(0), (VEC_SIZE * 0)(%rdi)
	VMOVU	%VEC(1), (VEC_SIZE * 1)(%rdi)

	VMOVU	%VEC(15), (VEC_SIZE * -1)(%rdi, %rdx)
	VMOVU	%VEC(14), (VEC_SIZE * -2)(%rdi, %rdx)
L(nop):
	VZEROUPPER_RETURN
	// 0x980
	.p2align 4
L(more_8x):
	cmpq	x86_rep_movsb_threshold(%rip), %rdx
	ja	L(movsb)
	NOP3
	cmpq	%rsi, %rdi
	ja	L(more_8x_backward)
	je	L(nop)

	subq	%rdi, %rsi
	leaq	(VEC_SIZE * -4)(%rdi, %rdx), %rdx
	/* Cache align.  */
	orq	$63, %rdi
	leaq	1(%rdi, %rsi), %rsi
	incq	%rdi
	// 0x9d0

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
    VMOVU	%VEC(2), (VEC_SIZE * 2)(%rax)
	VMOVU	%VEC(3), (VEC_SIZE * 3)(%rax)
	VZEROUPPER_RETURN

	// 0xa60
	.p2align 4
L(more_8x_backward):
	subq	%rdi, %rsi
	leaq	(VEC_SIZE * -4 + -1)(%rdi, %rdx), %rdi
	andq	$-64, %rdi
	addq	%rdi, %rsi
	.p2align 4
	// 0xa80
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

    VMOVU	%VEC(12), (VEC_SIZE * -4)(%rax, %rdx)
	VMOVU	%VEC(13), (VEC_SIZE * -3)(%rax, %rdx)
	VMOVU	%VEC(14), (VEC_SIZE * -2)(%rax, %rdx)
	VMOVU	%VEC(15), (VEC_SIZE * -1)(%rax, %rdx)
	VZEROUPPER_RETURN

	.p2align 4
L(movsb):
	cmp	x86_rep_movsb_stop_threshold(%rip), %RDX_LP
	jae	L(large_memcpy)
	movq	%rdi, %rcx
	subq	%rsi, %rcx
	cmpq	%rdx, %rcx
	jb	L(more_8x_backward)

	/* L(forward_movsb).  */
	subq	%rdi, %rsi
	leaq	-2(%rdi, %rdx), %rcx
	orq	$63, %rdi
	leaq	1(%rdi, %rsi), %rsi
	subq	%rdi, %rcx
	andq	$-64, %rcx
	incq	%rdi

	rep	movsb

	VMOVU	%VEC(0), (VEC_SIZE * 0)(%rax)
	VMOVU	%VEC(1), (VEC_SIZE * 1)(%rax)
    VMOVU	%VEC(2), (VEC_SIZE * 2)(%rax)
	VMOVU	%VEC(3), (VEC_SIZE * 3)(%rax)
    
    VMOVU	%VEC(12), (VEC_SIZE * -4)(%rax, %rdx)
	VMOVU	%VEC(13), (VEC_SIZE * -3)(%rax, %rdx)
	VMOVU	%VEC(14), (VEC_SIZE * -2)(%rax, %rdx)
	VMOVU	%VEC(15), (VEC_SIZE * -1)(%rax, %rdx)
	ret

L(large_memcpy):
	cmpq	x86_shared_non_temporal_threshold(%rip), %rdx
	jb	L(more_8x)
	ret

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
