#ifndef _BENCH_STATS_H_
#define _BENCH_STATS_H_

#include <common.h>
#include <memory-util.h>

typedef struct bench_stats {
    uint64_t n;
    double   mean;
    double   median;
    double   gmean;
    double   min;
    double   max;
    double   stdev;
} bench_stats_t;


void make_stats(bench_stats_t * stats,
                uint64_t *      times,
                uint64_t        n,
                double          times_scale);

#endif
