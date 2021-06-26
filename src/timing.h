#ifndef _TIMING_H_
#define _TIMING_H_


#include <time.h>
#include <x86intrin.h>

#include "common.h"


static ALWAYS_INLINE uint64_t
to_ns(struct timespec * ts) {
    return 1000UL * 1000UL * 1000UL * ts->tv_sec + ts->tv_nsec;
}


static ALWAYS_INLINE uint64_t
to_us(struct timespec * ts) {
    return 1000UL * 1000UL * ts->tv_sec + ts->tv_nsec / 1000UL;
}


static ALWAYS_INLINE uint64_t
to_ms(struct timespec * ts) {
    return 1000UL * ts->tv_sec + ts->tv_nsec / (1000UL * 1000UL);
}


static ALWAYS_INLINE uint64_t
dif_ns(struct timespec * ts0, struct timespec * ts1) {
    return to_ns(ts0) - to_ns(ts1);
}

static ALWAYS_INLINE uint64_t
dif_us(struct timespec * ts0, struct timespec * ts1) {
    return to_us(ts0) - to_us(ts1);
}

static ALWAYS_INLINE uint64_t
dif_ms(struct timespec * ts0, struct timespec * ts1) {
    return to_ms(ts0) - to_ms(ts1);
}


static ALWAYS_INLINE double
to_ns_dbl(struct timespec * ts) {
    return (double)(1000UL * 1000UL * 1000UL * ts->tv_sec + ts->tv_nsec);
}
static ALWAYS_INLINE double
to_us_dbl(struct timespec * ts) {
    return (double)(1000UL * 1000UL * ts->tv_sec) +
           (double)ts->tv_nsec / (double)(1000UL);
}
static ALWAYS_INLINE double
to_ms_dbl(struct timespec * ts) {
    return (double)(1000UL * ts->tv_sec) +
           (double)ts->tv_nsec / (double)(1000UL * 1000UL);
}

static ALWAYS_INLINE double
dif_ns_dbl(struct timespec * ts0, struct timespec * ts1) {
    return to_ns_dbl(ts0) - to_ns_dbl(ts1);
}

static ALWAYS_INLINE double
dif_us_dbl(struct timespec * ts0, struct timespec * ts1) {
    return to_us_dbl(ts0) - to_us_dbl(ts1);
}

static ALWAYS_INLINE double
dif_ms_dbl(struct timespec * ts0, struct timespec * ts1) {
    return to_ms_dbl(ts0) - to_ms_dbl(ts1);
}


static ALWAYS_INLINE uint64_t
get_cycles() {
    return _rdtsc();
}

static ALWAYS_INLINE void
gettime(struct timespec * ts) {
    clock_gettime(CLOCK_MONOTONIC, ts);
}


static ALWAYS_INLINE uint64_t
get_ns() {
    struct timespec ts;
    gettime(&ts);
    return to_ns(&ts);
}

static ALWAYS_INLINE void
bench_store_u64(uint64_t * ptr, uint64_t v) {
    asm volatile("movnti %[v], (%[ptr]); sfence"
                 : "=m"(*(uint64_t(*)[1])ptr)
                 : [v] "r"(v), [ptr] "r"(ptr)
                 :);
}

#endif
