#include "asm-common.h"
#define USE_MULTIARCH	1
#define USE_WITH_MULTIARCH_AND_LIBC
#define X86_STRING_CONTROL_AVOID_SHORT_DISTANCE_REP_MOVSB	1
#ifndef VZEROUPPER
# include "../avx2-vecs.h"
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
#define ALIGN_MOVSB	(VEC_SIZE	>	16)
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

#define COPY_BLOCK(mov_inst,	src_reg,	dst_reg,	size_reg,	len,	tmp_reg0,	tmp_reg1)	\
	mov_inst (%src_reg), %tmp_reg0; \
	mov_inst -(len)(%src_reg, %size_reg), %tmp_reg1; \
	mov_inst %tmp_reg0, (%dst_reg); \
	mov_inst %tmp_reg1, -(len)(%dst_reg, %size_reg);


#define COPY_4_8	COPY_BLOCK(movl,	rsi,	rdi,	rdx,	4,	ecx,	esi)
#define COPY_8_16	COPY_BLOCK(movq,	rsi,	rdi,	rdx,	8,	rcx,	rsi)
#define COPY_16_32	COPY_BLOCK(vmovdqu,	rsi,	rdi,	rdx,	16,	xmm0,	xmm1)
#define COPY_32_64	COPY_BLOCK(vmovdqu64,	rsi,	rdi,	rdx,	32,	ymm16,	ymm17)

	/* Whether to align before movsb. Ultimately we want 64 byte align
	   and not worth it to load 4x VEC for VEC_SIZE == 16.  */
#define ALIGN_MOVSB	(VEC_SIZE	>	16)

	/* Number of VECs to align movsb to.  */
#if VEC_SIZE == 64
# define MOVSB_ALIGN_TO	(VEC_SIZE)
#else
# define MOVSB_ALIGN_TO	(VEC_SIZE	*	2)
#endif

	.section SECTION(.text), "ax", @progbits
ENTRY(MEMCPY)
	movq	%rdi, %rax
L(start_erms):
#ifdef ILP32
	/* Clear the upper 32 bits.  */
	movl	%edx, %edx
#endif
	cmp	$VEC_SIZE, %RDX_LP
	/* Based on SPEC2017 distribution both 16 and 32 memcpy calls are
	   really hot so we want them to take the same branch path.  */
#if VEC_SIZE > 16
	jbe	L(less_vec)
#else
	jb	L(less_vec)
#endif
	cmp	$(VEC_SIZE * 2), %RDX_LP
#define USE_MOVSB
#ifdef USE_MOVSB
	ja	L(movsb_more_2x_vec)
#else
	ja	L(more_2x_vec)
#endif
L(last_2x_vec):
	/* From VEC and to 2 * VEC.  No branch when size == VEC_SIZE.  */
	VMOVU	(%rsi), %VEC(0)
	VMOVU	-VEC_SIZE(%rsi, %rdx), %VEC(1)
	VMOVU	%VEC(0), (%rdi)
	VMOVU	%VEC(1), -VEC_SIZE(%rdi, %rdx)
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

L(copy_33_63):
	COPY_32_64
	ret
#endif
	/* Only worth aligning if near end of 16 byte block and won't get
	   first branch in first decode after jump.  */
	.p2align 4,, 6
L(less_vec):
#if VEC_SIZE != 16 && VEC_SIZE != 32 && VEC_SIZE != 64
# error Unsupported VEC_SIZE!
#endif
	/* Second set of branches for smallest copies.  */
	cmpl	$(VEC_SIZE / 4), %edx
	jb	L(less_quarter_vec)

	cmpl	$(VEC_SIZE / 2), %edx
#if VEC_SIZE == 64
	/* We branch to [33, 63] instead of [16, 32] to give [16, 32] fall
	   through path as [16, 32] is hotter.  */
	ja	L(copy_33_63)
	COPY_16_32
#elif VEC_SIZE == 32
	/* Branch to [8, 15]. Fall through to [16, 32].  */
	jb	L(copy_8_15)
	COPY_16_32
#else
	/* Branch to [4, 7]. Fall through to [8, 15].  */
	jb	L(copy_4_7)
	COPY_8_16
#endif
	ret
	/* Align if won't cost too many bytes.  */
	.p2align 4,, 6
L(copy_4_7):
	COPY_4_8
	ret

	/* Cold target. No need to align.  */
L(copy_1):
	movzbl	(%rsi), %ecx
	movb	%cl, (%rdi)
	ret

	/* Colder copy case for [0, VEC_SIZE / 4 - 1].  */
L(less_quarter_vec):
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
	/* Fall through into copy [2, 3] as it is more common than [0, 1].
	 */
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
	/* COPY_8_16 is exactly 17 bytes so don't want to p2align after as
	   it wastes 15 bytes of code and 1 byte off is fine.  */
#endif

#if defined USE_MULTIARCH && 1
L(movsb):
	movq	%rdi, %rcx
	subq	%rsi, %rcx
	/* Go to backwards temporal copy if overlap no matter what as
	   backward movsb is slow.  */
	cmpq	%rdx, %rcx
	/* L(more_8x_vec_backward_check_nop) checks for src == dst.  */
	jb	L(more_8x_vec_backward_check_nop)
	/* If above x86_rep_movsb_stop_threshold most likely is candidate
	   for NT moves aswell.  */
	cmp	x86_rep_movsb_stop_threshold(%rip), %RDX_LP
	jae	L(large_memcpy_2x_check)
# if ALIGN_MOVSB
	VMOVU	(%rsi), %VEC(0)
#  if MOVSB_ALIGN_TO > VEC_SIZE
	VMOVU	VEC_SIZE(%rsi), %VEC(1)
#  endif
#  if MOVSB_ALIGN_TO > (VEC_SIZE * 2)
#   error Unsupported MOVSB_ALIGN_TO
#  endif
	/* Store dst for use after rep movsb.  */
	movq	%rdi, %r8
# endif
# if AVOID_SHORT_DISTANCE_REP_MOVSB
	/* Only avoid short movsb if CPU has FSRM.  */

	testl	$X86_STRING_CONTROL_AVOID_SHORT_DISTANCE_REP_MOVSB, x86_string_control(%rip)
	jz	L(skip_short_movsb_check)
	/* Avoid "rep movsb" if RCX, the distance between source and
	   destination, is N*4GB + [1..63] with N >= 0.  */

	/* ecx contains dst - src. Early check for backward copy conditions
	   means only case of slow movsb with src = dst + [0, 63] is ecx in
	   [-63, 0]. Use unsigned comparison with -64 check for that case.  */
	cmpl	$-64, %ecx
	ja	L(more_8x_vec_forward)
# endif
# if ALIGN_MOVSB
	/* Fall through means cpu has FSRM. In that case exclusively align
	   destination.  */

	/* Subtract dst from src. Add back after dst aligned.  */
	subq	%rdi, %rsi
	/* Add dst to len. Subtract back after dst aligned.  */
	leaq	(%rdi, %rdx), %rcx
	/* Exclusively align dst to MOVSB_ALIGN_TO (64).  */
	addq	$(MOVSB_ALIGN_TO - 1), %rdi
	andq	$-(MOVSB_ALIGN_TO), %rdi
	/* Restore src and len adjusted with new values for aligned dst.  */
	addq	%rdi, %rsi
	subq	%rdi, %rcx

	rep	movsb
	VMOVU	%VEC(0), (%r8)
#  if MOVSB_ALIGN_TO > VEC_SIZE
	VMOVU	%VEC(1), VEC_SIZE(%r8)
#  endif
	VZEROUPPER_RETURN
L(movsb_align_dst):
	/* Subtract dst from src. Add back after dst aligned.  */
	subq	%rdi, %rsi
	/* Add dst to len. Subtract back after dst aligned. -1 because dst
	   is initially aligned to MOVSB_ALIGN_TO - 1.  */
	leaq	-(1)(%rdi, %rdx), %rcx
	/* Inclusively align dst to MOVSB_ALIGN_TO - 1.  */
	orq	$(MOVSB_ALIGN_TO - 1), %rdi
	leaq	1(%rdi, %rsi), %rsi
	/* Restore src and len adjusted with new values for aligned dst.  */
	subq	%rdi, %rcx
	/* Finish aligning dst.  */
	incq	%rdi
	rep	movsb
	VMOVU	%VEC(0), (%r8)
#  if MOVSB_ALIGN_TO > VEC_SIZE
	VMOVU	%VEC(1), VEC_SIZE(%r8)
#  endif
	VZEROUPPER_RETURN

L(skip_short_movsb_check):
	/* If CPU does not have FSRM two options for aligning. Align src if
	   dst and src 4k alias. Otherwise align dst.  */
	testl	$(PAGE_SIZE - 512), %ecx
	jnz	L(movsb_align_dst)
	/* rcx already has dst - src.  */
	movq	%rcx, %r9
	/* Add src to len. Subtract back after src aligned. -1 because src
	   is initially aligned to MOVSB_ALIGN_TO - 1.  */
	leaq	-(1)(%rsi, %rdx), %rcx
	/* Inclusively align src to MOVSB_ALIGN_TO - 1.  */
	orq	$(MOVSB_ALIGN_TO - 1), %rsi
	/* Restore dst and len adjusted with new values for aligned dst.  */
	leaq	1(%rsi, %r9), %rdi
	subq	%rsi, %rcx
	/* Finish aligning src.  */
	incq	%rsi
	rep	movsb
	VMOVU	%VEC(0), (%r8)
#  if MOVSB_ALIGN_TO > VEC_SIZE
	VMOVU	%VEC(1), VEC_SIZE(%r8)
#  endif
	VZEROUPPER_RETURN
# else
	/* Not alignined rep movsb so just copy.  */
	mov	%RDX_LP, %RCX_LP
	rep	movsb
	ret
# endif
#endif
	/* Align if doesn't cost too many bytes.  */
	.p2align 4,, 6
#if defined USE_MULTIARCH && 1
L(movsb_more_2x_vec):
	cmp	x86_rep_movsb_threshold(%rip), %RDX_LP
	ja	L(movsb)
#endif
L(more_2x_vec):
	/* More than 2 * VEC and there may be overlap between destination
	   and source.  */
	cmpq	$(VEC_SIZE * 8), %rdx
	ja	L(more_8x_vec)
	cmpq	$(VEC_SIZE * 4), %rdx
	jbe	L(last_4x_vec)
	/* Copy from 4 * VEC + 1 to 8 * VEC, inclusively.  */
	VMOVU	(%rsi), %VEC(0)
	VMOVU	VEC_SIZE(%rsi), %VEC(1)
	VMOVU	(VEC_SIZE * 2)(%rsi), %VEC(2)
	VMOVU	(VEC_SIZE * 3)(%rsi), %VEC(3)
	VMOVU	-VEC_SIZE(%rsi, %rdx), %VEC(4)
	VMOVU	-(VEC_SIZE * 2)(%rsi, %rdx), %VEC(5)
	VMOVU	-(VEC_SIZE * 3)(%rsi, %rdx), %VEC(6)
	VMOVU	-(VEC_SIZE * 4)(%rsi, %rdx), %VEC(7)
	VMOVU	%VEC(0), (%rdi)
	VMOVU	%VEC(1), VEC_SIZE(%rdi)
	VMOVU	%VEC(2), (VEC_SIZE * 2)(%rdi)
	VMOVU	%VEC(3), (VEC_SIZE * 3)(%rdi)
	VMOVU	%VEC(4), -VEC_SIZE(%rdi, %rdx)
	VMOVU	%VEC(5), -(VEC_SIZE * 2)(%rdi, %rdx)
	VMOVU	%VEC(6), -(VEC_SIZE * 3)(%rdi, %rdx)
	VMOVU	%VEC(7), -(VEC_SIZE * 4)(%rdi, %rdx)
	VZEROUPPER_RETURN
	/* Align if doesn't cost too much code size. 6 bytes so that after
	   jump to target a full mov instruction will always be able to be
	   fetched.  */
	.p2align 4,, 6
L(last_4x_vec):
	/* Copy from 2 * VEC + 1 to 4 * VEC, inclusively.  */
	VMOVU	(%rsi), %VEC(0)
	VMOVU	VEC_SIZE(%rsi), %VEC(1)
	VMOVU	-VEC_SIZE(%rsi, %rdx), %VEC(2)
	VMOVU	-(VEC_SIZE * 2)(%rsi, %rdx), %VEC(3)
	VMOVU	%VEC(0), (%rdi)
	VMOVU	%VEC(1), VEC_SIZE(%rdi)
	VMOVU	%VEC(2), -VEC_SIZE(%rdi, %rdx)
	VMOVU	%VEC(3), -(VEC_SIZE * 2)(%rdi, %rdx)
	/* Keep nop target close to jmp for 2-byte encoding.  */
L(nop):
	VZEROUPPER_RETURN
	/* Align if doesn't cost too much code size.  */
	.p2align 4,, 10
L(more_8x_vec):
	/* Check if non-temporal move candidate.  */
#if (defined USE_MULTIARCH || VEC_SIZE == 16) && 1
	/* Check non-temporal store threshold.  */
	cmp	x86_shared_non_temporal_threshold(%rip), %RDX_LP
	ja	L(large_memcpy_2x)
#endif
	/* Entry if rdx is greater than non-temporal threshold but there is
	   overlap.  */
L(more_8x_vec_check):
	cmpq	%rsi, %rdi
	ja	L(more_8x_vec_backward)
	/* Source == destination is less common.  */
	je	L(nop)
	/* Entry if rdx is greater than movsb or stop movsb threshold but
	   there is overlap with dst > src.  */
L(more_8x_vec_forward):
	/* Load the first VEC and last 4 * VEC to support overlapping
	   addresses.  */
	VMOVU	(%rsi), %VEC(4)
	VMOVU	-VEC_SIZE(%rsi, %rdx), %VEC(5)
	VMOVU	-(VEC_SIZE * 2)(%rsi, %rdx), %VEC(6)
	VMOVU	-(VEC_SIZE * 3)(%rsi, %rdx), %VEC(7)
	VMOVU	-(VEC_SIZE * 4)(%rsi, %rdx), %VEC(8)
	/* Subtract dst from src. Add back after dst aligned.  */
	subq	%rdi, %rsi
	/* Store end of buffer minus tail in rdx.  */
	leaq	(VEC_SIZE * -4)(%rdi, %rdx), %rdx
	/* Save begining of dst.  */
	movq	%rdi, %rcx
	/* Align dst to VEC_SIZE - 1.  */
	orq	$(VEC_SIZE - 1), %rdi
	/* Restore src adjusted with new value for aligned dst.  */
	leaq	1(%rdi, %rsi), %rsi
	/* Finish aligning dst.  */
	incq	%rdi
	.p2align 4
L(loop_4x_vec_forward):
	/* Copy 4 * VEC a time forward.  */
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
	cmpq	%rdi, %rdx
	ja	L(loop_4x_vec_forward)
	/* Store the last 4 * VEC.  */
	VMOVU	%VEC(5), (VEC_SIZE * 3)(%rdx)
	VMOVU	%VEC(6), (VEC_SIZE * 2)(%rdx)
	VMOVU	%VEC(7), VEC_SIZE(%rdx)
	VMOVU	%VEC(8), (%rdx)
	/* Store the first VEC.  */
	VMOVU	%VEC(4), (%rcx)
	/* Keep nop target close to jmp for 2-byte encoding.  */
L(nop2):
	VZEROUPPER_RETURN
	/* Entry from fail movsb. Need to test if dst - src == 0 still.  */
L(more_8x_vec_backward_check_nop):
	testq	%rcx, %rcx
	jz	L(nop2)
L(more_8x_vec_backward):
	/* Load the first 4 * VEC and last VEC to support overlapping
	   addresses.  */
	VMOVU	(%rsi), %VEC(4)
	VMOVU	VEC_SIZE(%rsi), %VEC(5)
	VMOVU	(VEC_SIZE * 2)(%rsi), %VEC(6)
	VMOVU	(VEC_SIZE * 3)(%rsi), %VEC(7)
	VMOVU	-VEC_SIZE(%rsi, %rdx), %VEC(8)
	/* Subtract dst from src. Add back after dst aligned.  */
	subq	%rdi, %rsi
	/* Save begining of buffer.  */
	movq	%rdi, %rcx
	/* Set dst to begining of region to copy. -1 for inclusive
	   alignment.  */
	leaq	(VEC_SIZE * -4 + -1)(%rdi, %rdx), %rdi
	/* Align dst.  */
	andq	$-(VEC_SIZE), %rdi
	/* Restore src.  */
	addq	%rdi, %rsi
	/* Don't use multi-byte nop to align.  */
	.p2align 4,, 11
L(loop_4x_vec_backward):
	/* Copy 4 * VEC a time backward.  */
	VMOVU	(VEC_SIZE * 3)(%rsi), %VEC(0)
	VMOVU	(VEC_SIZE * 2)(%rsi), %VEC(1)
	VMOVU	(VEC_SIZE * 1)(%rsi), %VEC(2)
	VMOVU	(VEC_SIZE * 0)(%rsi), %VEC(3)
	addq	$(VEC_SIZE * -4), %rsi
	VMOVA	%VEC(0), (VEC_SIZE * 3)(%rdi)
	VMOVA	%VEC(1), (VEC_SIZE * 2)(%rdi)
	VMOVA	%VEC(2), (VEC_SIZE * 1)(%rdi)
	VMOVA	%VEC(3), (VEC_SIZE * 0)(%rdi)
	addq	$(VEC_SIZE * -4), %rdi
	cmpq	%rdi, %rcx
	jb	L(loop_4x_vec_backward)
	/* Store the first 4 * VEC.  */
	VMOVU	%VEC(4), (%rcx)
	VMOVU	%VEC(5), VEC_SIZE(%rcx)
	VMOVU	%VEC(6), (VEC_SIZE * 2)(%rcx)
	VMOVU	%VEC(7), (VEC_SIZE * 3)(%rcx)
	/* Store the last VEC.  */
	VMOVU	%VEC(8), -VEC_SIZE(%rdx, %rcx)
	VZEROUPPER_RETURN

#if (defined USE_MULTIARCH || VEC_SIZE == 16) && 1
	.p2align 4
	/* Entry if dst > stop movsb threshold (usually set to non-temporal
	   threshold).  */
L(large_memcpy_2x_check):
	cmp	x86_shared_non_temporal_threshold(%rip), %RDX_LP
	jb	L(more_8x_vec_forward)
L(large_memcpy_2x):
	/* Compute absolute value of difference between source and
	   destination.  */
	movq	%rdi, %r9
	subq	%rsi, %r9
	movq	%r9, %r8
	leaq	-1(%r9), %rcx
	sarq	$63, %r8
	xorq	%r8, %r9
	subq	%r8, %r9
	/* Don't use non-temporal store if there is overlap between
	   destination and source since destination may be in cache when source
	   is loaded.  */
	cmpq	%r9, %rdx
	ja	L(more_8x_vec_check)

	/* Cache align destination. First store the first 64 bytes then
	   adjust alignments.  */
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

	/* Test if source and destination addresses will alias. If they do
	   the larger pipeline in large_memcpy_4x alleviated the performance
	   drop.  */
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
	/* Only one prefetch set per page as doing 4 pages give more time
	   for prefetcher to keep up.  */
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
x86_string_control:
	.quad	(1)
	.previous
END(MEMCPY)
