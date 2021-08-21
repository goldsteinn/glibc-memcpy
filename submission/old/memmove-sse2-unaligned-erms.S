#if defined USE_MULTIARCH && IS_IN (libc)
# ifndef MEMMOVE_SYMBOL
#  define MEMMOVE_SYMBOL(p,s)	p##_sse2_##s##_erms
# endif

#define VEC_SIZE	16
#define VEC(i)	xmm##i
#define PREFETCHNT	prefetchnta
#define VMOVNT	movntdq
	/* Use movups and movaps for smaller code sizes.  */
#define VMOVU	movups
#define VMOVA	movaps

    
    
# define USE_WITH_ERMS	1
# include "memmove-sse2-unaligned.S"
#endif

