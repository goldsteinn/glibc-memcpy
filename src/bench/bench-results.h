#ifndef _BENCH_RESULTS_H_
#define _BENCH_RESULTS_H_

#include <common.h>
#include <macro.h>
#include <memory-util.h>
#include <perf/perf-ev-results.h>

#include "bench-params.h"
#include "bench-stats.h"


typedef struct bench_result {
    uint64_t *             times;
    bench_stats_t *        stats;
    const char *           impl_name;
    const bench_params_t * params;
    perf_ev_result_t       ev_results;
} bench_result_t;


bench_result_t * init_results(const bench_params_t * params,
                              uint64_t               nparams,
                              const char *           impl_name);

void destroy_results(bench_result_t * results, uint64_t nresults);


void display_all_results(const bench_result_t * results, uint64_t nresults);
#endif
