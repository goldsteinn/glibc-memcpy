#ifndef MEMMOVE_SYMBOL
# define MEMMOVE_SYMBOL(p,s)	p##_avx512_##s##_frms
#endif
#define USE_WITH_FRMS	1
#include "memmove-avx512-unaligned-erms.S"

