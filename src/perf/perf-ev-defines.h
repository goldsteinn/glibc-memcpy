#ifndef _PERF_EV_DEFINES_H_
#define _PERF_EV_DEFINES_H_

#define PERF_EVENTS 1
#if PERF_EVENTS
#define PERF_EV_PRG_EVENTS    4
#define PERF_EV_WITH_BUILTINS 0
#define PERF_EV_COUNTER_WIDTH 8
#else
#define PERF_EV_PRG_EVENTS    0u
#define PERF_EV_WITH_BUILTINS 0u
#define PERF_EV_COUNTER_WIDTH 0u
#endif

#if PERF_EV_WITH_BUILTINS
#define PERF_EV_NEVENTS (PERF_EV_PRG_EVENTS + 3)
#else
#define PERF_EV_NEVENTS (PERF_EV_PRG_EVENTS)
#endif

#endif
