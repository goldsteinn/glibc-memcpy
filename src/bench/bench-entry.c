
#include <error-util.h>
#include <memory-util.h>
#include <sched.h>
#include <unistd.h>

#include <perf/perf-ev-init.h>

#include "bench-constants.h"
#include "bench-entry.h"
#include "bench-results.h"

extern const perf_ev_t perf_events_in_use[];

static void
prep_bench_memory(bench_char_t * mem) {
    // set -1 so that 0-over-0 writeback optimization is disabled
    uint32_t fill = rand();
    memset(mem, fill | 1, 2 * MAX_BENCH_SIZE);
    // page in early pieces for src/dst
    memset(mem + MAX_BENCH_SIZE, fill | 4, 104876);
    memset(mem, fill | 2, 1048576);

    // finally get first bit hot in L1
    memset(mem + MAX_BENCH_SIZE, fill | 16, 8192);
    memset(mem, fill | 8, 8192);
}

static void
bench_init(const bench_params_t * params,
           uint64_t               nparams,
           const char *           impl_name,
           bench_result_t **      results_out,
           bench_char_t **        mem_lo_out,
           bench_char_t **        mem_hi_out) {
    bench_result_t * results = init_results(params, nparams, impl_name);
    *results_out             = results;

    bench_char_t * init_mem =
        (bench_char_t *)safe_mmap_alloc(2 * MAX_BENCH_SIZE);

    *mem_lo_out = init_mem;
    *mem_hi_out = init_mem + MAX_BENCH_SIZE;
}


static void
bench_finish(uint64_t         nresults,
             bench_result_t * results,
             bench_char_t *   init_mem) {
    PRINTFFL;
    display_all_results(results, nresults);
    PRINTFFL;
    destroy_results(results, nresults);
    PRINTFFL;
    safe_munmap(init_mem, 2 * MAX_BENCH_SIZE);
}


void
_run_benchmarks(const bench_params_t *        params,
                uint64_t                      nparams,
                const memcpy_info_t *         memcpy_info,
                int32_t                       core,
                const perf_ev_initializer_t * pev_initializer) {
    die_assert(nparams > 0);
    die_assert(params != NULL);
    die_assert(inner_trials < LSD_START || inner_trials > 256,
               "Benchmarkings will have unreasonable overhead from branch "
               "misses in the inner loop");
    PRINTFFL;
    cpu_set_t cur_set, bench_set;
    if (core >= 0) {
        die_assert(core < sysconf(_SC_NPROCESSORS_ONLN),
                   "Invalid core selection");
        CPU_ZERO(&cur_set);
        CPU_ZERO(&bench_set);
        err_assert(sched_getaffinity(0, sizeof(cpu_set_t), &cur_set) >= 0);
        CPU_SET(core, &bench_set);
        err_assert(sched_setaffinity(0, sizeof(cpu_set_t), &bench_set) >= 0);
        while (sched_getcpu() != core) {
            sched_yield();
        }
    }

    bench_result_t * results;
    bench_char_t *   mem_lo;
    bench_char_t *   mem_hi;
    const bench_func func = memcpy_info->run_bench;
    bench_init(params, nparams, memcpy_info->name, &results, &mem_lo, &mem_hi);

    int pev_group_fd = init_events(pev_initializer);
    enable_perf(pev_group_fd);

    PRINTFFL;
    for (uint64_t i = 0; i < nparams; ++i) {
        PRINTFFL;
        prep_bench_memory(mem_lo);
        func(params + i, results[i].times, results[i].ev_results.counters,
             mem_lo, mem_hi);
        PRINTFFL;
    }
    if (core >= 0) {
        warn_assert(sched_setaffinity(0, sizeof(cpu_set_t), &cur_set) >= 0,
                    "Unable to restore old cpu set\n")
    }

    PRINTFFL;
    disable_perf(pev_group_fd);
    bench_finish(nparams, results, mem_lo);
    PRINTFFL;
    (void)(pev_initializer);
}

void
run_benchmarks(const bench_params_t * params,
               uint64_t               nparams,
               const memcpy_info_t *  memcpy_info,
               int32_t                core) {
#if PERF_EVENTS
    struct perf_event_attr ev_attrs[PERF_EV_NEVENTS];
    perf_ev_initializer_t  pev_initializer = { perf_events_in_use, ev_attrs,
                                              PERF_EV_NEVENTS };
    _run_benchmarks(params, nparams, memcpy_info, core, &pev_initializer);
#else
    _run_benchmarks(params, nparams, memcpy_info, core, NULL);
#endif
}
