#include "bench-stats.h"

#include <math.h>

static int
cmp_u64(const void * a, const void * b) {
    uint64_t _a = *(const uint64_t *)a;
    uint64_t _b = *(const uint64_t *)b;
    return _a < _b ? -1 : (_a > _b ? 1 : 0);
}

static double
get_mean(double * times, uint64_t n) {
    double sum = 0.0;
    for (uint64_t i = 0; i < n; ++i) {
        sum += times[i];
    }
    return sum / ((double)n);
}

static double
get_gmean(double * times, uint64_t n) {
    double sum = 0.0;
    for (uint64_t i = 0; i < n; ++i) {
        sum += log((times[i]));
    }
    return exp(sum / ((double)n));
}

static double
get_stdev(double * times, uint64_t n) {
    if (n <= 1) {
        return 0.0;
    }
    double squared_sum = 0.0;
    double mean        = get_mean(times, n);
    for (uint64_t i = 0; i < n; ++i) {
        squared_sum += (times[i] - mean) * (times[i] - mean);
    }
    return sqrt((squared_sum / ((double)n)));
}


static double
get_median(double * times, uint64_t n) {
    return (times[n / 2] + times[(n - 1) / 2]) / 2.0;
}


static double
get_max(double * times, uint64_t n) {
    return times[n - 1];
}

static double
get_min(double * times, uint64_t n) {
    (void)(n);
    return times[0];
}


void
make_stats(bench_stats_t * stats,
           uint64_t *      times,
           uint64_t        n,
           double          times_scale) {
    qsort(times, n, sizeof(uint64_t), cmp_u64);
    double * times_dbl = (double *)calloc(n, sizeof(double));
    for (uint64_t i = 0; i < n; ++i) {
        double time_dbl = times[i];
        time_dbl /= times_scale;
        times_dbl[i] = time_dbl;
    }


    stats->n      = n;
    stats->mean   = get_mean(times_dbl, n);
    stats->median = get_median(times_dbl, n);
    stats->gmean  = get_gmean(times_dbl, n);
    stats->min    = get_min(times_dbl, n);
    stats->max    = get_max(times_dbl, n);
    stats->stdev  = get_stdev(times_dbl, n);
    free(times_dbl);
}
