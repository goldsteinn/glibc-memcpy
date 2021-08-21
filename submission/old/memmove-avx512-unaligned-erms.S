#if defined USE_MULTIARCH && IS_IN (libc)

# ifndef MEMMOVE_SYMBOL
#  define MEMMOVE_SYMBOL(p,s)	p##_avx512_##s##_erms
# endif

# define USE_WITH_ERMS	1

# include "memmove-avx512-unaligned.S"
#endif
