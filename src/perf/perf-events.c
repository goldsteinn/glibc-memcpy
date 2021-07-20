#include "cpu-event-defs/event-defs.h"
#include "perf-ev-defines.h"
#include "perf-ev-init.h"



const perf_ev_t perf_events_in_use[7] = {
#if PERF_EV_WITH_BUILTINS
    INST_RETIRED_ANY_P,
    UNHALTED_CORE_CYCLES,
    UNHALTED_REFERENCE_CYCLES,
#endif
#if PERF_EVENTS
    UOPS_DISPATCHED_PORT_0,
    UOPS_DISPATCHED_PORT_1,
    UOPS_DISPATCHED_PORT_5,
    UOPS_DISPATCHED_PORT_6
#endif
};
