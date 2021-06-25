// expected defines:
// sort_t
// NAME

#include <common.h>
#include <macro-math.h>
#include <memory-util.h>
#include <timing.h>

#include "bench-common.h"
#include "bench-confs.h"
#include "bench-macros.h"

#undef run_rand_bench
#undef run_fixed_bench
#undef bench_rand
#undef bench_fixed
#define run_rand_bench  CAT(run_rand_bench_, NAME)
#define run_fixed_bench CAT(run_fixed_bench_, NAME)
#define bench_rand      CAT(bench_rand_, NAME)
#define bench_fixed     CAT(bench_fixed_, NAME)

#ifndef MEASURE_LATENCY
#define MEASURE_LATENCY()
#endif

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
        uint64_t start = get_cycles();
        for (uint32_t i = nrand_confs; i; --i) {
            // potentially some overhead readying loop_confs if it false aliases
            // with dst. Possibly worth splitting 4k address alignment but
            // unsure of reasonable approach
            bench_conf_t   conf = *(loop_confs++);
            bench_char_t * dst  = mem + conf.al_dst;
            bench_char_t * src  = mem + conf.al_src;
            uint32_t       sz   = conf.sz;
            DO_NOT_OPTIMIZE_OUT(NAME(dst, src, sz));
            MEASURE_LATENCY();
        }
        uint64_t end = get_cycles();
        LIGHT_SERIALIZE();
        *(times++) = (end - start);
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
    // idea is we want both want to get a sense of variance but not add too much
    // overhead from timing. inner_trials is constant defined in bench-common.h.
    // Best to keep below 22 to avoid the LSD.
    for (; trials; --trials) {
        LIGHT_SERIALIZE();
        ALIGN_CODE(6);
        uint64_t start = get_cycles();
        for (uint32_t i = inner_trials; i; --i) {
            DO_NOT_OPTIMIZE_OUT(NAME(dst, src, sz));
            MEASURE_LATENCY();
        }
        uint64_t end = get_cycles();
        LIGHT_SERIALIZE();
        *(times++) = (end - start);
    }
}

static void
bench_fixed(const bench_result_t * restrict result,
            bench_char_t * restrict         mem_lo,
            bench_char_t * restrict         mem_hi) {
    uint64_t * restrict           current_times = result->times;
    const bench_conf_t * restrict confs         = result->confs;
    uint32_t                      trials        = result->trials;
    uint32_t                      nconfs        = result->nconfs;
    // warmup
    run_fixed_bench(confs[0], current_times, trials, mem_lo, mem_hi);
    LIGHT_SERIALIZE();

    for (uint32_t i = 0; i < nconfs; ++i) {
        bench_conf_t _conf = confs[i];
        run_fixed_bench(_conf, current_times, trials, mem_lo, mem_hi);
        current_times += trials;
    }
}


static void
bench_rand(const bench_result_t * restrict result,
           bench_char_t * restrict         mem_lo) {

    uint64_t * restrict           times  = result->times;
    const bench_conf_t * restrict confs  = result->confs;
    uint32_t                      trials = result->trials;
    // warmup
    run_rand_bench(confs, times, 1, mem_lo);
    LIGHT_SERIALIZE();
    
    run_rand_bench(confs, times, trials, mem_lo);
}

static void
CAT(bench_, NAME)(const bench_result_t * restrict result,
                  bench_char_t * restrict         mem_lo,
                  bench_char_t * restrict         mem_hi,
                  bench_options_t                 todo) {
    if (todo == RAND) {
        bench_rand(result, mem_lo);
    }
    else {
        bench_fixed(result, mem_lo, mem_hi);
    }
}
