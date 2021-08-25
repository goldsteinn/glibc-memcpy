#ifndef _AVX_VECS_H_
#define _AVX_VECS_H_

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
