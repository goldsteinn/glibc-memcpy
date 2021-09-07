#include "cpu-event-defs/event-defs.h"
#include "perf-ev-defines.h"
#include "perf-ev-init.h"
// echo 2 > /sys/devices/cpu/rdpmc 
const perf_ev_t perf_events_in_use[] = {
#if PERF_EV_WITH_BUILTINS
    INST_RETIRED_ANY_P,
    UNHALTED_CORE_CYCLES,
    UNHALTED_REFERENCE_CYCLES,
#endif
#if PERF_EVENTS
    MEM_LOAD_RETIRED_L1_HIT,
    MEM_LOAD_RETIRED_L1_MISS,
    //        UOPS_DISPATCHED_PORT_2_3,
    //    UOPS_DISPATCHED_PORT_4_9,
    //    UOPS_DISPATCHED_PORT_7_8,
    LSD_UOPS,
    IDQ_DSB_UOPS,
    //    LD_BLOCKS_NO_SR,
    //RESOURCE_STALLS_SB,
        MISPREDICTED_BRANCH_RETIRED,
    IDQ_UOPS_NOT_DELIVERED_CORE,
    
    UOPS_DISPATCHED_PORT_0,
    UOPS_DISPATCHED_PORT_6,
    UOPS_DISPATCHED_PORT_2_3,
    UOPS_DISPATCHED_PORT_4_9,
    UOPS_DISPATCHED_PORT_7_8,

    RESOURCE_STALLS_SB,
    LD_BLOCKS_STORE_FORWARD,
    LD_BLOCKS_NO_SR,
    LD_BLOCKS_PARTIAL_ADDRESS_ALIAS,
    MACHINE_CLEARS_MEMORY_ORDERING,
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
