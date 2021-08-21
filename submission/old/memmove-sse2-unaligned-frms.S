#ifndef MEMMOVE_SYMBOL
# define MEMMOVE_SYMBOL(p,s)	p##_sse2_##s##_frms
#endif
#define USE_WITH_FRMS	1
#include "memmove-sse2-unaligned-erms.S"

