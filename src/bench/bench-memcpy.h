// expected defines:
// sort_t
// NAME

#include <common.h>
#include <inl-nops.h>
#include <macro-math.h>
#include <memory-util.h>
#include <perf/perf-ev-read.h>
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
#define MEASURE_LATENCY 0

static BENCH_FUNC void
run_rand_bench(const bench_conf_t * restrict confs,
               uint64_t * restrict           times,
               ev_counter_t * restrict       ev_results,
               uint32_t                      trials,
               uint32_t                      nrand_confs,
               bench_char_t * restrict       mem) {
    IMPOSSIBLE(trials == 0);
    uint32_t dst_end = 0;
    read_events_start(ev_results);
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
            bench_conf_t conf = *(loop_confs);
            loop_confs += (dst_end + 1);
            bench_char_t * dst = mem + conf.al_dst;
            bench_char_t * src = mem + conf.al_src;
            uint32_t       sz  = conf.sz;
            DO_NOT_OPTIMIZE_OUT(NAME(dst, src, sz));
#if MEASURE_LATENCY
            uint8_t _dst_end = *(dst + sz - 1);
            dst_end          = _dst_end;
            asm volatile("andl  $0, %[dst_end]\n"
                         : [dst_end] "+r"(dst_end)
                         :
                         :);
#endif
        }
        uint64_t end = get_cycles();
        LIGHT_SERIALIZE();
        bench_store_u64(times, (end - start));
        ++times;
    }

    read_events_end(ev_results);
}

static BENCH_FUNC void
run_fixed_bench(bench_conf_t            conf,
                uint64_t * restrict     times,
                ev_counter_t * restrict ev_results,
                uint32_t                trials,
                bench_char_t * restrict mem_lo,
                bench_char_t * restrict mem_hi) {
    bench_char_t * restrict dst =
        (conf.direction ? mem_hi : mem_lo) + conf.al_dst;
    bench_char_t * restrict src =
        (conf.direction ? mem_hi : mem_lo) + conf.al_src;
    uint64_t sz = conf.sz;
    DO_NOT_OPTIMIZE_OUT(NAME(dst, src, sz));
    read_events_start(ev_results);
    // idea is we want both want to get a sense of variance but not add too much
    // overhead from timing. inner_trials is constant defined in bench-common.h.
    // Best to keep below 22 to avoid the LSD.
    IMPOSSIBLE(trials == 0);
    for (; trials; --trials) {

        ALIGN_CODE(6);
        uint64_t start = get_cycles();
        LIGHT_SERIALIZE();
        for (uint32_t i = inner_trials; i; --i) {
            DO_NOT_OPTIMIZE_OUT(NAME(dst, src, sz));
        }
        LIGHT_SERIALIZE();
        uint64_t end = get_cycles();

        bench_store_u64(times, (end - start));
        ++times;
    }
    read_events_end(ev_results);
}

static void
bench_fixed(const bench_params_t * restrict params,
            uint64_t * restrict             times,
            ev_counter_t * restrict         ev_results,
            bench_char_t * restrict         mem_lo,
            bench_char_t * restrict         mem_hi) {
    const bench_conf_t * restrict confs  = params->confs;
    uint32_t                      trials = params->trials;
    uint32_t                      nconfs = params->nconfs;
    // warmup
    run_fixed_bench(confs[0], times, ev_results, (trials / 4) | 1, mem_lo,
                    mem_hi);
    LIGHT_SERIALIZE();

    for (uint32_t i = 0; i < nconfs; ++i) {
        bench_conf_t _conf = confs[i];
        LIGHT_SERIALIZE();
        run_fixed_bench(_conf, times, ev_results, trials, mem_lo, mem_hi);
        LIGHT_SERIALIZE();
        times += trials;
    }
}


static void
bench_rand(const bench_params_t * restrict params,
           uint64_t * restrict             times,
           ev_counter_t * restrict         ev_results,
           bench_char_t * restrict         mem_lo) {

    const bench_conf_t * restrict confs       = params->confs;
    uint32_t                      trials      = params->trials;
    uint32_t                      nrand_confs = params->nrand_confs;
    // warmup
    run_rand_bench(confs, times, ev_results, (trials / 4) | 1, nrand_confs,
                   mem_lo);
    LIGHT_SERIALIZE();

    run_rand_bench(confs, times, ev_results, trials, nrand_confs, mem_lo);
    LIGHT_SERIALIZE();
}

static void
CAT(bench_, NAME)(const bench_params_t * restrict params,
                  uint64_t * restrict             times,
                  ev_counter_t * restrict         ev_results,
                  bench_char_t * restrict         mem_lo,
                  bench_char_t * restrict         mem_hi) {
    if (params->todo == RAND) {
        bench_rand(params, times, ev_results, mem_lo);
    }
    else {
        bench_fixed(params, times, ev_results, mem_lo, mem_hi);
    }
}
