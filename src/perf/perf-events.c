#include "cpu-event-defs/event-defs.h"
#include "perf-ev-defines.h"
#include "perf-ev-init.h"

const perf_ev_t perf_events_in_use[] = {
#if PERF_EV_WITH_BUILTINS
    INST_RETIRED_ANY_P, UNHALTED_CORE_CYCLES, UNHALTED_REFERENCE_CYCLES,
#endif
#if PERF_EVENTS
    UOPS_ISSUED_ANY
//  UOPS_DISPATCHED_PORT_0,
// UOPS_DISPATCHED_PORT_1,
// UOPS_DISPATCHED_PORT_5,
// UOPS_DISPATCHED_PORT_6,
//     MISPREDICTED_BRANCH_RETIRED
//    IDQ_DSB_UOPS,
//

//
//
//
// BR_MISP_RETIRED_COND_TAKEN,
// BR_MISP_RETIRED_COND_NTAKEN,
//
// BRANCH_INSTRUCTIONS_RETIRED,
//

// UOPS_DISPATCHED_PORT_2_3,
// UOPS_DISPATCHED_PORT_4_9,

// UOPS_DISPATCHED_PORT_7_8,
#endif
};
