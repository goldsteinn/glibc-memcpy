#ifndef _EVEX_VECS_H_
#define _EVEX_VECS_H_
#define MOV_SIZE               6
#define MOVSB_THRESHOLD        2112
#define LARGE_MEMCPY_THRESHOLD 1048576
#define ASDRB                  1
#define PAD_TO_10              .byte 0x66, 0x0f, 0x1f, 0x44, 0x00, 0x00;

#define HAS_VECS 1
#define USE_WITH_AVX512
#define VEC_SIZE 64
#define XMM0     xmm0
#define XMM1     xmm1
#define YMM0     ymm16
#define YMM1     ymm17
#define VEC0     zmm16
#define VEC1     zmm17
#define VEC2     zmm18
#define VEC3     zmm19
#define VEC4     zmm20
#define VEC5     zmm21
#define VEC6     zmm22
#define VEC7     zmm23
#define VEC8     zmm24
#define VEC9     zmm25
#define VEC10    zmm26
#define VEC11    zmm27
#define VEC12    zmm28
#define VEC13    zmm29
#define VEC14    zmm30
#define VEC15    zmm31
#define VEC(i)   VEC##i
#define VMOVNT   vmovntdq
#define VMOVU    vmovdqu64
#define VMOVA    vmovdqa64
#define VZEROUPPER
#define SECTION(p) p##.evex

#define USE_AS_EVEX 1
#endif
