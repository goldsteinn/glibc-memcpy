#if IS_IN (libc)
# ifndef MEMMOVE_SYMBOL
#  define MEMMOVE_SYMBOL(p,s)	p##_sse2_##s
# endif
#else
weak_alias (__mempcpy, mempcpy)
#endif

#include <sysdeps/x86_64/memmove.S>

#if defined SHARED && IS_IN (libc)
# include <shlib-compat.h>
# if SHLIB_COMPAT (libc, GLIBC_2_2_5, GLIBC_2_14)
	/* Use __memmove_sse2_unaligned to support overlapping addresses.  */
compat_symbol (libc, __memmove_sse2_unaligned, memcpy, GLIBC_2_2_5);
# endif
#endif
