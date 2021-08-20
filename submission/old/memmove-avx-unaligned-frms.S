#ifndef MEMMOVE_SYMBOL
# define MEMMOVE_SYMBOL(p,s)	p##_avx_##s##_frms
#endif
#define USE_WITH_FRMS	1
#include "memmove-avx-unaligned-erms.S"

