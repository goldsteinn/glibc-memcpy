#ifndef _BENCH_GENERIC_H_
#define _BENCH_GENERIC_H_

#include <common.h>
#include "bench-params-generic.h"

typedef void (*bench_func)(uint64_t,
                           const uint64_t *,
                           uint64_t,
                           const bench_init_t *,
                           uint64_t);
typedef struct bench {
    const bench_func   func;
    const char * const name;
    const char * const type;

} bench_t;


#endif
