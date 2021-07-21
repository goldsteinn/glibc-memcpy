#include "cpu-event-defs/event-defs.h"
#include "perf-ev-defines.h"
#include "perf-ev-init.h"


const perf_ev_t perf_events_in_use[] = {
#if PERF_EV_WITH_BUILTINS
    INST_RETIRED_ANY_P,
    UNHALTED_CORE_CYCLES,
    UNHALTED_REFERENCE_CYCLES,
#endif
#if PERF_EVENTS
    LSD_UOPS,
    IDQ_DSB_UOPS,
    MISPREDICTED_BRANCH_RETIRED,
    UOPS_DISPATCHED_PORT_6,



#endif
};
