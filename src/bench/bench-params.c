#include "bench-params.h"
#include <error-util.h>
#include <memory-util.h>
#include "bench-constants.h"

void
init_rand_params(bench_params_t * params_out,
                 int32_t          trials,
                 uint32_t         min_val,
                 uint32_t         max_val,
                 uint32_t         size_scale,
                 uint32_t         nrand_confs) {
    die_assert(nrand_confs >= 2048 && nrand_confs <= num_rand_confs &&
                   (!(nrand_confs & (nrand_confs - 1))),
               "Invalid nrand_confs values!")
        die_assert(trials != 0, "Trials must be > 0");
    params_out->confs =
        make_rand_confs(min_val, max_val, size_scale, nrand_confs);
    params_out->nconfs    = 1;
    params_out->test_name = "rand SPEC2017";
    params_out->trials    = trials < 0 ? DEFAULT_RAND_TRIALS : (uint32_t)trials;
    params_out->todo      = RAND;
    params_out->rand_conf_min_val    = min_val;
    params_out->rand_conf_max_val    = max_val;
    params_out->rand_conf_size_scale = size_scale;
    params_out->nrand_confs          = nrand_confs;
}


void
init_small_params(bench_params_t * params_out,
                  uint32_t         dst_al_offset,
                  int32_t          trials) {
    die_assert(trials != 0, "Trials must be > 0");
    params_out->confs = make_small_confs(dst_al_offset, &(params_out->nconfs));
    params_out->test_name = "fixed small";
    params_out->trials = trials < 0 ? DEFAULT_SMALL_TRIALS : (uint32_t)trials;
    params_out->todo   = FIXED;
}


void
init_medium_params(bench_params_t * params_out,
                   uint32_t         dst_al_offset,
                   int32_t          trials) {
    die_assert(trials != 0, "Trials must be > 0");
    params_out->confs = make_medium_confs(dst_al_offset, &(params_out->nconfs));
    params_out->test_name = "fixed medium";
    params_out->trials = trials < 0 ? DEFAULT_MEDIUM_TRIALS : (uint32_t)trials;
    params_out->todo   = FIXED;
}


void
init_large_params(bench_params_t * params_out, int32_t trials) {
    die_assert(trials != 0, "Trials must be > 0");
    params_out->confs     = make_large_confs(&(params_out->nconfs));
    params_out->test_name = "fixed large";
    params_out->trials = trials < 0 ? DEFAULT_LARGE_TRIALS : (uint32_t)trials;
    params_out->todo   = FIXED;
}

void
destroy_params(bench_params_t * params, uint64_t nparams) {
    for (uint64_t i = 0; i < nparams; ++i) {
        safe_free(params[i].confs);
    }
}


uint32_t
params_get_conf_sz(const bench_params_t * params, uint32_t i) {
    return params->todo == RAND ? params->rand_conf_size_scale
                                : params->confs[i].sz;
}

uint32_t
params_get_conf_al_dst(const bench_params_t * params, uint32_t i) {
    return params->todo == RAND ? params->rand_conf_min_val
                                : params->confs[i].al_dst;
}

uint32_t
params_get_conf_al_src(const bench_params_t * params, uint32_t i) {
    return params->todo == RAND ? params->rand_conf_max_val
                                : params->confs[i].al_src;
}
uint32_t
params_get_conf_direction(const bench_params_t * params, uint32_t i) {
    return params->todo == RAND ? params->nrand_confs
                                : params->confs[i].direction;
}
