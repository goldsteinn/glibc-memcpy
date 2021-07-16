#ifndef _PERF_EV_READ_H_
#define _PERF_EV_READ_H_

#include "perf-ev-common.h"
#if PERF_EVENTS
void read_events_start(ev_counter_t * counter_values);
void read_events_end(ev_counter_t * counter_values);
#else
static void
read_events_start(ev_counter_t * counter_values) {
    (void)(counter_values);
}
static void
read_events_end(ev_counter_t * counter_values) {
    (void)(counter_values);
}
#endif


#endif
