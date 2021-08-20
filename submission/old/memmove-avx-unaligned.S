#define VEC_SIZE	32
#define VEC(i)	ymm##i
#define VMOVNT	vmovntdq
#define VMOVU	vmovdqu
#define VMOVA	vmovdqa

#define SECTION(p)	p##.avx
#ifndef MEMMOVE_SYMBOL
# define MEMMOVE_SYMBOL(p,s)	p##_avx_##s##
#endif

#include "memmove-vec-unaligned.S"

