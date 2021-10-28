#include "asm-common.h"
#define USE_MULTIARCH	1
#define USE_WITH_MULTIARCH_AND_LIBC
#define X86_STRING_CONTROL_AVOID_SHORT_DISTANCE_REP_MOVSB	1
#ifndef VZEROUPPER
# include "../evex-vecs.h"
#endif
#if MOVSB_THRESHOLD == 4096
# define SMALL_MOV_SIZE	1
#else
# define LARGE_MOV_SIZE	1
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

#define COPY_BLOCK(mov_inst,	src_reg,	dst_reg,	size_reg,	len,	tmp_reg0,	tmp_reg1)	\
	mov_inst (%src_reg), %tmp_reg0; \
	mov_inst -(len)(%src_reg, %size_reg), %tmp_reg1; \
	mov_inst %tmp_reg0, (%dst_reg); \
	mov_inst %tmp_reg1, -(len)(%dst_reg, %size_reg);


#define COPY_4_8	COPY_BLOCK(movl,	rsi,	rdi,	rdx,	4,	ecx,	esi)
#define COPY_8_16	COPY_BLOCK(movq,	rsi,	rdi,	rdx,	8,	rcx,	rsi)
#define COPY_16_32	COPY_BLOCK(vmovdqu,	rsi,	rdi,	rdx,	16,	xmm0,	xmm1)
#define COPY_32_64	COPY_BLOCK(vmovdqu64,	rsi,	rdi,	rdx,	32,	ymm16,	ymm17)

	/* Whether to align before movsb. Ultimately we want 64 byte
	   align and not worth it to load 4x VEC for VEC_SIZE == 16.  */
#define ALIGN_MOVSB	0	&&	(VEC_SIZE	>	16)

	/* Number of VECs to align movsb to.  */
#if VEC_SIZE == 64
# define MOVSB_ALIGN_TO	(VEC_SIZE)
# define ALIAS_SIZE	256
#else
# define MOVSB_ALIGN_TO	(VEC_SIZE	*	2)
# define ALIAS_SIZE	(VEC_SIZE	*	8)
#endif
#if VEC_SIZE == 16
# define ALIGN_TO	(VEC_SIZE	*	2)
#else
# define ALIGN_TO	(VEC_SIZE	*	1)
#endif

	.section SECTION(.text), "ax", @progbits
#if 1
ENTRY(MEMCPY)
	movq	%rdi, %rax
L(start_erms):
# ifdef __ILP32__
	/* Clear the upper 32 bits.  */
	movl	%edx, %edx
# endif
	cmp	$VEC_SIZE, %RDX_LP
	jb	L(less_vec)
	VMOVU	(%rsi), %VEC(4)
	cmp	$(VEC_SIZE * 2), %RDX_LP
	ja	L(movsb_more_2x_vec)
L(last_2x_vec):
	/* From VEC and to 2 * VEC.  No branch when size == VEC_SIZE.
	 */
	VMOVU	-VEC_SIZE(%rsi, %rdx), %VEC(1)
	VMOVU	%VEC(4), (%rdi)
	VMOVU	%VEC(1), -VEC_SIZE(%rdi, %rdx)
L(return):
# if VEC_SIZE > 16
	ZERO_UPPER_VEC_REGISTERS_RETURN
# else
	ret
# endif
#endif


#if MOVSB_THRESHOLD != 4096
	.p2align 4,, 8
L(between_4_7):
	/* From 4 to 7.  No branch when size == 4.  */
	movl	(%rsi), %ecx
	movl	(%rsi, %rdx), %esi
	movl	%ecx, (%rdi)
	movl	%esi, (%rdi, %rdx)
	ret
#endif
	.p2align 4
L(less_vec):
	/* Less than 1 VEC.  */
#if VEC_SIZE != 16 && VEC_SIZE != 32 && VEC_SIZE != 64
# error Unsupported VEC_SIZE!
#endif
#if VEC_SIZE > 32
	cmpl	$32, %edx
	jae	L(between_32_63)
#endif
#if VEC_SIZE > 16
	cmpl	$16, %edx
	jae	L(between_16_31)
#endif
	cmpl	$8, %edx
	jae	L(between_8_15)
#if MOVSB_THRESHOLD == 4096
	cmpl	$4, %edx
	jae	L(between_4_7)
	cmpl	$1, %edx
	jb	L(copy_0)
	movb	(%rsi), %cl
	je	L(copy_1)
	movzwl	-2(%rsi, %rdx), %esi
	movw	%si, -2(%rdi, %rdx)
L(copy_1):
	movb	%cl, (%rdi)
#else
	subq	$4, %rdx
	jae	L(between_4_7)
	cmpl	$-3, %edx
	jl	L(copy_0)
	movb	(%rsi), %cl
	je	L(copy_1)
	movzwl	2(%rsi, %rdx), %esi
	movw	%si, 2(%rdi, %rdx)
L(copy_1):
	movb	%cl, (%rdi)
#endif
L(copy_0):
	ret

#if MOVSB_THRESHOLD == 4096
	.p2align 4,, 8
L(between_4_7):
	/* From 4 to 7.  No branch when size == 4.  */
	movl	(%rsi), %ecx
	movl	-4(%rsi, %rdx), %esi
	movl	%ecx, (%rdi)
	movl	%esi, -4(%rdi, %rdx)
	ret
#endif

#if VEC_SIZE > 16
	/* From 16 to 31.  No branch when size == 16.  */
	.p2align 4,, 8
L(between_16_31):
	vmovdqu	(%rsi), %xmm0
	vmovdqu	-16(%rsi, %rdx), %xmm1
	vmovdqu	%xmm0, (%rdi)
	vmovdqu	%xmm1, -16(%rdi, %rdx)
	ret
#endif

	.p2align 4,, 10
L(between_8_15):
	/* From 8 to 15.  No branch when size == 8.  */
	movq	(%rsi), %rcx
	movq	-8(%rsi, %rdx), %rsi
	movq	%rcx, (%rdi)
	movq	%rsi, -8(%rdi, %rdx)
	ret


#if VEC_SIZE > 32
	.p2align 4,, 10
L(between_32_63):
	/* From 32 to 63.  No branch when size == 32.  */
	VMOVU	(%rsi), %YMM0
	VMOVU	-32(%rsi, %rdx), %YMM1
	VMOVU	%YMM0, (%rdi)
	VMOVU	%YMM1, -32(%rdi, %rdx)
	VZEROUPPER_RETURN
#endif

	.p2align 4,, 10
L(last_4x_vec):
	/* Copy from 2 * VEC + 1 to 4 * VEC, inclusively.  */
	VMOVU	-VEC_SIZE(%rsi, %rdx), %VEC(2)
	VMOVU	-(VEC_SIZE * 2)(%rsi, %rdx), %VEC(3)
	VMOVU	%VEC(4), (%rdi)
	VMOVU	%VEC(1), VEC_SIZE(%rdi)
	VMOVU	%VEC(2), -VEC_SIZE(%rdi, %rdx)
	VMOVU	%VEC(3), -(VEC_SIZE * 2)(%rdi, %rdx)
	VZEROUPPER_RETURN

	.p2align 4
#if defined USE_MULTIARCH && IS_IN (libc)
L(movsb_more_2x_vec):
	cmp	$MOVSB_THRESHOLD, %RDX_LP
	ja	L(movsb)
#endif
L(more_2x_vec):
	/* More than 2 * VEC and there may be overlap between
	   destination and source.  */
	cmpq	$(VEC_SIZE * 8), %rdx
	ja	L(more_8x_vec)
	/* Load regardless.  */
	VMOVU	VEC_SIZE(%rsi), %VEC(1)
	cmpq	$(VEC_SIZE * 4), %rdx
	jbe	L(last_4x_vec)
	/* Copy from 4 * VEC + 1 to 8 * VEC, inclusively.  */
	VMOVU	(VEC_SIZE * 2)(%rsi), %VEC(2)
	VMOVU	(VEC_SIZE * 3)(%rsi), %VEC(3)
	VMOVU	-VEC_SIZE(%rsi, %rdx), %VEC(0)
	VMOVU	-(VEC_SIZE * 2)(%rsi, %rdx), %VEC(5)
	VMOVU	-(VEC_SIZE * 3)(%rsi, %rdx), %VEC(6)
	VMOVU	-(VEC_SIZE * 4)(%rsi, %rdx), %VEC(7)
	VMOVU	%VEC(4), (%rdi)
	VMOVU	%VEC(1), VEC_SIZE(%rdi)
	VMOVU	%VEC(2), (VEC_SIZE * 2)(%rdi)
	VMOVU	%VEC(3), (VEC_SIZE * 3)(%rdi)
	VMOVU	%VEC(0), -VEC_SIZE(%rdi, %rdx)
	VMOVU	%VEC(5), -(VEC_SIZE * 2)(%rdi, %rdx)
	VMOVU	%VEC(6), -(VEC_SIZE * 3)(%rdi, %rdx)
	VMOVU	%VEC(7), -(VEC_SIZE * 4)(%rdi, %rdx)
	VZEROUPPER_RETURN


	.p2align 4,, 4
L(more_8x_vec):
	movq	%rdi, %rcx
	subq	%rsi, %rcx
	/* Go to backwards temporal copy if overlap no matter what as
	   backward REP MOVSB is slow and we don't want to use NT stores if
	   there is overlap.  */
	cmpq	%rdx, %rcx
	/* L(more_8x_vec_backward_check_nop) checks for src == dst.  */
	jb	L(more_8x_vec_backward_check_nop)
	/* Check if non-temporal move candidate.  */
#if (defined USE_MULTIARCH || VEC_SIZE == 16) && IS_IN (libc)
	/* Check non-temporal store threshold.  */
	// cmp     x86_shared_non_temporal_threshold(%rip), %RDX_LP
	cmp	$LARGE_MEMCPY_THRESHOLD, %RDX_LP
	ja	L(large_memcpy_2x)
#endif
	/* To reach this point there cannot be overlap and dst > src. So
	   check for overlap and src > dst in which case correctness
	   requires forward copy. Otherwise decide between backward/forward
	   copy depending on address aliasing.  */

	/* Entry if rdx is greater than x86_rep_movsb_stop_threshold but
	   less than x86_shared_non_temporal_threshold.  */
L(more_8x_vec_check):
	/* rcx contains dst - src. Add back length (rdx).  */
	leaq	(%rcx, %rdx), %r8
	/* If r8 has different sign than rcx then there is overlap so we
	   must do forward copy.  */
	xorq	%rcx, %r8
	/* Isolate just sign bit of r8.  */
	shrq	$63, %r8
	/* Get 4k difference dst - src.  */
	andl	$(PAGE_SIZE - 256), %ecx
	/* If r8 is non-zero must do foward for correctness. Otherwise
	   if ecx is non-zero there is 4k False Alaising so do backward
	   copy.  */
	addl	%r8d, %ecx
	jz	L(more_8x_vec_backward)

	/* if rdx is greater than x86_shared_non_temporal_threshold but
	   there is overlap, or from short distance movsb.  */
L(more_8x_vec_forward):
	/* Load first and last 4 * VEC to support overlapping addresses.
	 */
	VMOVU	-VEC_SIZE(%rsi, %rdx), %VEC(5)
	VMOVU	-(VEC_SIZE * 2)(%rsi, %rdx), %VEC(6)
	/* Save begining of dst.  */
	movq	%rdi, %rcx
	/* Align dst to VEC_SIZE - 1.  */
	orq	$(VEC_SIZE - 1), %rdi
	VMOVU	-(VEC_SIZE * 3)(%rsi, %rdx), %VEC(7)
	VMOVU	-(VEC_SIZE * 4)(%rsi, %rdx), %VEC(8)

	/* Align dst.  */

	/* Subtract dst from src. Add back after dst aligned.  */
	subq	%rcx, %rsi

	/* Restore src adjusted with new value for aligned dst.  */
	// leaq    1(%rdi, %rsi), %rsi
	/* Finish aligning dst.  */
	incq	%rdi
	addq	%rdi, %rsi
	/* Store end of buffer minus tail in rdx.  */
	leaq	(VEC_SIZE * -4)(%rcx, %rdx), %rdx

	/* Dont use multi-byte nop to align.  */
	.p2align 4,, 11
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
	/* Keep L(nop_backward) target close to jmp for 2-byte encoding.
	 */
L(nop_backward):
	VZEROUPPER_RETURN

	.p2align 4,, 8
L(more_8x_vec_backward_check_nop):
	testq	%rcx, %rcx
	jz	L(nop_backward)
L(more_8x_vec_backward):
	/* Load the first 4 * VEC and last VEC to support overlapping
	   addresses.  */
	VMOVU	VEC_SIZE(%rsi), %VEC(5)
	VMOVU	(VEC_SIZE * 2)(%rsi), %VEC(6)
	/* Begining of region for 4x backward copy stored in rcx.  */
	leaq	(VEC_SIZE * -4 + -1)(%rdi, %rdx), %rcx
	VMOVU	(VEC_SIZE * 3)(%rsi), %VEC(7)
	VMOVU	-VEC_SIZE(%rsi, %rdx), %VEC(8)
	/* Subtract dst from src. Add back after dst aligned.  */
	subq	%rdi, %rsi
	/* Align dst.  */
	andq	$-(VEC_SIZE), %rcx
	/* Restore src.  */
	addq	%rcx, %rsi

	/* Don't use multi-byte nop to align.  */
	.p2align 4,, 11
L(loop_4x_vec_backward):
	/* Copy 4 * VEC a time backward.  */
	VMOVU	(VEC_SIZE * 3)(%rsi), %VEC(0)
	VMOVU	(VEC_SIZE * 2)(%rsi), %VEC(1)
	VMOVU	(VEC_SIZE * 1)(%rsi), %VEC(2)
	VMOVU	(VEC_SIZE * 0)(%rsi), %VEC(3)
	addq	$(VEC_SIZE * -4), %rsi
	VMOVA	%VEC(0), (VEC_SIZE * 3)(%rcx)
	VMOVA	%VEC(1), (VEC_SIZE * 2)(%rcx)
	VMOVA	%VEC(2), (VEC_SIZE * 1)(%rcx)
	VMOVA	%VEC(3), (VEC_SIZE * 0)(%rcx)
	addq	$(VEC_SIZE * -4), %rcx
	cmpq	%rcx, %rdi
	jb	L(loop_4x_vec_backward)
	/* Store the first 4 * VEC.  */
	VMOVU	%VEC(4), (%rdi)
	VMOVU	%VEC(5), VEC_SIZE(%rdi)
	VMOVU	%VEC(6), (VEC_SIZE * 2)(%rdi)
	VMOVU	%VEC(7), (VEC_SIZE * 3)(%rdi)
	/* Store the last VEC.  */
	VMOVU	%VEC(8), -VEC_SIZE(%rdx, %rdi)
	VZEROUPPER_RETURN


	.p2align 5,, 16
	/* NB: The size of the L(skip_short_movsb_check) and L(movsb)
	   blocks impacts the alignment of the L(more_2x_vec) and
	   L(more_8x_vec) cases. If this block is changes ensure no
	   performance degragragation for sizes running out of those
	   sections.  */
#if ALIGN_MOVSB
L(skip_short_movsb_check):
# if MOVSB_ALIGN_TO > VEC_SIZE
	VMOVU	VEC_SIZE(%rsi), %VEC(5)
# endif
# if MOVSB_ALIGN_TO > (VEC_SIZE * 2)
#  error Unsupported MOVSB_ALIGN_TO
# endif
	/* rcx already has dst - src.  */
	movq	%rcx, %r9
	/* Add src to len. Subtract back after src aligned. -1 because
	   src is initially aligned to MOVSB_ALIGN_TO - 1.  */
	leaq	-(1)(%rsi, %rdx), %rcx
	/* Inclusively align src to MOVSB_ALIGN_TO - 1.  */
	orq	$(MOVSB_ALIGN_TO - 1), %rsi
	/* Restore dst and len adjusted with new values for aligned dst.
	 */
	leaq	1(%rsi, %r9), %rdi
	subq	%rsi, %rcx
	/* Finish aligning src.  */
	incq	%rsi

	rep	movsb

	VMOVU	%VEC(4), (%r8)
# if MOVSB_ALIGN_TO > VEC_SIZE
	VMOVU	%VEC(5), VEC_SIZE(%r8)
# endif
	VZEROUPPER_RETURN
#endif
	.p2align 4
#if defined USE_MULTIARCH && IS_IN (libc)
L(movsb):
	movq	%rdi, %rcx
	subq	%rsi, %rcx
	/* Go to backwards temporal copy if overlap no matter what as
	   backward REP MOVSB is slow and we don't want to use NT stores if
	   there is overlap.  */
	cmpq	%rdx, %rcx
	/* L(more_8x_vec_backward_check_nop) checks for src == dst.  */
	jb	L(more_8x_vec_backward_check_nop)
	/* If above x86_rep_movsb_stop_threshold most likely is
	   candidate for NT moves aswell.  */
	cmp	$LARGE_MEMCPY_THRESHOLD, %RDX_LP
	jae	L(large_memcpy_2x_check)

	/* Check for 4k aliasing.  */

	/* rcx contains dst - src. Add back length (rdx).  */
	leaq	(%rcx, %rdx), %r8
	/* If r8 has different sign than rcx then there is overlap so we
	   must do forward copy.  */
	xorq	%rcx, %r8
	/* Isolate just sign bit of r8.  */
	shrq	$(63 - 9), %r8
	decl	%ecx
	orl	%ecx, %r8d
	andl	$(PAGE_SIZE - 512), %r8d
	jz	L(more_8x_vec_backward)
# if ALIGN_MOVSB
	/* Save dest for storing aligning VECs later.  */
	movq	%rdi, %r8
# endif
# if AVOID_SHORT_DISTANCE_REP_MOVSB || ALIGN_MOVSB
	/* Only avoid short movsb if CPU has FSRM.  */
	PAD_TO_10;
	cmp	$ASDRB, %rdx
	jb	L(skip_short_movsb_check)
#  if AVOID_SHORT_DISTANCE_REP_MOVSB
	/* Avoid "rep movsb" if RCX, the distance between source and
	   destination, is N*4GB + [1..63] with N >= 0.  */

	/* ecx contains dst - src. Early check for backward copy
	   conditions means only case of slow movsb with src = dst + [0,
	   63] is ecx in [-63, 0]. Use unsigned comparison with -64 check
	   for that case.  */
	cmpl	$-64, %ecx
	ja	L(more_8x_vec_forward)
#  endif
# endif
# if ALIGN_MOVSB
#  if MOVSB_ALIGN_TO > VEC_SIZE
	VMOVU	VEC_SIZE(%rsi), %VEC(5)
#  endif
#  if MOVSB_ALIGN_TO > (VEC_SIZE * 2)
#   error Unsupported MOVSB_ALIGN_TO
#  endif
	/* Fall through means cpu has FSRM. In that case exclusively
	   align destination.  */
L(movsb_align_dst):
	/* Subtract dst from src. Add back after dst aligned.  */
	subq	%rdi, %rsi
	/* Exclusively align dst to MOVSB_ALIGN_TO (64).  */
	addq	$(MOVSB_ALIGN_TO - 1), %rdi
	/* Add dst to len. Subtract back after dst aligned.  */
	leaq	(%r8, %rdx), %rcx
	/* Finish aligning dst.  */
	andq	$-(MOVSB_ALIGN_TO), %rdi
	/* Restore src and len adjusted with new values for aligned dst.
	 */
	addq	%rdi, %rsi
	subq	%rdi, %rcx

	rep	movsb

	/* Store VECs loaded for aligning.  */
	VMOVU	%VEC(4), (%r8)
#  if MOVSB_ALIGN_TO > VEC_SIZE
	VMOVU	%VEC(5), VEC_SIZE(%r8)
#  endif
	VZEROUPPER_RETURN
# else
	/* !ALIGN_MOVSB.  */
L(skip_short_movsb_check):
	mov	%RDX_LP, %RCX_LP
	rep	movsb
	ret
# endif
#endif

	.p2align 4,, 9
#if (defined USE_MULTIARCH || VEC_SIZE == 16) && IS_IN (libc)
L(large_memcpy_2x_check):
	cmp	$LARGE_MEMCPY_THRESHOLD, %RDX_LP
	jb	L(more_8x_vec_check)
L(large_memcpy_2x):
	/* To reach this point it is impossible for dst > src and
	   overlap. Remaining to check is src > dst and overlap. rcx
	   already contains dst - src. Negate rcx to get src - dst. If
	   length > rcx then there is overlap and forward copy is best.  */
	negq	%rcx
	cmpq	%rcx, %rdx
	ja	L(more_8x_vec_forward)

	/* Cache align destination. First store the first 64 bytes then
	   adjust alignments.  */
# if VEC_SIZE < 64
	VMOVU	VEC_SIZE(%rsi), %VEC(9)
#  if VEC_SIZE < 32
	VMOVU	(VEC_SIZE * 2)(%rsi), %VEC(10)
	VMOVU	(VEC_SIZE * 3)(%rsi), %VEC(11)
#  endif
# endif
	VMOVU	%VEC(4), (%rdi)
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

	/* Test if source and destination addresses will alias. If they
	   do the larger pipeline in large_memcpy_4x alleviated the
	   performance drop.  */
	notl	%ecx
	testl	$(PAGE_SIZE - VEC_SIZE * 8), %ecx
	jz	L(large_memcpy_4x)

	movq	%rdx, %r10
	shrq	$LOG_4X_MEMCPY_THRESH, %r10
	cmp	$LARGE_MEMCPY_THRESHOLD, %r10
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
	/* Only one prefetch set per page as doing 4 pages give more
	   time for prefetcher to keep up.  */
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
END (MEMCPY)
	.section SECTION(.rodata), "aM", @progbits, 8
	.p2align 6
x86_rep_movsb_threshold:
	.quad	2112	// (2048 * 1 * ((VEC_SIZE) / 16))
x86_shared_non_temporal_threshold:
	.quad	((3 * (L3_CACHE_SIZE)) / ((NCORES) * 4))
x86_rep_movsb_stop_threshold:
	.quad	((3 * (L3_CACHE_SIZE)) / ((NCORES) * 4))
x86_string_control:
	.quad	(1)
	.previous
END(MEMCPY)
