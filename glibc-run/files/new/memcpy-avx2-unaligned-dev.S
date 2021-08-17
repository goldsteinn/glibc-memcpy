#if IS_IN (libc)
# define VEC_SIZE	32
# define VEC(i)	ymm##i
# define VMOVNT	vmovntdq
# define VMOVU	vmovdqu
# define VMOVA	vmovdqa

# define SECTION(p)	p##.avx
# define MEMCPY	__memcpy_avx2_unaligned_dev

# define USE_WITH_MULTIARCH_AND_LIBC

# include "memmove-avx2-dev.S"
#endif
