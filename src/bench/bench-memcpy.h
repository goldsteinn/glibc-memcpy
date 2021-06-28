// expected defines:
// sort_t
// NAME

#include <common.h>
#include <macro-math.h>
#include <memory-util.h>
#include <timing.h>

#include "bench-constants.h"
#include "bench-macros.h"
#include "bench-params.h"

#undef run_rand_bench
#undef run_fixed_bench
#undef bench_rand
#undef bench_fixed
#define run_rand_bench  CAT(run_rand_bench_, NAME)
#define run_fixed_bench CAT(run_fixed_bench_, NAME)
#define bench_rand      CAT(bench_rand_, NAME)
#define bench_fixed     CAT(bench_fixed_, NAME)
//#define MEASURE_LATENCY

static BENCH_FUNC void
run_rand_bench(const bench_conf_t * restrict confs,
               uint64_t * restrict           times,
               uint32_t                      trials,
               bench_char_t * restrict       mem) {
    for (; trials; --trials) {
        const bench_conf_t * loop_confs = confs;
        // prevent OOE between loops
        LIGHT_SERIALIZE();
        ALIGN_CODE(6);
        uint64_t latency_tmp = 0;
        (void)(latency_tmp);
        uint64_t start       = get_cycles();
        for (uint32_t i = nrand_confs; i; --i) {
            // potentially some overhead readying loop_confs if it false aliases
            // with dst. Possibly worth splitting 4k address alignment but
            // unsure of reasonable approach
            bench_conf_t   conf = *(loop_confs++);
            bench_char_t * dst  = mem + conf.al_dst;
            bench_char_t * src  = mem + conf.al_src;
            uint32_t       sz   = conf.sz;
#ifdef MEASURE_LATENCY
            dst += latency_tmp;
            src += latency_tmp;
            sz += latency_tmp;
            bench_char_t * ret = NAME(dst, src, sz);
            DO_NOT_OPTIMIZE_OUT(ret);
            latency_tmp = (ret - dst);
#else
            DO_NOT_OPTIMIZE_OUT(NAME(dst, src, sz));
#endif
        }
        uint64_t end = get_cycles();
        LIGHT_SERIALIZE();
        bench_store_u64(times, (end - start));
        ++times;
    }
}

static BENCH_FUNC void
run_fixed_bench(bench_conf_t            conf,
                uint64_t * restrict     times,
                uint32_t                trials,
                bench_char_t * restrict mem_lo,
                bench_char_t * restrict mem_hi) {

    bench_char_t * restrict src =
        (conf.direction ? mem_hi : mem_lo) + conf.al_src;
    bench_char_t * restrict dst =
        (conf.direction ? mem_lo : mem_hi) + conf.al_dst;
    uint64_t sz = conf.sz;
    DO_NOT_OPTIMIZE_OUT(NAME(dst, src, sz));

    // idea is we want both want to get a sense of variance but not add too much
    // overhead from timing. inner_trials is constant defined in bench-common.h.
    // Best to keep below 22 to avoid the LSD.
    for (; trials; --trials) {
        LIGHT_SERIALIZE();
        ALIGN_CODE(6);
        uint64_t start = get_cycles();
        for (uint32_t i = inner_trials; i; --i) {
            DO_NOT_OPTIMIZE_OUT(NAME(dst, src, sz));
            //            MEASURE_LATENCY();
        }
        uint64_t end = get_cycles();
        LIGHT_SERIALIZE();
        bench_store_u64(times, (end - start));
        ++times;
    }
}

static void
bench_fixed(const bench_params_t * restrict params,
            uint64_t * restrict             times,
            bench_char_t * restrict         mem_lo,
            bench_char_t * restrict         mem_hi) {
    const bench_conf_t * restrict confs  = params->confs;
    uint32_t                      trials = params->trials;
    uint32_t                      nconfs = params->nconfs;
    // warmup
    run_fixed_bench(confs[0], times, (trials / 4) | 1, mem_lo, mem_hi);
    LIGHT_SERIALIZE();

    for (uint32_t i = 0; i < nconfs; ++i) {
        bench_conf_t _conf = confs[i];
        LIGHT_SERIALIZE();
        run_fixed_bench(_conf, times, trials, mem_lo, mem_hi);
        LIGHT_SERIALIZE();
        times += trials;
    }
}


static void
bench_rand(const bench_params_t * restrict params,
           uint64_t * restrict             times,
           bench_char_t * restrict         mem_lo) {

    const bench_conf_t * restrict confs  = params->confs;
    uint32_t                      trials = params->trials;
    // warmup
    run_rand_bench(confs, times, (trials / 4) | 1, mem_lo);
    LIGHT_SERIALIZE();

    run_rand_bench(confs, times, trials, mem_lo);
    LIGHT_SERIALIZE();
}

static void
CAT(bench_, NAME)(const bench_params_t * restrict params,
                  uint64_t * restrict             times,
                  bench_char_t * restrict         mem_lo,
                  bench_char_t * restrict         mem_hi) {
    if (params->todo == RAND) {
        bench_rand(params, times, mem_lo);
    }
    else {
        bench_fixed(params, times, mem_lo, mem_hi);
    }
}
