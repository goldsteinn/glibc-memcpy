#ifndef _BENCH_ENTRY_H_
#define _BENCH_ENTRY_H_

#include "bench-params.h"

#include <memcpy-defs.h>

void run_benchmarks(const bench_params_t * params,
                    uint64_t               nparams,
                    const memcpy_info_t *  memcpy_info,
                    int32_t                core);

#endif
