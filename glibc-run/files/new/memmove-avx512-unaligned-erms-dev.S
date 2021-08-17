#if IS_IN (libc)
# define VEC_SIZE	64
# define VEC0	zmm16
# define VEC1	zmm17
# define VEC2	zmm18
# define VEC3	zmm19
# define VEC4	zmm20
# define VEC5	zmm21
# define VEC6	zmm22
# define VEC7	zmm23
# define VEC8	zmm24
# define VEC9	zmm25
# define VEC10	zmm26
# define VEC11	zmm27
# define VEC12	zmm28
# define VEC13	zmm29
# define VEC14	zmm30
# define VEC15	zmm31
# define VEC(i)	VEC##i
# define VMOVNT	vmovntdq
# define VMOVU	vmovdqu64
# define VMOVA	vmovdqa64
# define VZEROUPPER

# define SECTION(p)	p##.evex512
# define MEMCPY	__memmove_avx512_unaligned_erms_dev
# define VZEROUPPER


# define USE_WITH_MULTIARCH_AND_LIBC
# define USE_WITH_ERMS

# include "memmove-avx512-dev.S"
#endif
