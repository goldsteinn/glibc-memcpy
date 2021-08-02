#include <config-shit.h>
#include "cpu-event-defs/event-defs.h"
#include "perf-ev-defines.h"
#include "perf-ev-init.h"

const perf_ev_t perf_events_in_use[] = {
#if PERF_EV_WITH_BUILTINS
    INST_RETIRED_ANY_P, UNHALTED_CORE_CYCLES, UNHALTED_REFERENCE_CYCLES,
#endif
#if PERF_EVENTS
    EVENT_TO_CHECK
// UOPS_RETIRED_SLOTS
// UOPS_ISSUED_ANY,
// UOPS_EXECUTED_CORE,
// IDQ_UOPS_NOT_DELIVERED_CORE,
// IDQ_MS_UOPS,
// IDQ_MITE_UOPS,
// DSB2MITE_SWITCHES_PENALTY_CYCLES,
// IDQ_DSB_UOPS,
// LSD_UOPS,
// BR_MISP_RETIRED_COND_TAKEN,
// BR_MISP_RETIRED_COND_NTAKEN,
// BACLEARS_ANY,
// BRANCH_INSTRUCTIONS_RETIRED,
// MISPREDICTED_BRANCH_RETIRED,
// UOPS_DISPATCHED_PORT_0,
// UOPS_DISPATCHED_PORT_1,
// UOPS_DISPATCHED_PORT_2_3,
// UOPS_DISPATCHED_PORT_4,
// UOPS_DISPATCHED_PORT_5,
// UOPS_DISPATCHED_PORT_6,
// UOPS_DISPATCHED_PORT_7_8,
#endif
};
