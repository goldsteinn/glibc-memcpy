#ifndef _PERF_EV_COMMON_H_
#define _PERF_EV_COMMON_H_

#include <stdint.h>

#include "perf-ev-defines.h"

#if PERF_EV_COUNTER_WIDTH == 8
typedef uint64_t ev_counter_t;
#elif PERF_EV_COUNTER_WIDTH == 4
typedef uint32_t ev_counter_t;
#else
#if PERF_EVENTS
#error "Invalid PERF_EV_COUNTER_WIDTH. Must be 4 or 8!"
#else
typedef uint8_t ev_counter_t;
#endif
#endif


#endif
