#include "asm-common.h"

#define USE_WITH_MULTIARCH_AND_LIBC
#define USE_WITH_ERMS
#ifndef VZEROUPPER
# include "../evex-vecs.h"
#endif

	// Tigerlake
#define L3_CACHE_SIZE	(12288	*	1024)
#define NCORES	8

#ifndef XMM0
# define XMM0	xmm0
#endif

#ifndef YMM0
# define YMM0	ymm0
#endif

#ifndef VZEROUPPER
# if VEC_SIZE > 16
#  define VZEROUPPER	vzeroupper
# else
#  define VZEROUPPER
# endif
#endif

#ifndef PAGE_SIZE
# define PAGE_SIZE	4096
#endif

#if PAGE_SIZE != 4096
# error Unsupported PAGE_SIZE
#endif

#ifndef LOG_PAGE_SIZE
# define LOG_PAGE_SIZE	12
#endif

#if PAGE_SIZE != (1 << LOG_PAGE_SIZE)
# error Invalid LOG_PAGE_SIZE
#endif

	/* Byte per page for large_memcpy inner loop.  */
#if VEC_SIZE == 64
# define LARGE_LOAD_SIZE	(VEC_SIZE	*	2)
#else
# define LARGE_LOAD_SIZE	(VEC_SIZE	*	4)
#endif

	/* Amount to shift rdx by to compare for memcpy_large_4x.  */
#ifndef LOG_4X_MEMCPY_THRESH
# define LOG_4X_MEMCPY_THRESH	4
#endif

	/* Avoid short distance rep movsb only with non- SSE vector.  */
#ifndef AVOID_SHORT_DISTANCE_REP_MOVSB
# define AVOID_SHORT_DISTANCE_REP_MOVSB	(VEC_SIZE	>	16)
#else
# define AVOID_SHORT_DISTANCE_REP_MOVSB	0
#endif

#ifndef PREFETCH
# define PREFETCH(addr)	prefetcht0	addr
#endif

	/* Assume 64-byte prefetch size.  */
#ifndef PREFETCH_SIZE
# define PREFETCH_SIZE	64
#endif

#define PREFETCHED_LOAD_SIZE	(VEC_SIZE	*	4)

#if PREFETCH_SIZE == 64
# if PREFETCHED_LOAD_SIZE == PREFETCH_SIZE
#  define PREFETCH_ONE_SET(dir,	base,	offset)	\
	PREFETCH ((offset)base)
# elif PREFETCHED_LOAD_SIZE == 2 * PREFETCH_SIZE
#  define PREFETCH_ONE_SET(dir,	base,	offset)	\
	PREFETCH ((offset)base); \
	PREFETCH ((offset + dir * PREFETCH_SIZE)base)
# elif PREFETCHED_LOAD_SIZE == 4 * PREFETCH_SIZE
#  define PREFETCH_ONE_SET(dir,	base,	offset)	\
	PREFETCH ((offset)base); \
	PREFETCH ((offset + dir * PREFETCH_SIZE)base); \
	PREFETCH ((offset + dir * PREFETCH_SIZE * 2)base); \
	PREFETCH ((offset + dir * PREFETCH_SIZE * 3)base)
# else
#  error Unsupported PREFETCHED_LOAD_SIZE!
# endif
#else
# error Unsupported PREFETCH_SIZE!
#endif

#if LARGE_LOAD_SIZE == (VEC_SIZE * 2)
# define LOAD_ONE_SET(base,	offset,	vec0,	vec1,	...)	\
	VMOVU	(offset)base, vec0; \
	VMOVU	((offset) + VEC_SIZE)base, vec1;
# define STORE_ONE_SET(base,	offset,	vec0,	vec1,	...)	\
	VMOVNT	vec0, (offset)base; \
	VMOVNT	vec1, ((offset) + VEC_SIZE)base;
#elif LARGE_LOAD_SIZE == (VEC_SIZE * 4)
# define LOAD_ONE_SET(base,	offset,	vec0,	vec1,	vec2,	vec3)	\
	VMOVU	(offset)base, vec0; \
	VMOVU	((offset) + VEC_SIZE)base, vec1; \
	VMOVU	((offset) + VEC_SIZE * 2)base, vec2; \
	VMOVU	((offset) + VEC_SIZE * 3)base, vec3;
# define STORE_ONE_SET(base,	offset,	vec0,	vec1,	vec2,	vec3)	\
	VMOVNT	vec0, (offset)base; \
	VMOVNT	vec1, ((offset) + VEC_SIZE)base; \
	VMOVNT	vec2, ((offset) + VEC_SIZE * 2)base; \
	VMOVNT	vec3, ((offset) + VEC_SIZE * 3)base;
#else
# error Invalid LARGE_LOAD_SIZE
#endif

#ifndef SECTION
# error SECTION is not defined!
#endif

	.section SECTION(.text), "ax", @progbits
ENTRY(MEMCPY)
	movq	%rdi, %rax
L(start_erms):
#ifdef __ILP32__
	/* Clear the upper 32 bits.  */
	movl	%edx, %edx
#endif
	cmp	$VEC_SIZE, %RDX_LP
	jb	L(less_vec)
	VMOVU	(%rsi), %VEC(0)
    VMOVU	-VEC_SIZE(%rsi, %rdx), %VEC(15)
	cmp	$(VEC_SIZE * 2), %RDX_LP
    //ja	L(movsb_more_2x_vec)
    ja	L(more_2x_vec)
//    ja  L(more_8x_vec)
L(last_2x_vec):
	/* From VEC and to 2 * VEC.  No branch when size == VEC_SIZE.  */

	VMOVU	%VEC(0), (%rdi)
	VMOVU	%VEC(15), -VEC_SIZE(%rdi, %rdx)
L(return):
#if VEC_SIZE > 16
	ZERO_UPPER_VEC_REGISTERS_RETURN
#else
	ret
#endif

	.p2align 4,, 6
L(less_vec):
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

//    .p2align 4
#if defined USE_WITH_ERMS && defined USE_WITH_MULTIARCH_AND_LIBC
L(movsb_more_2x_vec):
	cmp	x86_shared_non_temporal_threshold(%rip), %RDX_LP
	ja	L(large_memcpy_2x)
//    NOP13
#endif
L(more_2x_vec):
	/* More than 2 * VEC and there may be overlap between destination and
	   source.  */
	cmpq	$(VEC_SIZE * 8), %rdx
	ja	L(more_8x_vec)
	cmpq	$(VEC_SIZE * 4), %rdx
	jbe	L(last_4x_vec)
	/* Copy from 4 * VEC + 1 to 8 * VEC, inclusively.  */
	VMOVU	VEC_SIZE(%rsi), %VEC(1)
	VMOVU	(VEC_SIZE * 2)(%rsi), %VEC(2)
	VMOVU	(VEC_SIZE * 3)(%rsi), %VEC(3)
	VMOVU	-(VEC_SIZE * 2)(%rsi, %rdx), %VEC(14)
	VMOVU	-(VEC_SIZE * 3)(%rsi, %rdx), %VEC(13)
	VMOVU	-(VEC_SIZE * 4)(%rsi, %rdx), %VEC(12)
	VMOVU	%VEC(0), (%rdi)
	VMOVU	%VEC(1), VEC_SIZE(%rdi)
	VMOVU	%VEC(2), (VEC_SIZE * 2)(%rdi)
	VMOVU	%VEC(3), (VEC_SIZE * 3)(%rdi)
	VMOVU	%VEC(15), -VEC_SIZE(%rdi, %rdx)
	VMOVU	%VEC(14), -(VEC_SIZE * 2)(%rdi, %rdx)
	VMOVU	%VEC(13), -(VEC_SIZE * 3)(%rdi, %rdx)
	VMOVU	%VEC(12), -(VEC_SIZE * 4)(%rdi, %rdx)
	VZEROUPPER_RETURN
L(last_4x_vec):
	/* Copy from 2 * VEC + 1 to 4 * VEC, inclusively.  */
	VMOVU	VEC_SIZE(%rsi), %VEC(1)
	VMOVU	-(VEC_SIZE * 2)(%rsi, %rdx), %VEC(14)
	VMOVU	%VEC(0), (%rdi)
	VMOVU	%VEC(1), VEC_SIZE(%rdi)
	VMOVU	%VEC(15), -VEC_SIZE(%rdi, %rdx)
	VMOVU	%VEC(14), -(VEC_SIZE * 2)(%rdi, %rdx)
	VZEROUPPER_RETURN

	.p2align 4
L(more_8x_vec):
	/* Check if non-temporal move candidate.  */
#ifdef USE_WITH_MULTIARCH_AND_LIBC
	/* Check non-temporal store threshold.  */
	cmp	x86_rep_movsb_threshold(%rip), %RDX_LP
	ja	L(movsb)

#endif
	/* Entry if rdx is greater than non-temporal threshold but there is
	   overlap.  */
L(more_8x_vec_check):
	cmpq	%rsi, %rdi
	ja	L(more_8x_vec_backward)
	/* Source == destination is less common.  */
	je	L(nop)
	/* Load the first VEC and last 4 * VEC to support overlapping addresses.
	 */
	VMOVU	-(VEC_SIZE * 2)(%rsi, %rdx), %VEC(14)
	VMOVU	-(VEC_SIZE * 3)(%rsi, %rdx), %VEC(13)
	VMOVU	-(VEC_SIZE * 4)(%rsi, %rdx), %VEC(12)
	/* Save start and stop of the destination buffer.  */
	movq	%rdi, %r11
	leaq	-VEC_SIZE(%rdi, %rdx), %rcx
	/* Align destination for aligned stores in the loop.  Compute how much
	   destination is misaligned.  */
	movq	%rdi, %r8
	andq	$(VEC_SIZE - 1), %r8
	/* Get the negative of offset for alignment.  */
	subq	$VEC_SIZE, %r8
	/* Adjust source.  */
	subq	%r8, %rsi
	/* Adjust destination which should be aligned now.  */
	subq	%r8, %rdi
	/* Adjust length.  */
	addq	%r8, %rdx

	.p2align 4
L(loop_4x_vec_forward):
	/* Copy 4 * VEC a time forward.  */
	VMOVU	(%rsi), %VEC(4)
	VMOVU	VEC_SIZE(%rsi), %VEC(5)
	VMOVU	(VEC_SIZE * 2)(%rsi), %VEC(6)
	VMOVU	(VEC_SIZE * 3)(%rsi), %VEC(7)
	subq	$-(VEC_SIZE * 4), %rsi
	addq	$-(VEC_SIZE * 4), %rdx
	VMOVA	%VEC(4), (%rdi)
	VMOVA	%VEC(5), VEC_SIZE(%rdi)
	VMOVA	%VEC(6), (VEC_SIZE * 2)(%rdi)
	VMOVA	%VEC(7), (VEC_SIZE * 3)(%rdi)
	subq	$-(VEC_SIZE * 4), %rdi
	cmpq	$(VEC_SIZE * 4), %rdx
	ja	L(loop_4x_vec_forward)
	/* Store the last 4 * VEC.  */
	VMOVU	%VEC(15), (%rcx)
	VMOVU	%VEC(14), -VEC_SIZE(%rcx)
	VMOVU	%VEC(13), -(VEC_SIZE * 2)(%rcx)
	VMOVU	%VEC(12), -(VEC_SIZE * 3)(%rcx)
	/* Store the first VEC.  */
	VMOVU	%VEC(0), (%r11)

	VZEROUPPER_RETURN

L(more_8x_vec_backward):
	/* Load the first 4 * VEC and last VEC to support overlapping addresses.
	 */
	VMOVU	(%rsi), %VEC(0)
	VMOVU	VEC_SIZE(%rsi), %VEC(1)
	VMOVU	(VEC_SIZE * 2)(%rsi), %VEC(2)
	VMOVU	(VEC_SIZE * 3)(%rsi), %VEC(3)
	/* Save stop of the destination buffer.  */
	leaq	-VEC_SIZE(%rdi, %rdx), %r11
	/* Align destination end for aligned stores in the loop.  Compute how
	   much destination end is misaligned.  */
	leaq	-VEC_SIZE(%rsi, %rdx), %rcx
	movq	%r11, %r9
	movq	%r11, %r8
	andq	$(VEC_SIZE - 1), %r8
	/* Adjust source.  */
	subq	%r8, %rcx
	/* Adjust the end of destination which should be aligned now.  */
	subq	%r8, %r9
	/* Adjust length.  */
	subq	%r8, %rdx

	.p2align 4
L(loop_4x_vec_backward):
	/* Copy 4 * VEC a time backward.  */
	VMOVU	(%rcx), %VEC(4)
	VMOVU	-VEC_SIZE(%rcx), %VEC(5)
	VMOVU	-(VEC_SIZE * 2)(%rcx), %VEC(6)
	VMOVU	-(VEC_SIZE * 3)(%rcx), %VEC(7)
	addq	$-(VEC_SIZE * 4), %rcx
	addq	$-(VEC_SIZE * 4), %rdx
	VMOVA	%VEC(4), (%r9)
	VMOVA	%VEC(5), -VEC_SIZE(%r9)
	VMOVA	%VEC(6), -(VEC_SIZE * 2)(%r9)
	VMOVA	%VEC(7), -(VEC_SIZE * 3)(%r9)
	addq	$-(VEC_SIZE * 4), %r9
	cmpq	$(VEC_SIZE * 4), %rdx
	ja	L(loop_4x_vec_backward)
	/* Store the first 4 * VEC.  */
	VMOVU	%VEC(0), (%rdi)
	VMOVU	%VEC(1), VEC_SIZE(%rdi)
	VMOVU	%VEC(2), (VEC_SIZE * 2)(%rdi)
	VMOVU	%VEC(3), (VEC_SIZE * 3)(%rdi)
	/* Store the last VEC.  */
	VMOVU	%VEC(15), (%r11)
	VZEROUPPER_RETURN

#if defined USE_WITH_ERMS && defined USE_WITH_MULTIARCH_AND_LIBC
L(movsb):
	movl	(%rdx), %edx
	cmp	x86_rep_movsb_stop_threshold(%rip), %RDX_LP
	jae	L(more_8x_vec)
	cmpq	%rsi, %rdi
	jb	1f
	/* Source == destination is less common.  */
	je	L(nop)
	leaq	(%rsi, %rdx), %r9
	cmpq	%r9, %rdi
	/* Avoid slow backward REP MOVSB.  */
	jb	L(more_8x_vec_backward)
# if AVOID_SHORT_DISTANCE_REP_MOVSB
	movq	%rdi, %rcx
	subq	%rsi, %rcx
	jmp	2f
# endif
1:
# if AVOID_SHORT_DISTANCE_REP_MOVSB
	movq	%rsi, %rcx
	subq	%rdi, %rcx
2:
	/* Avoid "rep movsb" if RCX, the distance between source and
	   destination, is N*4GB + [1..63] with N >= 0.  */
	cmpl	$63, %ecx
	/* Avoid "rep movsb" if ECX <= 63.  */
	jbe	L(more_2x_vec)
# endif
	mov	%RDX_LP, %RCX_LP
	rep	movsb
L(nop):
	ret
#endif


#ifdef USE_WITH_MULTIARCH_AND_LIBC
	.p2align 4
L(large_memcpy_2x):
	/* Compute absolute value of difference between source and destination.
	 */
	movq	%rdi, %r9
	subq	%rsi, %r9
	movq	%r9, %r8
	leaq	-1(%r9), %rcx
	sarq	$63, %r8
	xorq	%r8, %r9
	subq	%r8, %r9
	/* Don't use non-temporal store if there is overlap between destination
	   and source since destination may be in cache when source is loaded.  */
	cmpq	%r9, %rdx
	ja	L(more_8x_vec_check)

	/* Cache align destination. First store the first 64 bytes then adjust
	   alignments.  */
	VMOVU	(%rsi), %VEC(8)
# if VEC_SIZE < 64
	VMOVU	VEC_SIZE(%rsi), %VEC(9)
#  if VEC_SIZE < 32
	VMOVU	(VEC_SIZE * 2)(%rsi), %VEC(10)
	VMOVU	(VEC_SIZE * 3)(%rsi), %VEC(11)
#  endif
# endif
	VMOVU	%VEC(8), (%rdi)
# if VEC_SIZE < 64
	VMOVU	%VEC(9), VEC_SIZE(%rdi)
#  if VEC_SIZE < 32
	VMOVU	%VEC(10), (VEC_SIZE * 2)(%rdi)
	VMOVU	%VEC(11), (VEC_SIZE * 3)(%rdi)
#  endif
# endif
	/* Adjust source, destination, and size.  */
	movq	%rdi, %r8
	andq	$63, %r8
	/* Get the negative of offset for alignment.  */
	subq	$64, %r8
	/* Adjust source.  */
	subq	%r8, %rsi
	/* Adjust destination which should be aligned now.  */
	subq	%r8, %rdi
	/* Adjust length.  */
	addq	%r8, %rdx

	/* Test if source and destination addresses will alias. If they do the
	   larger pipeline in large_memcpy_4x alleviated the performance drop.  */
	testl	$(PAGE_SIZE - VEC_SIZE * 8), %ecx
	jz	L(large_memcpy_4x)

	movq	%rdx, %r10
	shrq	$LOG_4X_MEMCPY_THRESH, %r10
	cmp	x86_shared_non_temporal_threshold(%rip), %r10
	jae	L(large_memcpy_4x)

	/* edx will store remainder size for copying tail.  */
	andl	$(PAGE_SIZE * 2 - 1), %edx
	/* r10 stores outer loop counter.  */
	shrq	$((LOG_PAGE_SIZE + 1) - LOG_4X_MEMCPY_THRESH), %r10
	/* Copy 4x VEC at a time from 2 pages.  */
	.p2align 4
L(loop_large_memcpy_2x_outer):
	/* ecx stores inner loop counter.  */
	movl	$(PAGE_SIZE / LARGE_LOAD_SIZE), %ecx
L(loop_large_memcpy_2x_inner):
	PREFETCH_ONE_SET(1, (%rsi), PREFETCHED_LOAD_SIZE)
	PREFETCH_ONE_SET(1, (%rsi), PREFETCHED_LOAD_SIZE * 2)
	PREFETCH_ONE_SET(1, (%rsi), PAGE_SIZE + PREFETCHED_LOAD_SIZE)
	PREFETCH_ONE_SET(1, (%rsi), PAGE_SIZE + PREFETCHED_LOAD_SIZE * 2)
	/* Load vectors from rsi.  */
	LOAD_ONE_SET((%rsi), 0, %VEC(0), %VEC(1), %VEC(2), %VEC(3))
	LOAD_ONE_SET((%rsi), PAGE_SIZE, %VEC(4), %VEC(5), %VEC(6), %VEC(7))
	subq	$-LARGE_LOAD_SIZE, %rsi
	/* Non-temporal store vectors to rdi.  */
	STORE_ONE_SET((%rdi), 0, %VEC(0), %VEC(1), %VEC(2), %VEC(3))
	STORE_ONE_SET((%rdi), PAGE_SIZE, %VEC(4), %VEC(5), %VEC(6), %VEC(7))
	subq	$-LARGE_LOAD_SIZE, %rdi
	decl	%ecx
	jnz	L(loop_large_memcpy_2x_inner)
	addq	$PAGE_SIZE, %rdi
	addq	$PAGE_SIZE, %rsi
	decq	%r10
	jne	L(loop_large_memcpy_2x_outer)
	sfence

	/* Check if only last 4 loads are needed.  */
	cmpl	$(VEC_SIZE * 4), %edx
	jbe	L(large_memcpy_2x_end)

	/* Handle the last 2 * PAGE_SIZE bytes.  */
L(loop_large_memcpy_2x_tail):
	/* Copy 4 * VEC a time forward with non-temporal stores.  */
	PREFETCH_ONE_SET (1, (%rsi), PREFETCHED_LOAD_SIZE)
	PREFETCH_ONE_SET (1, (%rdi), PREFETCHED_LOAD_SIZE)
	VMOVU	(%rsi), %VEC(0)
	VMOVU	VEC_SIZE(%rsi), %VEC(1)
	VMOVU	(VEC_SIZE * 2)(%rsi), %VEC(2)
	VMOVU	(VEC_SIZE * 3)(%rsi), %VEC(3)
	subq	$-(VEC_SIZE * 4), %rsi
	addl	$-(VEC_SIZE * 4), %edx
	VMOVA	%VEC(0), (%rdi)
	VMOVA	%VEC(1), VEC_SIZE(%rdi)
	VMOVA	%VEC(2), (VEC_SIZE * 2)(%rdi)
	VMOVA	%VEC(3), (VEC_SIZE * 3)(%rdi)
	subq	$-(VEC_SIZE * 4), %rdi
	cmpl	$(VEC_SIZE * 4), %edx
	ja	L(loop_large_memcpy_2x_tail)

L(large_memcpy_2x_end):
	/* Store the last 4 * VEC.  */
	VMOVU	-(VEC_SIZE * 4)(%rsi, %rdx), %VEC(0)
	VMOVU	-(VEC_SIZE * 3)(%rsi, %rdx), %VEC(1)
	VMOVU	-(VEC_SIZE * 2)(%rsi, %rdx), %VEC(2)
	VMOVU	-VEC_SIZE(%rsi, %rdx), %VEC(3)

	VMOVU	%VEC(0), -(VEC_SIZE * 4)(%rdi, %rdx)
	VMOVU	%VEC(1), -(VEC_SIZE * 3)(%rdi, %rdx)
	VMOVU	%VEC(2), -(VEC_SIZE * 2)(%rdi, %rdx)
	VMOVU	%VEC(3), -VEC_SIZE(%rdi, %rdx)
	VZEROUPPER_RETURN

	.p2align 4
L(large_memcpy_4x):
	movq	%rdx, %r10
	/* edx will store remainder size for copying tail.  */
	andl	$(PAGE_SIZE * 4 - 1), %edx
	/* r10 stores outer loop counter.  */
	shrq	$(LOG_PAGE_SIZE + 2), %r10
	/* Copy 4x VEC at a time from 4 pages.  */
	.p2align 4
L(loop_large_memcpy_4x_outer):
	/* ecx stores inner loop counter.  */
	movl	$(PAGE_SIZE / LARGE_LOAD_SIZE), %ecx
L(loop_large_memcpy_4x_inner):
	/* Only one prefetch set per page as doing 4 pages give more time for
	   prefetcher to keep up.  */
	PREFETCH_ONE_SET(1, (%rsi), PREFETCHED_LOAD_SIZE)
	PREFETCH_ONE_SET(1, (%rsi), PAGE_SIZE + PREFETCHED_LOAD_SIZE)
	PREFETCH_ONE_SET(1, (%rsi), PAGE_SIZE * 2 + PREFETCHED_LOAD_SIZE)
	PREFETCH_ONE_SET(1, (%rsi), PAGE_SIZE * 3 + PREFETCHED_LOAD_SIZE)
	/* Load vectors from rsi.  */
	LOAD_ONE_SET((%rsi), 0, %VEC(0), %VEC(1), %VEC(2), %VEC(3))
	LOAD_ONE_SET((%rsi), PAGE_SIZE, %VEC(4), %VEC(5), %VEC(6), %VEC(7))
	LOAD_ONE_SET((%rsi), PAGE_SIZE * 2, %VEC(8), %VEC(9), %VEC(10), %VEC(11))
	LOAD_ONE_SET((%rsi), PAGE_SIZE * 3, %VEC(12), %VEC(13), %VEC(14), %VEC(15))
	subq	$-LARGE_LOAD_SIZE, %rsi
	/* Non-temporal store vectors to rdi.  */
	STORE_ONE_SET((%rdi), 0, %VEC(0), %VEC(1), %VEC(2), %VEC(3))
	STORE_ONE_SET((%rdi), PAGE_SIZE, %VEC(4), %VEC(5), %VEC(6), %VEC(7))
	STORE_ONE_SET((%rdi), PAGE_SIZE * 2, %VEC(8), %VEC(9), %VEC(10), %VEC(11))
	STORE_ONE_SET((%rdi), PAGE_SIZE * 3, %VEC(12), %VEC(13), %VEC(14), %VEC(15))
	subq	$-LARGE_LOAD_SIZE, %rdi
	decl	%ecx
	jnz	L(loop_large_memcpy_4x_inner)
	addq	$(PAGE_SIZE * 3), %rdi
	addq	$(PAGE_SIZE * 3), %rsi
	decq	%r10
	jne	L(loop_large_memcpy_4x_outer)
	sfence
	/* Check if only last 4 loads are needed.  */
	cmpl	$(VEC_SIZE * 4), %edx
	jbe	L(large_memcpy_4x_end)

	/* Handle the last 4  * PAGE_SIZE bytes.  */
L(loop_large_memcpy_4x_tail):
	/* Copy 4 * VEC a time forward with non-temporal stores.  */
	PREFETCH_ONE_SET (1, (%rsi), PREFETCHED_LOAD_SIZE)
	PREFETCH_ONE_SET (1, (%rdi), PREFETCHED_LOAD_SIZE)
	VMOVU	(%rsi), %VEC(0)
	VMOVU	VEC_SIZE(%rsi), %VEC(1)
	VMOVU	(VEC_SIZE * 2)(%rsi), %VEC(2)
	VMOVU	(VEC_SIZE * 3)(%rsi), %VEC(3)
	subq	$-(VEC_SIZE * 4), %rsi
	addl	$-(VEC_SIZE * 4), %edx
	VMOVA	%VEC(0), (%rdi)
	VMOVA	%VEC(1), VEC_SIZE(%rdi)
	VMOVA	%VEC(2), (VEC_SIZE * 2)(%rdi)
	VMOVA	%VEC(3), (VEC_SIZE * 3)(%rdi)
	subq	$-(VEC_SIZE * 4), %rdi
	cmpl	$(VEC_SIZE * 4), %edx
	ja	L(loop_large_memcpy_4x_tail)

L(large_memcpy_4x_end):
	/* Store the last 4 * VEC.  */
	VMOVU	-(VEC_SIZE * 4)(%rsi, %rdx), %VEC(0)
	VMOVU	-(VEC_SIZE * 3)(%rsi, %rdx), %VEC(1)
	VMOVU	-(VEC_SIZE * 2)(%rsi, %rdx), %VEC(2)
	VMOVU	-VEC_SIZE(%rsi, %rdx), %VEC(3)

	VMOVU	%VEC(0), -(VEC_SIZE * 4)(%rdi, %rdx)
	VMOVU	%VEC(1), -(VEC_SIZE * 3)(%rdi, %rdx)
	VMOVU	%VEC(2), -(VEC_SIZE * 2)(%rdi, %rdx)
	VMOVU	%VEC(3), -VEC_SIZE(%rdi, %rdx)
	VZEROUPPER_RETURN
#endif

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
