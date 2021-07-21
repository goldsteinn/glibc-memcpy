#include "bench-params.h"
#include <error-util.h>
#include <memory-util.h>
#include "bench-constants.h"

void
init_rand_params(bench_params_t * params_out,
                 int32_t          trials,
                 uint32_t         min_val,
                 uint32_t         max_val) {
    die_assert(trials != 0, "Trials must be > 0");
    params_out->confs     = make_rand_confs(min_val, max_val);
    params_out->nconfs    = 1;
    params_out->test_name = "rand SPEC2017";
    params_out->trials    = trials < 0 ? DEFAULT_RAND_TRIALS : (uint32_t)trials;
    params_out->todo      = RAND;
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
