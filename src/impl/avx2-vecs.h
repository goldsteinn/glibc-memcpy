#ifndef _AVX2_VECS_H_
#define _AVX2_VECS_H_
#define MOV_SIZE 4
# define MOVSB_THRESHOLD 4096
# define LARGE_MEMCPY_THRESHOLD 1048576
# define ASDRB 1048576
#define PAD_TO_10     .byte 0x0f, 0x1f, 0x00;

#define HAS_VECS 1
#define USE_WITH_AVX
#define VEC_SIZE   32
#define XMM0       xmm0
#define XMM1       xmm1
#define YMM0       ymm0
#define YMM1       ymm1
#define VEC0       ymm0
#define VEC1       ymm1
#define VEC2       ymm2
#define VEC3       ymm3
#define VEC4       ymm4
#define VEC5       ymm5
#define VEC6       ymm6
#define VEC7       ymm7
#define VEC8       ymm8
#define VEC9       ymm9
#define VEC10      ymm10
#define VEC11      ymm11
#define VEC12      ymm12
#define VEC13      ymm13
#define VEC14      ymm14
#define VEC15      ymm15
#define VEC(i)     ymm##i
#define VMOVNT     vmovntdq
#define VMOVU      vmovdqu
#define VMOVA      vmovdqa
#define VZEROUPPER vzeroupper
#define SECTION(p) p##.evex

#define USE_AS_AVX2 1
#endif
