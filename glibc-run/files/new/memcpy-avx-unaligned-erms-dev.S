#if IS_IN (libc)
# define VEC_SIZE	16
# define VEC(i)	xmm##i
# define VMOVNT	vmovntdq
# define VMOVU	vmovdqu
# define VMOVA	vmovdqa

# define SECTION(p)	p##.avx
# define MEMCPY	__memcpy_avx_unaligned_dev_erms
# define VZEROUPPER

# define USE_WITH_MULTIARCH_AND_LIBC
# define USE_WITH_ERMS

# include "memmove-avx-dev.S"
#endif
