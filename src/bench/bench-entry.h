#ifndef _BENCH_ENTRY_H_
#define _BENCH_ENTRY_H_

#include "bench-params.h"

void run_benchmarks(const bench_params_t * params,
                    uint64_t               nparams,
                    const bench_t *        bench_info,
                    int32_t                core);

#endif
