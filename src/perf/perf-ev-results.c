#include "perf-ev-results.h"


void
display_counters_hdr(FILE * fp, const perf_ev_t * perf_events) {
#if PERF_EVENTS
    for (uint64_t i = 0; i < PERF_EV_NEVENTS; ++i) {
        if (i) {
            fprintf(fp, ",");
        }
        fprintf(fp, "%-10s", perf_events[i].desc);
    }
#else
    (void)(fp);
    (void)(perf_events);
#endif
}


void
display_counters(FILE *                   fp,
                 const perf_ev_result_t * results,
                 const perf_ev_t *        perf_events,
                 int32_t                  human_readable) {
#if PERF_EVENTS
    if (human_readable) {
        fprintf(fp, "\nPERF COUNTER VALUES\n");
    }
    for (uint64_t i = 0; i < PERF_EV_NEVENTS; ++i) {
        if (human_readable) {
            fprintf(fp, "%-24s -> %-12lu -> %-.1E\n", perf_events[i].desc,
                    (uint64_t)(results->counters[i]),
                    (double)(results->counters[i]));
        }
        else {
            if (i) {
                fprintf(fp, ",");
            }
            fprintf(fp, "%-10lu", ((uint64_t)(results->counters[i])));
        }
    }
#else
    (void)(fp);
    (void)(results);
    (void)(perf_events);
    (void)(human_readable);
#endif
}
