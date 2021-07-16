#include "perf-ev-defines.h"
#include "perf-ev-init.h"


#define INSTRUCTIONS                                                           \
    { 0x3c, PERF_TYPE_RAW, "Instructions" }
#define CYCLES                                                                 \
    { 0xc0, PERF_TYPE_RAW, "Cycles" }
#define REF_CYCLES                                                             \
    { 0x300, PERF_TYPE_RAW, "Ref Cycles" }
#define P0_UOPS                                                                \
    { 0x01a1, PERF_TYPE_RAW, "P0 Uops" }
#define P1_UOPS                                                                \
    { 0x02a1, PERF_TYPE_RAW, "P1 Uops" }
#define P5_UOPS                                                                \
    { 0x20a1, PERF_TYPE_RAW, "P5 Uops" }
#define P6_UOPS                                                                \
    { 0x40a1, PERF_TYPE_RAW, "P6 Uops" }

const perf_ev_t perf_events_in_use[7] = {
#if PERF_EV_WITH_BUILTINS
    INSTRUCTIONS, CYCLES,  REF_CYCLES,
#endif
#if PERF_EVENTS
    P0_UOPS,      P1_UOPS, P5_UOPS,    P6_UOPS
#endif
};
