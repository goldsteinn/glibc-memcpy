#ifndef _BENCH_PARAMS_H_
#define _BENCH_PARAMS_H_

#include <common.h>
#include "bench-confs.h"
#include "bench-constants.h"

typedef enum bench_options { RAND, FIXED } bench_options_t;

typedef uint8_t bench_char_t;

typedef struct bench_params {
    bench_conf_t *  confs;
    uint32_t        nconfs;
    const char *    test_name;
    uint32_t        trials;
    bench_options_t todo;
    uint32_t        rand_conf_min_val;
    uint32_t        rand_conf_max_val;
    uint32_t        rand_conf_size_scale;
    uint32_t        nrand_confs;
} bench_params_t;


void init_rand_params(bench_params_t * params_out,
                      int32_t          trials,
                      uint32_t         min_val,
                      uint32_t         max_val,
                      uint32_t         size_scale,
                      uint32_t         nrand_confs);
void init_small_params(bench_params_t * params_out,
                       uint32_t         dst_al_offset,
                       int32_t          trials);
void init_medium_params(bench_params_t * params_out,
                        uint32_t         dst_al_offset,
                        int32_t          trials);
void init_large_params(bench_params_t * params_out, int32_t trials);

void destroy_params(bench_params_t * params, uint64_t nparams);

uint32_t params_get_conf_sz(const bench_params_t * params, uint32_t i);

uint32_t params_get_conf_al_dst(const bench_params_t * params, uint32_t i);

uint32_t params_get_conf_al_src(const bench_params_t * params, uint32_t i);
uint32_t params_get_conf_direction(const bench_params_t * params, uint32_t i);


#endif
