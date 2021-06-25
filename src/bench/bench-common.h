#ifndef _BENCH_COMMON_H_
#define _BENCH_COMMON_H_

#include <common.h>
#include <macro.h>
#include <memory-util.h>

#include "bench-confs.h"
#include "bench-stats.h"

static const uint32_t inner_trials = 16;
static const uint32_t LSD_START    = 22;


typedef char bench_char_t;

typedef enum bench_options { RAND, FIXED } bench_options_t;


typedef struct bench_params {
    bench_conf_t *     confs;
    uint32_t           trials;
    uint64_t           nconfs;
    const char * const test_name;
    bench_options_t    todo;
} bench_params_t;


typedef struct bench_result {
    uint64_t *      times;
    bench_conf_t *  confs;
    uint32_t        trials;
    bench_stats_t * stats;
    uint32_t        nconfs;
    const char *    test_name;
    const char *    impl_name;
} bench_result_t;


typedef void (*bench_func)(const bench_result_t * restrict result,
                           bench_char_t * restrict         mem_lo,
                           bench_char_t * restrict         mem_hi,
                           bench_options_t                 todo);


typedef struct bench {
    const bench_func func;
    const char *     name;
} bench_t;

void
run_benchmark(const char *           file_path,
              const bench_params_t * params,
              uint64_t               nparams,
              const bench_t *        bench_info);
#endif
