#ifndef _PERF_EV_RESULTS_H_
#define _PERF_EV_RESULTS_H_

#include "perf-ev-common.h"
#include "perf-ev-init.h"

#include <stdio.h>
typedef struct perf_ev_result {
    ev_counter_t counters[8];
} perf_ev_result_t __attribute__((aligned(8 * sizeof(ev_counter_t))));


void display_counters_hdr(FILE * fp, const perf_ev_t * perf_events);

void display_counters(FILE *                   fp,
                      const perf_ev_result_t * results,
                      const perf_ev_t *        perf_events,
                      int32_t                  human_readable);

#endif
