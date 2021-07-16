#include "perf-ev-defines.h"
#if PERF_EVENTS
# if PERF_EV_WITH_BUILTINS
#  define NBUILTINS	3
# else
#  define NBUILTINS	0
# endif
# define PRG_OFFSET	(NBUILTINS	*	PERF_EV_COUNTER_WIDTH)

# if PERF_EV_COUNTER_WIDTH == 8
#  define NEG	negq
#  define STORE_EV_START(ev_val,	offset)	\
	movl	$(ev_val), %ecx; \
	rdpmc;	\
	salq	$32, %rdx; \
	orq	%rdx, %rax; \
	movq	%rax, (offset)(%rdi)

#  define STORE_EV_END(ev_val,	offset)	\
	movl	$(ev_val), %ecx; \
	rdpmc;	\
	salq	$32, %rdx; \
	orq	%rdx, %rax; \
	subq	%rax, (offset)(%rdi)

# else
#  define NEG	negl
#  define STORE_EV_START(ev_val,	offset)	\
	movl	$(ev_val), %ecx; \
	rdpmc;	\
	movl	%eax, (offset)(%rdi)

#  define STORE_EV_END(ev_val,	offset)	\
	movl	$(ev_val), %ecx; \
	rdpmc;	\
	subl	%eax, (offset)(%rdi)
# endif



# define ENTRY(name)	\
	.p2align 4; \
	.globl	name; \
	.type	name, @function; \
	name

# define END(name)	.size	name,	.-	name
# define L(name)	.L##name

ENTRY(read_events_start):
	.cfi_startproc
	lfence
# if PERF_EV_WITH_BUILTINS
	STORE_EV_START(1u << 30, 0)
	STORE_EV_START((1u << 30) + 1, PERF_EV_COUNTER_WIDTH)
	STORE_EV_START((1u << 30) + 2, 2 * PERF_EV_COUNTER_WIDTH)
# endif
# if PERF_EV_NEVENTS > NBUILTINS
	STORE_EV_START(0, PRG_OFFSET)
# endif
# if PERF_EV_NEVENTS > (NBUILTINS + 1)
	STORE_EV_START(1, PRG_OFFSET + PERF_EV_COUNTER_WIDTH)
# endif
# if PERF_EV_NEVENTS > (NBUILTINS + 2)
	STORE_EV_START(2, PRG_OFFSET + 2 * PERF_EV_COUNTER_WIDTH)
# endif
# if PERF_EV_NEVENTS > (NBUILTINS + 3)
	STORE_EV_START(3, PRG_OFFSET + 3 * PERF_EV_COUNTER_WIDTH)
# endif
	lfence
	ret
	.cfi_endproc
END(read_events_start)


ENTRY(read_events_end):
	.cfi_startproc
	lfence
# if PERF_EV_WITH_BUILTINS
	STORE_EV_END(1u << 30, 0)
	STORE_EV_END((1u << 30) + 1, PERF_EV_COUNTER_WIDTH)
	STORE_EV_END((1u << 30) + 2, 2 * PERF_EV_COUNTER_WIDTH)
# endif
# if PERF_EV_NEVENTS > NBUILTINS
	STORE_EV_END(0, PRG_OFFSET)
# endif
# if PERF_EV_NEVENTS > (NBUILTINS + 1)
	STORE_EV_END(1, PRG_OFFSET + PERF_EV_COUNTER_WIDTH)
# endif
# if PERF_EV_NEVENTS > (NBUILTINS + 2)
	STORE_EV_END(2, PRG_OFFSET + 2 * PERF_EV_COUNTER_WIDTH)
# endif
# if PERF_EV_NEVENTS > (NBUILTINS + 3)
	STORE_EV_END(3, PRG_OFFSET + 3 * PERF_EV_COUNTER_WIDTH)
# endif
	lfence
	xorl	%ecx, %ecx
	cmpl	$(PERF_EV_NEVENTS), %ecx
	je	L(return)
L(negate_counters):
	NEG	(%rdi, %rcx, PERF_EV_COUNTER_WIDTH)
	incl	%ecx
	cmpl	$(PERF_EV_NEVENTS), %ecx
	jne	L(negate_counters)
L(return):
	ret
	.cfi_endproc
END(read_events_end)
#endif
