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

#include <sysdep.h>

#ifndef MEMCPY_SYMBOL
# define MEMCPY_SYMBOL(p,s)	MEMMOVE_SYMBOL(p,	s)
#endif

#ifndef MEMPCPY_SYMBOL
# define MEMPCPY_SYMBOL(p,s)	MEMMOVE_SYMBOL(p,	s)
#endif

#ifndef MEMMOVE_CHK_SYMBOL
# define MEMMOVE_CHK_SYMBOL(p,s)	MEMMOVE_SYMBOL(p,	s)
#endif


#define ALIGN_MOVSB	1
#define ALIGN_MOVSB_EXCLUSIVE	0
#define LOOP_ALIGN_CONF	64
#define MOVSB_ALIGN_CONF	64


#if VEC_SIZE < LOOP_ALIGN_CONF
# define LOOP_4X_ALIGN_TO	(VEC_SIZE	*	2)
#else
# define LOOP_4X_ALIGN_TO	(VEC_SIZE)
#endif
	/* Avoid short distance rep movsb only with non-SSE vector.  */
#if ALIGN_MOVSB
# if VEC_SIZE < MOVSB_ALIGN_CONF
#  define MOVSB_ALIGN_TO	(VEC_SIZE	*	2)
# else
#  define MOVSB_ALIGN_TO	(VEC_SIZE)
# endif
#else
# define MOVSB_ALIGN_TO	0
#endif
#if MOVSB_ALIGN_TO > LOOP_4X_ALIGN_TO
# error "Invalid MOVSB Alignment"
#endif


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

	/* Avoid short distance rep movsb only with non-SSE vector.  */
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

#define COPY_BLOCK(mov_inst,	src_reg,	dst_reg,	size_reg,	len,	tmp_reg0,	tmp_reg1)	\
	mov_inst (%src_reg), %tmp_reg0; \
	mov_inst -(len)(%src_reg, %size_reg), %tmp_reg1; \
	mov_inst %tmp_reg0, (%dst_reg); \
	mov_inst %tmp_reg1, -(len)(%dst_reg, %size_reg);


#define COPY_4_8	COPY_BLOCK(movl,	rsi,	rdi,	rdx,	4,	ecx,	esi)
#define COPY_8_16	COPY_BLOCK(movq,	rsi,	rdi,	rdx,	8,	rcx,	rsi)
#define COPY_16_32	COPY_BLOCK(vmovdqu,	rsi,	rdi,	rdx,	16,	xmm0,	xmm1)
#define COPY_32_64	COPY_BLOCK(vmovdqu64,	rsi,	rdi,	rdx,	32,	ymm16,	ymm17)


#ifndef SECTION
# error SECTION is not defined!
#endif

	.section SECTION(.text), "ax", @progbits
#if defined SHARED && IS_IN (libc)
ENTRY (MEMMOVE_CHK_SYMBOL (__mempcpy_chk, unaligned))
	cmp	%RDX_LP, %RCX_LP
	jb	HIDDEN_JUMPTARGET (__chk_fail)
END (MEMMOVE_CHK_SYMBOL (__mempcpy_chk, unaligned))
#endif

ENTRY (MEMPCPY_SYMBOL (__mempcpy, unaligned))
	mov	%RDI_LP, %RAX_LP
	add	%RDX_LP, %RAX_LP
	jmp	L(start)
END (MEMPCPY_SYMBOL (__mempcpy, unaligned))

#if defined SHARED && IS_IN (libc)
ENTRY (MEMMOVE_CHK_SYMBOL (__memmove_chk, unaligned))
	cmp	%RDX_LP, %RCX_LP
	jb	HIDDEN_JUMPTARGET (__chk_fail)
END (MEMMOVE_CHK_SYMBOL (__memmove_chk, unaligned))
#endif


#if defined WITH_CHK_ERMS
ENTRY (__mempcpy_chk_erms)
	cmp	%RDX_LP, %RCX_LP
	jb	HIDDEN_JUMPTARGET (__chk_fail)
END (__mempcpy_chk_erms)

	/* Only used to measure performance of REP MOVSB.  */
ENTRY (__mempcpy_erms)
	mov	%RDI_LP, %RAX_LP
	/* Skip zero length.  */
	test	%RDX_LP, %RDX_LP
	jz	2f
	add	%RDX_LP, %RAX_LP
	jmp	L(start_movsb)
END (__mempcpy_erms)

ENTRY (__memmove_chk_erms)
	cmp	%RDX_LP, %RCX_LP
	jb	HIDDEN_JUMPTARGET (__chk_fail)
END (__memmove_chk_erms)

ENTRY (__memmove_erms)
	movq	%rdi, %rax
	/* Skip zero length.  */
	test	%RDX_LP, %RDX_LP
	jz	2f
L(start_movsb):
	mov	%RDX_LP, %RCX_LP
	cmp	%RSI_LP, %RDI_LP
	jb	1f
	/* Source == destination is less common.  */
	je	2f
	lea	(%rsi, %rcx), %RDX_LP
	cmp	%RDX_LP, %RDI_LP
	jb	L(movsb_backward)
1:
	rep	movsb
2:
	ret
L(movsb_backward):
	leaq	-1(%rdi, %rcx), %rdi
	leaq	-1(%rsi, %rcx), %rsi
	std
	rep	movsb
	cld
	ret
END (__memmove_erms)
	strong_alias (__memmove_erms, __memcpy_erms)
	strong_alias (__memmove_chk_erms, __memcpy_chk_erms)
#endif

P2ALIGN_ENTRY (MEMMOVE_SYMBOL (__memmove, unaligned), 6)
	movq	%rdi, %rax
L(start):
#ifdef __ILP32__
	/* Clear the upper 32 bits.  */
	movl	%edx, %edx
#endif
	cmp	$VEC_SIZE, %RDX_LP
#if VEC_SIZE > 16
	jbe	L(leq_1x)
#else
	jb	L(leq_1x)
#endif
#if MOVSB_ALIGN_TO >= VEC_SIZE
	VMOVU	(%rsi), %VEC(0)
#endif
	cmp	$(VEC_SIZE * 2), %RDX_LP
	ja	L(gt_2x)
	/* From VEC and to 2 * VEC.  No branch when size == VEC_SIZE.  */
#if MOVSB_ALIGN_TO < VEC_SIZE
	VMOVU	(%rsi), %VEC(0)
#endif
	VMOVU	(VEC_SIZE * -1)(%rsi, %rdx), %VEC(15)
	VMOVU	%VEC(0), (VEC_SIZE * 0)(%rdi)
	VMOVU	%VEC(15), (VEC_SIZE * -1)(%rdi, %rdx)
L(return):
#if VEC_SIZE > 16
	ZERO_UPPER_VEC_REGISTERS_RETURN
#else
	ret
#endif

#if VEC_SIZE == 64
L(copy_8_15):
	COPY_8_16
	ret

L(copy_32_63):
	vmovdqu64 (%rsi), %ymm16
	vmovdqu64 -32(%rsi, %rdx), %ymm17
	vmovdqu64 %ymm16, (%rdi)
	vmovdqu64 %ymm17, -32(%rdi, %rdx)
	ret
#endif

	.p2align 4,, 6
L(leq_1x):
#if VEC_SIZE != 16 && VEC_SIZE != 32 && VEC_SIZE != 64
# error Unsupported VEC_SIZE!
#endif
	cmpl	$(VEC_SIZE / 4), %edx
	jb	L(lt_quarter_x)

	cmpl	$(VEC_SIZE / 2), %edx
#if VEC_SIZE == 64
	ja	L(copy_32_63)
	COPY_16_32
#elif VEC_SIZE == 32
	jb	L(copy_8_15)
	COPY_16_32
#else
	jb	L(copy_4_7)
	COPY_8_16
#endif
	ret


	.p2align 4,, 6
L(copy_4_7):
	COPY_4_8
	ret

L(copy_1):
	movzbl	(%rsi), %ecx
	movb	%cl, (%rdi)
	ret

	/* Colder copy case for [0, 7].  */
L(lt_quarter_x):
#if VEC_SIZE > 32
	cmpl	$8, %edx
	jae	L(copy_8_15)
#endif
#if VEC_SIZE > 16
	cmpl	$4, %edx
	jae	L(copy_4_7)
#endif
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
#if VEC_SIZE == 32
L(copy_8_15):
	COPY_8_16
	ret
#endif


L(gt_2x):
#if MOVSB_ALIGN_TO >= (VEC_SIZE * 2)
	VMOVU	(VEC_SIZE * 1)(%rsi), %VEC(1)
#endif
	cmpq	$(VEC_SIZE * 8), %rdx
	ja	L(gt_8x)
#if MOVSB_ALIGN_TO < (VEC_SIZE)
	VMOVU	(VEC_SIZE * 0)(%rsi), %VEC(0)
#endif
#if MOVSB_ALIGN_TO < (VEC_SIZE * 2)
	VMOVU	(VEC_SIZE * 1)(%rsi), %VEC(1)
#endif
	cmpl	$(VEC_SIZE * 4), %edx
	jbe	L(copy_gt_2x_leq_4x)

	VMOVU	(VEC_SIZE * 2)(%rsi), %VEC(2)
	VMOVU	(VEC_SIZE * 3)(%rsi), %VEC(3)
	VMOVU	(VEC_SIZE * -4)(%rsi, %rdx), %VEC(12)
	VMOVU	(VEC_SIZE * -3)(%rsi, %rdx), %VEC(13)
	VMOVU	(VEC_SIZE * -2)(%rsi, %rdx), %VEC(14)
	VMOVU	(VEC_SIZE * -1)(%rsi, %rdx), %VEC(15)
	VMOVU	%VEC(0), (VEC_SIZE * 0)(%rdi)
	VMOVU	%VEC(1), (VEC_SIZE * 1)(%rdi)
	VMOVU	%VEC(2), (VEC_SIZE * 2)(%rdi)
	VMOVU	%VEC(3), (VEC_SIZE * 3)(%rdi)

	VMOVU	%VEC(12), (VEC_SIZE * -4)(%rdi, %rdx)
	VMOVU	%VEC(13), (VEC_SIZE * -3)(%rdi, %rdx)
	VMOVU	%VEC(14), (VEC_SIZE * -2)(%rdi, %rdx)
	VMOVU	%VEC(15), (VEC_SIZE * -1)(%rdi, %rdx)
	VZEROUPPER_RETURN

	.p2align 4
L(copy_gt_2x_leq_4x):
	VMOVU	(VEC_SIZE * -2)(%rsi, %rdx), %VEC(14)
	VMOVU	(VEC_SIZE * -1)(%rsi, %rdx), %VEC(15)

	VMOVU	%VEC(0), (VEC_SIZE * 0)(%rdi)
	VMOVU	%VEC(1), (VEC_SIZE * 1)(%rdi)
	VMOVU	%VEC(14), (VEC_SIZE * -2)(%rdi, %rdx)
	VMOVU	%VEC(15), (VEC_SIZE * -1)(%rdi, %rdx)
	VZEROUPPER_RETURN
#if VEC_SIZE != 16
	.p2align 4
#endif

	.p2align 4
L(gt_8x):
	movq	%rdi, %rcx
	subq	%rsi, %rcx
	movq	%rdi, %r8
	cmpq	%rdx, %rcx
	jb	L(gt_8x_backward)

#ifdef USE_WITH_ERMS
	cmpq	__x86_rep_movsb_threshold(%rip), %rdx
	ja	L(movsb)
#elif (defined USE_MULTIARCH || VEC_SIZE == 16) && IS_IN (libc)
	/* Check non-temporal store threshold.  */
	cmp	__x86_shared_non_temporal_threshold(%rip), %RDX_LP
	ja	L(large_memcpy_2x)
#endif
	/* Entry if rdx is greater than non-temporal threshold but there is
	   overlap.  */
L(gt_8x_forward):
#if MOVSB_ALIGN_TO < (VEC_SIZE)
	VMOVU	(VEC_SIZE * 0)(%rsi), %VEC(0)
#endif
#if MOVSB_ALIGN_TO < (VEC_SIZE * 2) && (LOOP_4X_ALIGN_TO > VEC_SIZE)
	VMOVU	(VEC_SIZE * 1)(%rsi), %VEC(1)
#endif

	VMOVU	(VEC_SIZE * -1)(%rsi, %rdx), %VEC(15)
	VMOVU	(VEC_SIZE * -2)(%rsi, %rdx), %VEC(14)
	VMOVU	(VEC_SIZE * -3)(%rsi, %rdx), %VEC(13)
	VMOVU	(VEC_SIZE * -4)(%rsi, %rdx), %VEC(12)
	subq	%rdi, %rsi
	leaq	(VEC_SIZE * -4)(%rdi, %rdx), %rdx
	orq	$(LOOP_4X_ALIGN_TO - 1), %rdi
	leaq	1(%rdi, %rsi), %rsi
	incq	%rdi
	/* Loop 4x VEC at a time copy forward from src (rsi) to dst (rdi).  */
	.p2align 4
L(loop_4x_forward):
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
	ja	L(loop_4x_forward)
	VMOVU	%VEC(12), (VEC_SIZE * 0)(%rdx)
	VMOVU	%VEC(13), (VEC_SIZE * 1)(%rdx)
	VMOVU	%VEC(14), (VEC_SIZE * 2)(%rdx)
	VMOVU	%VEC(15), (VEC_SIZE * 3)(%rdx)

	VMOVU	%VEC(0), (VEC_SIZE * 0)(%r8)
#if (LOOP_4X_ALIGN_TO > VEC_SIZE)
	VMOVU	%VEC(1), (VEC_SIZE * 1)(%r8)
#endif
L(nop):
	VZEROUPPER_RETURN

	.p2align 4
L(gt_8x_backward):
#if MOVSB_ALIGN_TO < (VEC_SIZE)
	VMOVU	(VEC_SIZE * 0)(%rsi), %VEC(0)
#endif
#if MOVSB_ALIGN_TO < (VEC_SIZE * 2)
	VMOVU	(VEC_SIZE * 1)(%rsi), %VEC(1)
#endif
	VMOVU	(VEC_SIZE * 2)(%rsi), %VEC(2)
	VMOVU	(VEC_SIZE * 3)(%rsi), %VEC(3)

	VMOVU	(VEC_SIZE * -1)(%rsi, %rdx), %VEC(15)
#if (LOOP_4X_ALIGN_TO > VEC_SIZE)
	VMOVU	(VEC_SIZE * -2)(%rsi, %rdx), %VEC(14)
#endif

	/* Subtract dst (rdi) from src (rsi) before aligning dst (rdi) so that
	   src (rsi) can be properly adjusted.  */
	subq	%rdi, %rsi
	jz	L(nop)
	/* Set dst (rdi) as end of region. Set 4x VEC from begining of region as
	   those VECs where already loaded. -1 for aligning dst (rdi).  */
	leaq	(VEC_SIZE * -4 + -1)(%rdi, %rdx), %rdi
	/* Align dst (rdi).  */
	andq	$-(LOOP_4X_ALIGN_TO), %rdi
	/* Readjust src (rsi).  */
	addq	%rdi, %rsi
	/* Loop 4x VEC at a time copy backward from src (rsi) to dst (rdi).  */
	.p2align 4
L(loop_4x_backward):
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

	cmpq	%rdi, %r8
	jb	L(loop_4x_backward)

	/* Store begining of region.  */

	/* NB: rax was set to dst on function entry.  */
	VMOVU	%VEC(3), (VEC_SIZE * 3)(%r8)
	VMOVU	%VEC(2), (VEC_SIZE * 2)(%r8)
	VMOVU	%VEC(1), (VEC_SIZE * 1)(%r8)
	VMOVU	%VEC(0), (VEC_SIZE * 0)(%r8)

	/* Store VECs loaded for aligning dst (rdi).  */
#if (LOOP_4X_ALIGN_TO > VEC_SIZE)
	VMOVU	%VEC(14), (VEC_SIZE * -2)(%r8, %rdx)
#endif
	VMOVU	%VEC(15), (VEC_SIZE * -1)(%r8, %rdx)
	VZEROUPPER_RETURN

#ifdef USE_WITH_ERMS
	.p2align 4
L(movsb):
	/* If size (rdx) > __x86_rep_movsb_stop_threshold go to large copy.  */

	/* NB: __x86_rep_movsb_stop_threshold in range
	   [__x86_rep_movsb_threshold, __x86_shared_non_temporal_threshold].  */
	cmp	__x86_rep_movsb_stop_threshold(%rip), %RDX_LP
# if (defined USE_MULTIARCH || VEC_SIZE == 16) && IS_IN (libc)
	jae	L(large_memcpy_2x)
# else
	jae	L(gt_8x_forward)
# endif
# if MOVSB_ALIGN_TO < VEC_SIZE
#  if VEC_SIZE > 16
	cmpl	$-64, %ecx
	jae	L(gt_8x_forward)
#  endif
	movq	%rdx, %rcx
	rep	movsb
	VZEROUPPER_RETURN
# elif ALIGN_MOVSB_EXCLUSIVE
#  if VEC_SIZE > 16
	cmpl	$-64, %ecx
	jae	L(gt_8x_forward)
#  endif
	subq	%rdi, %rsi
	leaq	(%rdi, %rdx), %rcx
	addq	$(MOVSB_ALIGN_TO - 1), %rdi
	andq	$-(MOVSB_ALIGN_TO), %rdi
	addq	%rdi, %rsi
	subq	%rdi, %rcx

	rep	movsb

	VMOVU	%VEC(0), (VEC_SIZE * 0)(%r8)
#  if MOVSB_ALIGN_TO > VEC_SIZE
	VMOVU	%VEC(1), (VEC_SIZE * 1)(%r8)
#  endif
	VZEROUPPER_RETURN
# else
	testl	$(PAGE_SIZE - 512), %ecx
	jnz	L(movsb_align_dst)
	movq	%rcx, %r9
	leaq	-(1)(%rsi, %rdx), %rcx
	orq	$(MOVSB_ALIGN_TO - 1), %rsi
	leaq	1(%rsi, %r9), %rdi
	subq	%rsi, %rcx
	incq	%rsi
	rep	movsb

	/* Store aligning VECs.  */

	/* NB: rax was set to dst on function entry.  */
	VMOVU	%VEC(0), (VEC_SIZE * 0)(%r8)
#  if MOVSB_ALIGN_TO > VEC_SIZE
	VMOVU	%VEC(1), (VEC_SIZE * 1)(%r8)
#  endif
	VZEROUPPER_RETURN
L(movsb_align_dst):
	subq	%rdi, %rsi
	leaq	-(1)(%rdi, %rdx), %rcx
	orq	$(MOVSB_ALIGN_TO - 1), %rdi
	leaq	1(%rdi, %rsi), %rsi
	subq	%rdi, %rcx
	incq	%rdi

	rep	movsb

	VMOVU	%VEC(0), (VEC_SIZE * 0)(%r8)
#  if MOVSB_ALIGN_TO > VEC_SIZE
	VMOVU	%VEC(1), (VEC_SIZE * 1)(%r8)
#  endif
	VZEROUPPER_RETURN
# endif
#endif


#if (defined USE_MULTIARCH || VEC_SIZE == 16) && IS_IN (libc)
	.p2align 4
L(large_memcpy_2x):
# ifdef USE_WITH_ERMS
	cmpq	__x86_shared_non_temporal_threshold(%rip), %rdx
	jb	L(gt_8x_forward)
# endif
	/* We want to use temporal copies if there is overlap constructive cache
	   interference between loading from src and storing to dst.  */

	/* rcx current has dst - src. Above logic already checked that there is
	   no overlap in the case of dst > src. Negating rcx will produce a
	   negative value (or large positive value) if dst > src (which we already
	   know has no overlap) and the src - dst if dst < src. If rdx is greater
	   than negated rcx then there is overlap in the dst < src case (so do
	   forward copy) or in the highly unlikely case that length > 2^64 the
	   temporal forward copy will still work.  */
	negq	%rcx
	cmpq	%rcx, %rdx
	ja	L(gt_8x_forward)

	/* Cache align destination. First store the first 64 bytes then adjust
	   alignments.  */
# if MOVSB_ALIGN_TO < VEC_SIZE
	VMOVU	(%rsi), %VEC(0)
# endif
# if VEC_SIZE < 64
#  if MOVSB_ALIGN_TO < (2 * VEC_SIZE)
	VMOVU	VEC_SIZE(%rsi), %VEC(1)
#  endif
#  if VEC_SIZE < 32
	VMOVU	(VEC_SIZE * 2)(%rsi), %VEC(2)
	VMOVU	(VEC_SIZE * 3)(%rsi), %VEC(3)
#  endif
# endif

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
	incl	%ecx
	testl	$(PAGE_SIZE - VEC_SIZE * 8), %ecx
	jz	L(large_memcpy_4x)

	movq	%rdx, %r10
	shrq	$LOG_4X_MEMCPY_THRESH, %r10
	cmp	__x86_shared_non_temporal_threshold(%rip), %r10
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
END (MEMMOVE_SYMBOL (__memmove, unaligned))

#if IS_IN (libc)
# ifdef SHARED
	strong_alias (MEMMOVE_CHK_SYMBOL (__memmove_chk, unaligned),
	MEMMOVE_CHK_SYMBOL (__memcpy_chk, unaligned))
# endif
#endif
	strong_alias (MEMMOVE_SYMBOL (__memmove, unaligned),
	MEMCPY_SYMBOL (__memcpy, unaligned))
