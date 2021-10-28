#ifndef _AVX_VECS_H_
#define _AVX_VECS_H_
#define MOV_SIZE 3
# define MOVSB_THRESHOLD 4096
# define LARGE_MEMCPY_THRESHOLD 1048576
# define ASDRB 1048576
#define PAD_TO_10     .byte 0x0f, 0x1f, 0x00;

#define HAS_VECS 1
#define USE_WITH_SSE2
#define VEC_SIZE 16
#define VEC(i)   xmm##i
#define VMOVNT   movntdq
#define VMOVU    movups
#define VMOVA    movaps
#define VZEROUPPER
#define SECTION(p) p##.evex

#define USE_AS_AVX 1
#endif
