#ifndef _AVX_VECS_H_
#define _AVX_VECS_H_

#define HAS_VECS 1

#define VEC_SIZE 16
#define VEC(i)   xmm##i
#define VMOVNT   vmovntdq
#define VMOVU    vmovdqu
#define VMOVA    vmovdqa
#define VZEROUPPER
#define SECTION(p) p##.evex

#define USE_AS_AVX 1
#endif
