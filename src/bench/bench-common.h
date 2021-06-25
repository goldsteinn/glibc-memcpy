#ifndef _BENCH_COMMON_H_
#define _BENCH_COMMON_H_

#include <common.h>
#include <macro.h>
#include <memory-util.h>


#include "bench-stats.h"

static const uint32_t inner_trials = 16;
static const uint32_t LSD_START    = 22;


typedef char bench_char_t;

typedef enum bench_options { RAND, FIXED } bench_options_t;

typedef struct bench_conf {
    uint64_t al_dst : 16;
    uint64_t al_src : 16;
    uint64_t direction : 1;
    uint32_t sz : 31;
} bench_conf_t;

typedef struct bench_params {
    bench_conf_t *  confs;
    uint64_t        nconfs;
    const char *    test_name;
    uint32_t        trials;
    bench_options_t todo;
} bench_params_t;


typedef struct bench_result {
    uint64_t *             times;
    bench_stats_t *        stats;
    const char *           impl_name;
    const bench_params_t * params;
} bench_result_t;


typedef void (*bench_func)(const bench_params_t * restrict params,
                           uint64_t * restrict             times,
                           bench_char_t * restrict         mem_lo,
                           bench_char_t * restrict         mem_hi);


typedef struct bench {
    const bench_func func;
    const char *     name;
} bench_t;

void run_benchmark(const char *           file_path,
                   const bench_params_t * params,
                   uint64_t               nparams,
                   const bench_t *        bench_info);
#endif
