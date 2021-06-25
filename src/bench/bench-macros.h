
#ifndef _BENCH_MACROS_H_
#define _BENCH_MACROS_H_

#define SERIALIZE()            asm volatile("lfence" : : : "memory")
#define LIGHT_SERIALIZE()      asm volatile("lfence" : : :)
#define BARRIER()              asm volatile("" : : : "memory")
#define DO_NOT_OPTIMIZE_OUT(x) asm volatile("" : : "r,m"(x) : "memory")
#define ALIGN_CODE(x)          asm volatile(".p2align " V_TO_STR(x) "\n" : : :);

#endif
