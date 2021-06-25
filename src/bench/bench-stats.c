#include "bench-stats.h"

#include <math.h>

static int
cmp_u64(const void * a, const void * b) {
    uint64_t _a = *(const uint64_t *)a;
    uint64_t _b = *(const uint64_t *)b;
    return _a < _b ? -1 : (_a > _b ? 1 : 0);
}

static double
get_mean(uint64_t * times, uint64_t n) {
    uint64_t sum = 0;
    for (uint64_t i = 0; i < n; ++i) {
        sum += times[i];
    }

    return ((double)sum) / ((double)n);
}

static double
get_gmean(uint64_t * times, uint64_t n) {
    double sum = 0.0;
    for (uint64_t i = 0; i < n; ++i) {
        sum += log((double)(times[i]));
    }
    sum /= ((double)n);
    return exp(sum);
}

static double
get_stdev(uint64_t * times, uint64_t n) {
    if (n <= 1) {
        return 0.0;
    }
    uint64_t squared_sum = 0;
    uint64_t mean        = get_mean(times, n);
    for (uint64_t i = 0; i < n; ++i) {
        squared_sum += (times[i] - mean) * (times[i] - mean);
    }
    double stdev = ((double)squared_sum) / ((double)n);
    return sqrt(stdev);
}


static double
get_median(uint64_t * times, uint64_t n) {
    return (double)((times[n / 2] + times[(n - 1) / 2]) / 2);
}


static double
get_max(uint64_t * times, uint64_t n) {
    return (double)(times[n - 1]);
}

static double
get_min(uint64_t * times, uint64_t n) {
    (void)(n);
    return (double)(times[0]);
}


void
make_stats(bench_stats_t * stats, uint64_t * times, uint64_t n) {
    qsort(times, n, sizeof(uint64_t), cmp_u64);
    stats->n      = n;
    stats->mean   = get_mean(times, n);
    stats->median = get_median(times, n);
    stats->gmean  = get_gmean(times, n);
    stats->min    = get_min(times, n);
    stats->max    = get_max(times, n);
    stats->stdev  = get_stdev(times, n);
}
