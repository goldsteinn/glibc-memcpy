#define USE_WITH_MULTIARCH_AND_LIBC
#define USE_WITH_ERMS
#define ALIGN_TO	32
#ifndef VZEROUPPER
# include "../evex-vecs.h"
#endif
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

	/* Avoid short distance rep movsb only with non- SSE vector.  */
#ifndef AVOID_SHORT_DISTANCE_REP_MOVSB
# define AVOID_SHORT_DISTANCE_REP_MOVSB	(VEC_SIZE	>	16)
#else
# define AVOID_SHORT_DISTANCE_REP_MOVSB	0
#endif


#ifndef SECTION
# error SECTION is not defined!
#endif

	.section SECTION(.text), "ax", @progbits
ENTRY(MEMCPY)
	movq	%rdi, %rax
	cmpq	$(VEC_SIZE), %rdx
	jbe	L(leq_1x)
	VMOVU	(%rsi), %VEC(0)
	cmpq	$(VEC_SIZE * 2), %rdx
	ja	L(gt_2x)
	VMOVU	(VEC_SIZE * -1)(%rsi, %rdx), %VEC(15)
	VMOVU	%VEC(0), (VEC_SIZE * 0)(%rdi)
	VMOVU	%VEC(15), (VEC_SIZE * -1)(%rdi, %rdx)
	VZEROUPPER_RETURN
	.p2align 4,, 6
L(leq_1x):
	cmpl	$8, %edx
	jb	L(copy_0_7)

	cmpl	$16, %edx
	jb	L(copy_8_15)
	/* Fall through for [16, 32] which is the most hot range in [0, 32].  */
	vmovdqu	(%rsi), %xmm0
	vmovdqu	-16(%rsi, %rdx), %xmm1
	vmovdqu	%xmm0, (%rdi)
	vmovdqu	%xmm1, -16(%rdi, %rdx)
	ret
	.p2align 4,, 6
L(copy_4_7):
	movl	(%rsi), %ecx
	movl	-4(%rsi, %rdx), %esi
	movl	%ecx, (%rdi)
	movl	%esi, -4(%rdi, %rdx)
	ret
L(copy_1):
	movzbl	(%rsi), %ecx
	movb	%cl, (%rdi)
	ret

	/* Colder copy case for [0, 7].  */
L(copy_0_7):
	cmpl	$4, %edx
	jae	L(copy_4_7)
	cmpl	$1, %edx
	je	L(copy_1)
	jb	L(copy_0)
	/* Fall through into copy [2, 3] as it is more common than [0, 1].  */
	movzwl	(%rsi), %ecx
	movzbl	-1(%rsi, %rdx), %esi
	movw	%cx, (%rdi)
	movb	%sil, -1(%rdi, %rdx)
L(copy_0):
	ret

	.p2align 4
L(copy_8_15):
	movq	(%rsi), %rcx
	movq	-8(%rsi, %rdx), %rsi
	movq	%rcx, (%rdi)
	movq	%rsi, -8(%rdi, %rdx)
	ret

L(gt_2x):
#if ALIGN_TO == 64
	VMOVU	(VEC_SIZE * 1)(%rsi), %VEC(1)
#endif
	cmpq	$(VEC_SIZE * 8), %rdx
	ja	L(gt_8x)
#if ALIGN_TO != 64
	VMOVU	(VEC_SIZE * 1)(%rsi), %VEC(1)
#endif
	VMOVU	(VEC_SIZE * -1)(%rsi, %rdx), %VEC(15)
	VMOVU	(VEC_SIZE * -2)(%rsi, %rdx), %VEC(14)

	cmpl	$(VEC_SIZE * 4), %edx
	jbe	L(copy_gt_2x_leq_4x)
	/* copy (4x, 8x].  */
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

	.p2align 4,, 6
L(copy_gt_2x_leq_4x):
	VMOVU	%VEC(0), (VEC_SIZE * 0)(%rdi)
	VMOVU	%VEC(1), (VEC_SIZE * 1)(%rdi)

	VMOVU	%VEC(15), (VEC_SIZE * -1)(%rdi, %rdx)
	VMOVU	%VEC(14), (VEC_SIZE * -2)(%rdi, %rdx)
L(nop):
	VZEROUPPER_RETURN
	.p2align 4
L(gt_8x):
	movq	%rdi, %rcx
	subq	%rsi, %rcx
	cmpq	%rdx, %rcx
	jb	L(gt_8x_backward)
#ifdef USE_WITH_MULTIARCH_AND_LIBC
# ifdef USE_WITH_ERMS
	cmpq	x86_rep_movsb_threshold(%rip), %rdx
	ja	L(movsb)
# else
	cmpq	x86_shared_non_temporal_threshold(%rip), %rdx
	ja	L(large_memcpy_2x)
# endif
#endif
L(gt_8x_forward):
	VMOVU	(VEC_SIZE * -1)(%rsi, %rdx), %VEC(15)
	VMOVU	(VEC_SIZE * -2)(%rsi, %rdx), %VEC(14)
	VMOVU	(VEC_SIZE * -3)(%rsi, %rdx), %VEC(13)
	VMOVU	(VEC_SIZE * -4)(%rsi, %rdx), %VEC(12)
	subq	%rdi, %rsi
	leaq	(VEC_SIZE * -4)(%rdi, %rdx), %rdx
	orq	$(ALIGN_TO - 1), %rdi
	leaq	1(%rdi, %rsi), %rsi
	incq	%rdi
	/* Loop 4x VEC at a time copy forward from src (rsi) to dst (rdi).  */
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

	VMOVU	%VEC(15), (VEC_SIZE * 3)(%rdx)
	VMOVU	%VEC(14), (VEC_SIZE * 2)(%rdx)
	VMOVU	%VEC(13), (VEC_SIZE * 1)(%rdx)
	VMOVU	%VEC(12), (VEC_SIZE * 0)(%rdx)


	VMOVU	%VEC(0), (VEC_SIZE * 0)(%rax)
#if ALIGN_TO == 64
	VMOVU	%VEC(1), (VEC_SIZE * 1)(%rax)
#endif

	VZEROUPPER_RETURN

	.p2align 4
L(gt_8x_backward):
	/* Load begining of region to support overlap.  */
	VMOVU	(VEC_SIZE * -1)(%rsi, %rdx), %VEC(15)

#if ALIGN_TO == 64
	VMOVU	(VEC_SIZE * -2)(%rsi, %rdx), %VEC(14)
#else
	VMOVU	(VEC_SIZE * 1)(%rsi), %VEC(1)
#endif
	VMOVU	(VEC_SIZE * 2)(%rsi), %VEC(2)
	VMOVU	(VEC_SIZE * 3)(%rsi), %VEC(3)

	/* Subtract dst (rdi) from src (rsi) before aligning dst (rdi) so that
	   src (rsi) can be properly adjusted.  */
	subq	%rdi, %rsi

	/* Set dst (rdi) as end of region. Set 4x VEC from begining of region as
	   those VECs where already loaded. -1 for aligning dst (rdi).  */
	leaq	(VEC_SIZE * -4 + -1)(%rdi, %rdx), %rdi
	/* Align dst (rdi).  */
	andq	$-(ALIGN_TO), %rdi
	/* Readjust src (rsi).  */
	addq	%rdi, %rsi
	/* Loop 4x VEC at a time copy backward from src (rsi) to dst (rdi).  */
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

	/* Store begining of region.  */

	/* NB: rax was set to dst on function entry.  */
	VMOVU	%VEC(3), (VEC_SIZE * 3)(%rax)
	VMOVU	%VEC(2), (VEC_SIZE * 2)(%rax)
	VMOVU	%VEC(1), (VEC_SIZE * 1)(%rax)
	VMOVU	%VEC(0), (VEC_SIZE * 0)(%rax)

	/* Store VECs loaded for aligning dst (rdi).  */
#if ALIGN_TO == 64
	VMOVU	%VEC(14), (VEC_SIZE * -2)(%rax, %rdx)
#endif
	VMOVU	%VEC(15), (VEC_SIZE * -1)(%rax, %rdx)
	VZEROUPPER_RETURN

#ifdef USE_WITH_MULTIARCH_AND_LIBC
# ifdef USE_WITH_ERMS
	.p2align 4
L(movsb):
	/* If size (rdx) > __x86_rep_movsb_stop_threshold go to large copy.  */

	/* NB: __x86_rep_movsb_stop_threshold in range
	   [__x86_rep_movsb_threshold, __x86_shared_non_temporal_threshold].  */
	cmp	x86_rep_movsb_stop_threshold(%rip), %RDX_LP
	jae	L(large_memcpy_2x)
#  if AVOID_SHORT_DISTANCE_REP_MOVSB
	// andl    $X86_STRING_CONTROL_AVOID_SHORT_DISTANCE_REP_MOVSB,
	// __x86_string_control(%rip)
	// jz      L(movsb_aligned)
	cmpq	$-64, %rcx
	jl	L(gt_8x_forward)
L(movsb_align):
#  endif
	/* 64 byte align both dst (rdi) and dst (rdi) + size (rdx). The
	   necessary VECs where already loaded.  */

	/* Subtract dst (rdi) from src (rsi) before aligning dst (rdi) so that
	   src (rsi) can be properly adjusted.  */
	subq	%rdi, %rsi
	/* -2 incase both dst (rdi) and dst (rdi) + size (rdx) where already 64
	   byte aligned.  */
	leaq	-2(%rdi, %rdx), %rcx
	/* Aligned dst (rdi).  */
	orq	$63, %rdi
	/* Restore src (rsi).  */
	leaq	1(%rdi, %rsi), %rsi
	/* Store proper size in rcx.  */
	subq	%rdi, %rcx
	andq	$-64, %rcx
	incq	%rdi

	rep	movsb

	/* Store aligning VECs.  */

	/* NB: rax was set to dst on function entry.  */
	VMOVU	%VEC(0), (VEC_SIZE * 0)(%rax)
	VMOVU	%VEC(1), (VEC_SIZE * 1)(%rax)

	VMOVU	%VEC(14), (VEC_SIZE * -2)(%rax, %rdx)
	VMOVU	%VEC(15), (VEC_SIZE * -1)(%rax, %rdx)
	ret
# endif
#endif
	.p2align 4
L(large_memcpy_2x):
	ret

	.section SECTION(.rodata), "aM", @progbits, 8
	.p2align 5
x86_rep_movsb_threshold:
	.quad	(2048 * 1 * ((VEC_SIZE) / 16))
x86_shared_non_temporal_threshold:
	.quad	((3 * (L3_CACHE_SIZE)) / ((NCORES) * 4))
x86_rep_movsb_stop_threshold:
	.quad	((3 * (L3_CACHE_SIZE)) / ((NCORES) * 4))
	.previous
END(MEMCPY)

