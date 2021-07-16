#ifndef _MEMCPY_DEFS_H_
#define _MEMCPY_DEFS_H_
#include <bench/bench-params.h>
#include <perf/perf-ev-common.h>
#include <stddef.h>
#include <stdint.h>

typedef void (*bench_func)(const bench_params_t * restrict params,
                           uint64_t * restrict             times,
                           ev_counter_t * restrict         ev_results,
                           bench_char_t * restrict         mem_lo,
                           bench_char_t * restrict         mem_hi);

typedef void * (*memcpy_func)(void *, const void *, size_t);

typedef struct memcpy_info {
    const memcpy_func run_memcpy;
    const bench_func  run_bench;
    const char *      name;
} memcpy_info_t;

#endif
