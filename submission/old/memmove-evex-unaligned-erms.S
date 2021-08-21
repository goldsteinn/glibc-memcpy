#if defined USE_MULTIARCH && IS_IN (libc)

# ifndef MEMMOVE_SYMBOL
#  define MEMMOVE_SYMBOL(p,s)	p##_evex_##s##_erms
# endif

# define USE_WITH_ERMS	1

# include "memmove-evex-unaligned.S"
#endif
