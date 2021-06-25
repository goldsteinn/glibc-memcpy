#ifndef _BENCH_PARAMS_H_
#define _BENCH_PARAMS_H_

#include <common.h>
#include "bench-confs.h"

typedef enum bench_options { RAND, FIXED } bench_options_t;

typedef char bench_char_t;


typedef struct bench_params {
    bench_conf_t *  confs;
    uint32_t        nconfs;
    const char *    test_name;
    uint32_t        trials;
    bench_options_t todo;
} bench_params_t;

typedef void (*bench_func)(const bench_params_t * restrict params,
                           uint64_t * restrict             times,
                           bench_char_t * restrict         mem_lo,
                           bench_char_t * restrict         mem_hi);

typedef struct bench {
    const bench_func func;
    const char *     name;
} bench_t;


void init_rand_params(bench_params_t * params_out, uint32_t trials);
void init_small_params(bench_params_t * params_out,
                       uint32_t         dst_al_offset,
                       uint32_t         trials);
void init_medium_params(bench_params_t * params_out,
                        uint32_t         dst_al_offset,
                        uint32_t         trials);
void init_large_params(bench_params_t * params_out,
                       uint32_t         dst_al_offset,
                       uint32_t         trials);

void destroy_params(bench_params_t * params, uint64_t nparams);
#endif
