#ifndef _BENCH_CONSTANTS_H_
#define _BENCH_CONSTANTS_H_

#include <stdint.h>
static const uint32_t inner_trials = 16;
static const uint32_t LSD_START    = 22;

static const uint32_t nrand_confs    = 16 * 4096;
static const uint64_t MAX_BENCH_SIZE = (1UL << 26);


static const uint32_t DEFAULT_SMALL_TRIALS  = (1UL << 12);
static const uint32_t DEFAULT_MEDIUM_TRIALS = (1UL << 8);
static const uint32_t DEFAULT_LARGE_TRIALS  = (1UL << 2);
static const uint32_t DEFAULT_RAND_TRIALS =
    (((1UL << 12) * 65536) / nrand_confs);

#endif
