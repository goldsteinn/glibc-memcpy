#if IS_IN (libc)
# define VEC_SIZE	32
# define VEC(i)		ymm##i
# define VMOVNT		vmovntdq
# define VMOVU		vmovdqu
# define VMOVA		vmovdqa

# define SECTION(p)		p##.avx
# define MEMCPY	__memmove_avx2_unaligned_erms_dev

#define USE_WITH_MULTIARCH_AND_LIBC
#define USE_WITH_ERMS
    
# include "memmove-avx2-dev.S"
#endif
