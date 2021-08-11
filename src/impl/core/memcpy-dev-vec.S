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

	/* COPY_32 only used if evex is available for VEC_SIZE == 64.  */
#define COPY_32	\
	vmovdqu64 (%rsi), %ymm16; \
	vmovdqu64 -16(%rsi, %rdx), %ymm16; \
	vmovdqu64 %ymm16, (%rdi); \
	vmovdqu64 %ymm16, -16(%rdi, %rdx)

#define COPY_16	\
	vmovdqu	(%rsi), %xmm0; \
	vmovdqu	-16(%rsi, %rdx), %xmm1; \
	vmovdqu	%xmm0, (%rdi); \
	vmovdqu	%xmm1, -16(%rdi, %rdx)

#define COPY_8	\
	movq	(%rsi), %rcx; \
	movq	-8(%rsi, %rdx), %rsi; \
	movq	%rcx, (%rdi); \
	movq	%rsi, -8(%rdi, %rdx)

#define COPY_4	\
	movl	(%rsi), %ecx; \
	movl	-4(%rsi, %rdx), %esi; \
	movl	%ecx, (%rdi); \
	movl	%esi, -4(%rdi, %rdx)

#define COPY_2	\
	movzwl	(%rsi), %ecx; \
	movzbl	-1(%rsi, %rdx), %esi; \
	movw	%cx, (%rdi); \
	movb	%sil, -1(%rdi, %rdx)

#define COPY_1	\
	movzbl	(%rsi), %ecx; \
	movb	%cl, (%rdi)

	.section SECTION(.text), "ax", @progbits
ENTRY(MEMCPY)
	movq	%rdi, %rax
	cmpq	$(VEC_SIZE * 1), %rdx
	jb	L(less_1x)

	VMOVU	(VEC_SIZE * 0)(%rsi), %VEC(0)

	cmpq	$(VEC_SIZE * 2), %rdx
	ja	L(more_2x)
	/* [1x, 2x].  */
	VMOVU	(VEC_SIZE * -1)(%rsi, %rdx), %VEC(15)
	VMOVU	%VEC(0), (VEC_SIZE * 0)(%rdi)
	VMOVU	%VEC(15), (VEC_SIZE * -1)(%rdi, %rdx)
	VZEROUPPER_RETURN

	.p2align 4
L(less_quarter_x):
#if VEC_SIZE == 64
	cmpl	$8, %edx
	jae	L(copy_8_15)
#endif
#if VEC_SIZE >= 32
	cmpl	$4, %edx
	jae	L(copy_4_7)
#endif
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
    
#if VEC_SIZE >= 64
	.p2align 4
L(copy_8_15):
	movq	(%rsi), %rcx
	movq	-8(%rsi, %rdx), %rsi
	movq	%rcx, (%rdi)
	movq	%rsi, -8(%rdi, %rdx)
	ret
#endif
#if VEC_SIZE >= 32
L(copy_4_7):
	movl	(%rsi), %ecx
	movl	-4(%rsi, %rdx), %esi
	movl	%ecx, (%rdi)
	movl	%esi, -4(%rdi, %rdx)
	ret
#endif

	.p2align 4
L(copy_half_x):
#if VEC_SIZE == 64
	COPY_16
#elif VEC_SIZE == 32
	COPY_8
#elif VEC_SIZE == 16
	COPY_4
#else
# error Unsupported VEC_SIZE
#endif
	ret

L(less_1x):
	cmpl	$(VEC_SIZE / 4), %edx
	jb	L(less_quarter_x)

	cmpl	$(VEC_SIZE / 2), %edx
	jb	L(copy_half_x)

#if VEC_SIZE == 64
	COPY_32
#elif VEC_SIZE == 32
	COPY_16
#elif VEC_SIZE == 16
	COPY_8
#else
# error Unsupported VEC_SIZE
#endif
	ret

	// 0x8d0

	.p2align 4
L(more_2x):
	VMOVU	(VEC_SIZE * 1)(%rsi), %VEC(1)
	VMOVU	(VEC_SIZE * -1)(%rsi, %rdx), %VEC(15)
	VMOVU	(VEC_SIZE * -2)(%rsi, %rdx), %VEC(14)
	cmpq	$(VEC_SIZE * 8), %rdx
	ja	L(more_8x)
	cmpl	$(VEC_SIZE * 4), %edx
	jbe	L(copy_2x_4x)

	/* [4x + 1, 8x].  */
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
	// 0x960
	.p2align 4
L(copy_2x_4x):
	VMOVU	%VEC(0), (VEC_SIZE * 0)(%rdi)
	VMOVU	%VEC(1), (VEC_SIZE * 1)(%rdi)

	VMOVU	%VEC(15), (VEC_SIZE * -1)(%rdi, %rdx)
	VMOVU	%VEC(14), (VEC_SIZE * -2)(%rdi, %rdx)
L(nop):
	VZEROUPPER_RETURN

	// NOP16
	// 0x980
	NOP32
	.p2align 4
L(more_8x):
	cmpq	x86_rep_movsb_threshold(%rip), %rdx
	ja	L(movsb)
	NOP3
	cmpq	%rsi, %rdi
	ja	L(more_8x_backward)
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

#ifdef USE_AS_AVX2
	NOP32
#else
	NOP16
#endif
	.p2align 4
L(more_8x_backward):
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
	cmp	x86_rep_movsb_stop_threshold(%rip), %RDX_LP
	jae	L(large_memcpy)
	movq	%rdi, %rcx
	subq	%rsi, %rcx
	cmpq	%rdx, %rcx
	jb	L(more_8x_backward)

	/* L(forward_movsb).  */
	subq	%rdi, %rsi
	leaq	-2(%rdi, %rdx), %rcx
	orq	$(VEC_SIZE * 2 - 1), %rdi
	leaq	1(%rdi, %rsi), %rsi
	subq	%rdi, %rcx
	andq	$(VEC_SIZE * - 2), %rcx
	incq	%rdi

	rep	movsb

	VMOVU	%VEC(0), (VEC_SIZE * 0)(%rax)
	VMOVU	%VEC(1), (VEC_SIZE * 1)(%rax)

	VMOVU	%VEC(14), (VEC_SIZE * -2)(%rax, %rdx)
	VMOVU	%VEC(15), (VEC_SIZE * -1)(%rax, %rdx)
	ret

L(large_memcpy):
	cmpq	x86_shared_non_temporal_threshold(%rip), %rdx
	jb	L(more_256)
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
