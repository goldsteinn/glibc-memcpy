#ifndef _AVX2_VECS_H_
#define _AVX2_VECS_H_

#define HAS_VECS 1

#define VEC_SIZE   64
#define VEC(i)     zmm##i
#define VMOVNT     vmovntdq
#define VMOVU      vmovdqu64
#define VMOVA      vmovdqa64
#define VZEROUPPER vzeroupper
#define SECTION(p) p##.evex

#define USE_AS_AVX512 1
#endif
