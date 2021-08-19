#ifndef _BENCH_CONSTANTS_H_
#define _BENCH_CONSTANTS_H_

#include <stdint.h>
static const uint32_t inner_trials = 8 * 131072;
static const uint32_t LSD_START    = 2200000;

static const uint32_t min_rand_confs = 2048;
static const uint32_t num_rand_confs = 65536;
static const uint64_t MAX_BENCH_SIZE = (1UL << 26);


static const uint32_t DEFAULT_SMALL_TRIALS  = (1UL << 12);
static const uint32_t DEFAULT_MEDIUM_TRIALS = (1UL << 8);
static const uint32_t DEFAULT_LARGE_TRIALS  = (1UL << 2);
static const uint32_t DEFAULT_RAND_TRIALS =
    (((1UL << 12) * 65536) / num_rand_confs);

#endif
