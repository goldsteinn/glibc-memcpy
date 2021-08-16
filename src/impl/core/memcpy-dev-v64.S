/* memmove/memcpy/mempcpy with unaligned load/store and rep movsb
   Copyright (C) 2016-2021 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
<https:	// www.gnu.org/licenses/>.  */

/* memmove/memcpy/mempcpy is implemented as:
   1. Use overlapping load and store to avoid branch.
   2. Load all sources into registers and store them together to avoid
      possible address overlap between source and destination.
   3. If size is 8 * VEC_SIZE or less, load all sources into registers
      and store them together.
   4. If address of destination > address of source, backward copy
      4 * VEC_SIZE at a time with unaligned load and aligned store.
      Load the first 4 * VEC and last VEC before the loop and store
      them after the loop to support overlapping addresses.
   5. Otherwise, forward copy 4 * VEC_SIZE at a time with unaligned
      load and aligned store.  Load the last 4 * VEC and first VEC
      before the loop and store them after the loop to support
      overlapping addresses.
   6. On machines with ERMS feature, if size greater than equal or to
      __x86_rep_movsb_threshold and less than
      __x86_rep_movsb_stop_threshold, then REP MOVSB will be used.
   7. If size >= __x86_shared_non_temporal_threshold and there is no
      overlap between destination and source, use non-temporal store
      instead of aligned store copying from either 2 or 4 pages at
      once.
   8. For point 7) if size < 16 * __x86_shared_non_temporal_threshold
      and source and destination do not page alias, copy from 2 pages
      at once using non-temporal stores. Page aliasing in this case is
      considered true if destination's page alignment - sources' page
      alignment is less than 8 * VEC_SIZE.
   9. If size >= 16 * __x86_shared_non_temporal_threshold or source
      and destination do page alias copy from 4 pages at once using
      non-temporal stores.  */


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
	/* VEC_SIZE == 64. Based on SPEC2017 distribution sizes in [1, 32] are
	   all hot so this branch is expected to be taken. Make jbe to keep it as
	   predictable as possible.  */
	cmpq	$(VEC_SIZE), %rdx
	jbe	L(leq_1x)
	/* Will need to copy VEC(0) and VEC(15) no matter what.  */
	VMOVU	(VEC_SIZE * 0)(%rsi), %VEC(0)
	VMOVU	(VEC_SIZE * -1)(%rsi, %rdx), %VEC(15)
	cmpq	$(VEC_SIZE * 2), %rdx
	ja	L(gt_2x)
	/* copy (1x, 2x].  */
	VMOVU	%VEC(0), (VEC_SIZE * 0)(%rdi)
	VMOVU	%VEC(15), (VEC_SIZE * -1)(%rdi, %rdx)
	VZEROUPPER_RETURN

	.p2align 4
L(copy_8_15):
	movq	(%rsi), %rcx
	movq	-8(%rsi, %rdx), %rsi
	movq	%rcx, (%rdi)
	movq	%rsi, -8(%rdi, %rdx)
	ret
L(copy_32_63):
	vmovdqu64 (%rsi), %ymm16
	vmovdqu64 -32(%rsi, %rdx), %ymm17
	vmovdqu64 %ymm16, (%rdi)
	vmovdqu64 %ymm17, -32(%rdi, %rdx)
	ret

	.p2align 4
L(leq_1x):
	cmpl	$16, %edx
	jb	L(copy_0_15)

	cmpl	$32, %edx
	ja	L(copy_32_63)

	/* Put fall through as [16, 32].  */
	vmovdqu	(%rsi), %xmm0
	vmovdqu	-16(%rsi, %rdx), %xmm1
	vmovdqu	%xmm0, (%rdi)
	vmovdqu	%xmm1, -16(%rdi, %rdx)
	ret

	.p2align 4
L(copy_4_7):
	movl	(%rsi), %ecx
	movl	-4(%rsi, %rdx), %esi
	movl	%ecx, (%rdi)
	movl	%esi, -4(%rdi, %rdx)
	ret

	.p2align 4
L(copy_0_15):
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
L(copy_1):
	movzbl	(%rsi), %ecx
	movb	%cl, (%rdi)
	ret

	/* NOP16 to avoid branch conflicts.  */
	.p2align 4
L(gt_2x):
	cmpq	$(VEC_SIZE * 4), %rdx
	jbe	L(copy_gt_2x_leq_4x)

	cmpq	$(VEC_SIZE * 8), %rdx
	ja	L(gt_8x)

	/* copy (4x, 8x].  */
	VMOVU	(VEC_SIZE * 1)(%rsi), %VEC(1)
	VMOVU	(VEC_SIZE * 2)(%rsi), %VEC(2)
	VMOVU	(VEC_SIZE * 3)(%rsi), %VEC(3)

	VMOVU	(VEC_SIZE * -2)(%rsi, %rdx), %VEC(14)
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
L(copy_gt_2x_leq_4x):
	VMOVU	(VEC_SIZE * 1)(%rsi), %VEC(1)
	VMOVU	(VEC_SIZE * -2)(%rsi, %rdx), %VEC(14)

	VMOVU	%VEC(0), (VEC_SIZE * 0)(%rdi)
	VMOVU	%VEC(1), (VEC_SIZE * 1)(%rdi)

	VMOVU	%VEC(15), (VEC_SIZE * -1)(%rdi, %rdx)
	VMOVU	%VEC(14), (VEC_SIZE * -2)(%rdi, %rdx)
L(nop):
	VZEROUPPER_RETURN

	.p2align 4
L(gt_8x):
#ifdef USE_WITH_MULTIARCH_AND_LIBC
# ifdef USE_WITH_ERMS
	/* If size greater than __x86_rep_movsb_threshold use rep movsb. This
	   will go to NT store path if approriate.  */
	cmpq	x86_rep_movsb_threshold(%rip), %rdx
	ja	L(movsb)
# else
	/* If without ERMS; if size greater than
	   __x86_shared_non_temporal_threshold use NT stores.  */
	cmpq	x86_shared_non_temporal_threshold(%rip), %rdx
	ja	L(large_memcpy_2x)
# endif
	NOP3	// CHECK IF THIS IS NECESSARY
#endif
	/* If dst (rdi) > src (rsi) do backward copy to support overlapping
	   regions.  */
L(gt_8x_check):
	cmpq	%rsi, %rdi
	ja	L(gt_8x_backward)
	/* If dst (rdi) == src (rsi) do nothing.  */
	je	L(nop)

	/* Target if L(movsb) or L(large_memcpy) are not optimal despite the
	   size.  */
L(gt_8x_forward):
	/* Load end of region so support copy of overlapping regions.  */
    VMOVU	(VEC_SIZE * -2)(%rsi, %rdx), %VEC(14)
	VMOVU	(VEC_SIZE * -3)(%rsi, %rdx), %VEC(13)
	VMOVU	(VEC_SIZE * -4)(%rsi, %rdx), %VEC(12)

	/* Subtract dst (rdi) from src (rsi) before aligning dst (rdi) so that
	   src (rsi) can be properly adjusted.  */
	subq	%rdi, %rsi
	/* Store end pointer in rdx.  */
	leaq	(VEC_SIZE * -4)(%rdi, %rdx), %rdx
	/* 64 byte align dst (rdi). Note VEC(0) and VEC(1) where already loaded
	   in earlier logic.  */
	orq	$63, %rdi
	/* Readjusted src (rsi).  */
	leaq	1(%rdi, %rsi), %rsi
	/* Finish aligning dst (rdi).  */
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

	/* Store end of region.  */
	VMOVU	%VEC(12), (VEC_SIZE * 0)(%rdx)
	VMOVU	%VEC(13), (VEC_SIZE * 1)(%rdx)
	VMOVU	%VEC(14), (VEC_SIZE * 2)(%rdx)
	VMOVU	%VEC(15), (VEC_SIZE * 3)(%rdx)

	/* Store VECs loaded for aligning dst (rdi).  */

	/* NB: rax was set to dst on function entry.  */
	VMOVU	%VEC(0), (VEC_SIZE * 0)(%rax)
	VZEROUPPER_RETURN

    NOP32
	.p2align 4
L(gt_8x_backward):
	/* Load begining of region to support overlap.  */
	VMOVU	(VEC_SIZE * 1)(%rsi), %VEC(1)
	VMOVU	(VEC_SIZE * 2)(%rsi), %VEC(2)
	VMOVU	(VEC_SIZE * 3)(%rsi), %VEC(3)

	/* Subtract dst (rdi) from src (rsi) before aligning dst (rdi) so that
	   src (rsi) can be properly adjusted.  */
	subq	%rdi, %rsi

	/* Set dst (rdi) as end of region. Set 4x VEC from begining of region as
	   those VECs where already loaded. -1 for aligning dst (rdi).  */
	leaq	(VEC_SIZE * -4 + -1)(%rdi, %rdx), %rdi
	/* Align dst (rdi).  */
	andq	$-64, %rdi
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
	VMOVU	%VEC(15), (VEC_SIZE * -1)(%rax, %rdx)
	VZEROUPPER_RETURN
#ifdef USE_WITH_MULTIARCH_AND_LIBC
# ifdef USE_WITH_ERMS
	.p2align 4
L(movsb):
	/* If size > __x86_rep_movsb_stop_threshold go to large copy.  */

	/* NB: __x86_rep_movsb_stop_threshold in range
	   [__x86_rep_movsb_threshold, __x86_shared_non_temporal_threshold].  */
	cmp	x86_rep_movsb_stop_threshold(%rip), %RDX_LP
	jae	L(large_memcpy_2x)
	/* Check if dst (rdi) > src (rsi) and overlapping regions. If so don't
	   use rep movsb as backward copy is slow.  */
	movq	%rdi, %rcx
	subq	%rsi, %rcx
	cmpq	%rdx, %rcx
	jb	L(gt_8x_backward)
#  if AVOID_SHORT_DISTANCE_REP_MOVSB
	// andl    $X86_STRING_CONTROL_AVOID_SHORT_DISTANCE_REP_MOVSB,
	// __x86_string_control(%rip)
	// jz      L(movsb_aligned)
	/* The backward + overlapping check above would covers short distance
	   backward copy.  */
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
	VMOVU	%VEC(15), (VEC_SIZE * -1)(%rax, %rdx)
	ret
# endif
	.p2align 4
L(large_memcpy_2x):
# ifdef USE_WITH_ERMS
	/* Its possible to enter L(large_memcpy) with size less than
	   __x86_shared_non_temporal_threshold.  */
	cmpq	x86_shared_non_temporal_threshold(%rip), %rdx
	jb	L(gt_8x)
# endif

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
	ja	L(gt_8x_check)
	/* Store VECs for 64 byte aligning dst (rdi) (loaded earlier).  */
	VMOVU	%VEC(0), (%rdi)
# if VEC_SIZE < 64
	VMOVU	%VEC(1), VEC_SIZE(%rdi)
#  if VEC_SIZE < 32
	VMOVU	%VEC(2), (VEC_SIZE * 2)(%rdi)
	VMOVU	%VEC(3), (VEC_SIZE * 3)(%rdi)
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
	subl	$(VEC_SIZE * 4), %edx
	jbe	L(large_memcpy_2x_end)

	/* Handle the last 2 * PAGE_SIZE bytes.  */
	.p2align 4
L(loop_large_memcpy_2x_tail):
	/* Copy 4 * VEC a time forward with non-temporal stores.  */
	PREFETCH_ONE_SET (1, (%rsi), PREFETCHED_LOAD_SIZE)
	PREFETCH_ONE_SET (1, (%rdi), PREFETCHED_LOAD_SIZE)
	VMOVU	(%rsi), %VEC(0)
	VMOVU	VEC_SIZE(%rsi), %VEC(1)
	VMOVU	(VEC_SIZE * 2)(%rsi), %VEC(2)
	VMOVU	(VEC_SIZE * 3)(%rsi), %VEC(3)
	subq	$-(VEC_SIZE * 4), %rsi
	VMOVA	%VEC(0), (%rdi)
	VMOVA	%VEC(1), VEC_SIZE(%rdi)
	VMOVA	%VEC(2), (VEC_SIZE * 2)(%rdi)
	VMOVA	%VEC(3), (VEC_SIZE * 3)(%rdi)
	subq	$-(VEC_SIZE * 4), %rdi
	subl	$-(VEC_SIZE * 4), %edx
	ja	L(loop_large_memcpy_2x_tail)

L(large_memcpy_2x_end):
	/* Store the last 4 * VEC.  */
# if VEC_SIZE > 32
	VMOVU	(VEC_SIZE * -2)(%rsi, %rdx), %VEC(14)
# endif
# if VEC_SIZE > 16
	VMOVU	(VEC_SIZE * -3)(%rsi, %rdx), %VEC(13)
	VMOVU	(VEC_SIZE * -4)(%rsi, %rdx), %VEC(12)
# endif
	VMOVU	%VEC(15), -VEC_SIZE(%rdi, %rdx)
	VMOVU	%VEC(14), -(VEC_SIZE * 2)(%rdi, %rdx)
	VMOVU	%VEC(13), -(VEC_SIZE * 3)(%rdi, %rdx)
	VMOVU	%VEC(12), -(VEC_SIZE * 4)(%rdi, %rdx)
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
# if VEC_SIZE > 32
	VMOVU	(VEC_SIZE * -2)(%rsi, %rdx), %VEC(14)
# endif
# if VEC_SIZE > 16
	VMOVU	(VEC_SIZE * -3)(%rsi, %rdx), %VEC(13)
	VMOVU	(VEC_SIZE * -4)(%rsi, %rdx), %VEC(12)
# endif
	VMOVU	%VEC(15), -VEC_SIZE(%rdi, %rdx)
	VMOVU	%VEC(14), -(VEC_SIZE * 2)(%rdi, %rdx)
	VMOVU	%VEC(13), -(VEC_SIZE * 3)(%rdi, %rdx)
	VMOVU	%VEC(12), -(VEC_SIZE * 4)(%rdi, %rdx)
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
