#ifndef _ICL_EVENTS_H_
#define _ICL_EVENTS_H_

#define ASSISTS_ANY                                                            \
    { 0x07c1, PERF_TYPE_RAW, "ASSISTS_ANY" }
#define ASSISTS_FP                                                             \
    { 0x02c1, PERF_TYPE_RAW, "ASSISTS_FP" }
#define BACLEARS_ANY                                                           \
    { 0x01e6, PERF_TYPE_RAW, "BACLEARS_ANY" }
#define BRANCH_INSTRUCTIONS_RETIRED                                            \
    { 0x00c4, PERF_TYPE_RAW, "BRANCH_INSTRUCTIONS_RETIRED" }
#define BR_INST_RETIRED_COND                                                   \
    { 0x11c4, PERF_TYPE_RAW, "BR_INST_RETIRED_COND" }
#define BR_INST_RETIRED_COND_NTAKEN                                            \
    { 0x10c4, PERF_TYPE_RAW, "BR_INST_RETIRED_COND_NTAKEN" }
#define BR_INST_RETIRED_COND_TAKEN                                             \
    { 0x01c4, PERF_TYPE_RAW, "BR_INST_RETIRED_COND_TAKEN" }
#define BR_INST_RETIRED_FAR_BRANCH                                             \
    { 0x40c4, PERF_TYPE_RAW, "BR_INST_RETIRED_FAR_BRANCH" }
#define BR_INST_RETIRED_INDIRECT                                               \
    { 0x80c4, PERF_TYPE_RAW, "BR_INST_RETIRED_INDIRECT" }
#define BR_INST_RETIRED_NEAR_CALL                                              \
    { 0x02c4, PERF_TYPE_RAW, "BR_INST_RETIRED_NEAR_CALL" }
#define BR_INST_RETIRED_NEAR_RETURN                                            \
    { 0x08c4, PERF_TYPE_RAW, "BR_INST_RETIRED_NEAR_RETURN" }
#define BR_INST_RETIRED_NEAR_TAKEN                                             \
    { 0x20c4, PERF_TYPE_RAW, "BR_INST_RETIRED_NEAR_TAKEN" }
#define BR_MISP_RETIRED_COND                                                   \
    { 0x11c5, PERF_TYPE_RAW, "BR_MISP_RETIRED_COND" }
#define BR_MISP_RETIRED_COND_NTAKEN                                            \
    { 0x10c5, PERF_TYPE_RAW, "BR_MISP_RETIRED_COND_NTAKEN" }
#define BR_MISP_RETIRED_COND_TAKEN                                             \
    { 0x01c5, PERF_TYPE_RAW, "BR_MISP_RETIRED_COND_TAKEN" }
#define BR_MISP_RETIRED_INDIRECT                                               \
    { 0x80c5, PERF_TYPE_RAW, "BR_MISP_RETIRED_INDIRECT" }
#define BR_MISP_RETIRED_INDIRECT_CALL                                          \
    { 0x02c5, PERF_TYPE_RAW, "BR_MISP_RETIRED_INDIRECT_CALL" }
#define BR_MISP_RETIRED_NEAR_TAKEN                                             \
    { 0x20c5, PERF_TYPE_RAW, "BR_MISP_RETIRED_NEAR_TAKEN" }
#define CORE_POWER_LVL0_TURBO_LICENSE                                          \
    { 0x0728, PERF_TYPE_RAW, "CORE_POWER_LVL0_TURBO_LICENSE" }
#define CORE_POWER_LVL1_TURBO_LICENSE                                          \
    { 0x1828, PERF_TYPE_RAW, "CORE_POWER_LVL1_TURBO_LICENSE" }
#define CORE_POWER_LVL2_TURBO_LICENSE                                          \
    { 0x2028, PERF_TYPE_RAW, "CORE_POWER_LVL2_TURBO_LICENSE" }
#define CPU_CLK_UNHALTED_DISTRIBUTED                                           \
    { 0x02ec, PERF_TYPE_RAW, "CPU_CLK_UNHALTED_DISTRIBUTED" }
#define CPU_CLK_UNHALTED_ONE_THREAD_ACTIVE                                     \
    { 0x023c, PERF_TYPE_RAW, "CPU_CLK_UNHALTED_ONE_THREAD_ACTIVE" }
#define CPU_CLK_UNHALTED_REF_DISTRIBUTED                                       \
    { 0x08ec, PERF_TYPE_RAW, "CPU_CLK_UNHALTED_REF_DISTRIBUTED" }
#define CPU_CLK_UNHALTED_REF_TSC                                               \
    { 0x033c, PERF_TYPE_RAW, "CPU_CLK_UNHALTED_REF_TSC" }
#define CPU_CLK_UNHALTED_REF_XCLK                                              \
    { 0x013c, PERF_TYPE_RAW, "CPU_CLK_UNHALTED_REF_XCLK" }
#define DSB2MITE_SWITCHES_PENALTY_CYCLES                                       \
    { 0x02ab, PERF_TYPE_RAW, "DSB2MITE_SWITCHES_PENALTY_CYCLES" }
#define DTLB_LOAD_MISSES_STLB_HIT                                              \
    { 0x2008, PERF_TYPE_RAW, "DTLB_LOAD_MISSES_STLB_HIT" }
#define DTLB_LOAD_MISSES_WALK_COMPLETED                                        \
    { 0x0e08, PERF_TYPE_RAW, "DTLB_LOAD_MISSES_WALK_COMPLETED" }
#define DTLB_LOAD_MISSES_WALK_COMPLETED_2M_4M                                  \
    { 0x0408, PERF_TYPE_RAW, "DTLB_LOAD_MISSES_WALK_COMPLETED_2M_4M" }
#define DTLB_LOAD_MISSES_WALK_COMPLETED_4K                                     \
    { 0x0208, PERF_TYPE_RAW, "DTLB_LOAD_MISSES_WALK_COMPLETED_4K" }
#define DTLB_LOAD_MISSES_WALK_PENDING                                          \
    { 0x1008, PERF_TYPE_RAW, "DTLB_LOAD_MISSES_WALK_PENDING" }
#define DTLB_STORE_MISSES_STLB_HIT                                             \
    { 0x2049, PERF_TYPE_RAW, "DTLB_STORE_MISSES_STLB_HIT" }
#define DTLB_STORE_MISSES_WALK_COMPLETED                                       \
    { 0x0e49, PERF_TYPE_RAW, "DTLB_STORE_MISSES_WALK_COMPLETED" }
#define DTLB_STORE_MISSES_WALK_COMPLETED_2M_4M                                 \
    { 0x0449, PERF_TYPE_RAW, "DTLB_STORE_MISSES_WALK_COMPLETED_2M_4M" }
#define DTLB_STORE_MISSES_WALK_COMPLETED_4K                                    \
    { 0x0249, PERF_TYPE_RAW, "DTLB_STORE_MISSES_WALK_COMPLETED_4K" }
#define DTLB_STORE_MISSES_WALK_PENDING                                         \
    { 0x1049, PERF_TYPE_RAW, "DTLB_STORE_MISSES_WALK_PENDING" }
#define EXE_ACTIVITY_1_PORTS_UTIL                                              \
    { 0x02a6, PERF_TYPE_RAW, "EXE_ACTIVITY_1_PORTS_UTIL" }
#define EXE_ACTIVITY_2_PORTS_UTIL                                              \
    { 0x04a6, PERF_TYPE_RAW, "EXE_ACTIVITY_2_PORTS_UTIL" }
#define EXE_ACTIVITY_3_PORTS_UTIL                                              \
    { 0x08a6, PERF_TYPE_RAW, "EXE_ACTIVITY_3_PORTS_UTIL" }
#define EXE_ACTIVITY_4_PORTS_UTIL                                              \
    { 0x10a6, PERF_TYPE_RAW, "EXE_ACTIVITY_4_PORTS_UTIL" }
#define EXE_ACTIVITY_EXE_BOUND_0_PORTS                                         \
    { 0x80a6, PERF_TYPE_RAW, "EXE_ACTIVITY_EXE_BOUND_0_PORTS" }
#define FP_ARITH_128B_PACKED_DOUBLE                                            \
    { 0x04c7, PERF_TYPE_RAW, "FP_ARITH_128B_PACKED_DOUBLE" }
#define FP_ARITH_128B_PACKED_SINGLE                                            \
    { 0x08c7, PERF_TYPE_RAW, "FP_ARITH_128B_PACKED_SINGLE" }
#define FP_ARITH_256B_PACKED_DOUBLE                                            \
    { 0x10c7, PERF_TYPE_RAW, "FP_ARITH_256B_PACKED_DOUBLE" }
#define FP_ARITH_256B_PACKED_SINGLE                                            \
    { 0x20c7, PERF_TYPE_RAW, "FP_ARITH_256B_PACKED_SINGLE" }
#define FP_ARITH_512B_PACKED_DOUBLE                                            \
    { 0x40c7, PERF_TYPE_RAW, "FP_ARITH_512B_PACKED_DOUBLE" }
#define FP_ARITH_512B_PACKED_SINGLE                                            \
    { 0x80c7, PERF_TYPE_RAW, "FP_ARITH_512B_PACKED_SINGLE" }
#define FP_ARITH_SCALAR_DOUBLE                                                 \
    { 0x01c7, PERF_TYPE_RAW, "FP_ARITH_SCALAR_DOUBLE" }
#define FP_ARITH_SCALAR_SINGLE                                                 \
    { 0x02c7, PERF_TYPE_RAW, "FP_ARITH_SCALAR_SINGLE" }
#define HLE_RETIRED_ABORTED                                                    \
    { 0x04c8, PERF_TYPE_RAW, "HLE_RETIRED_ABORTED" }
#define HLE_RETIRED_ABORTED_EVENTS                                             \
    { 0x80c8, PERF_TYPE_RAW, "HLE_RETIRED_ABORTED_EVENTS" }
#define HLE_RETIRED_ABORTED_MEM                                                \
    { 0x08c8, PERF_TYPE_RAW, "HLE_RETIRED_ABORTED_MEM" }
#define HLE_RETIRED_ABORTED_UNFRIENDLY                                         \
    { 0x20c8, PERF_TYPE_RAW, "HLE_RETIRED_ABORTED_UNFRIENDLY" }
#define HLE_RETIRED_COMMIT                                                     \
    { 0x02c8, PERF_TYPE_RAW, "HLE_RETIRED_COMMIT" }
#define HLE_RETIRED_START                                                      \
    { 0x01c8, PERF_TYPE_RAW, "HLE_RETIRED_START" }
#define ICACHE_16B_IFDATA_STALL                                                \
    { 0x0480, PERF_TYPE_RAW, "ICACHE_16B_IFDATA_STALL" }
#define ICACHE_64B_IFTAG_HIT                                                   \
    { 0x0183, PERF_TYPE_RAW, "ICACHE_64B_IFTAG_HIT" }
#define ICACHE_64B_IFTAG_MISS                                                  \
    { 0x0283, PERF_TYPE_RAW, "ICACHE_64B_IFTAG_MISS" }
#define ICACHE_64B_IFTAG_STALL                                                 \
    { 0x0483, PERF_TYPE_RAW, "ICACHE_64B_IFTAG_STALL" }
#define IDQ_DSB_UOPS                                                           \
    { 0x0879, PERF_TYPE_RAW, "IDQ_DSB_UOPS" }
#define IDQ_MITE_UOPS                                                          \
    { 0x0479, PERF_TYPE_RAW, "IDQ_MITE_UOPS" }
#define IDQ_MS_UOPS                                                            \
    { 0x3079, PERF_TYPE_RAW, "IDQ_MS_UOPS" }
#define IDQ_UOPS_NOT_DELIVERED_CORE                                            \
    { 0x019c, PERF_TYPE_RAW, "IDQ_UOPS_NOT_DELIVERED_CORE" }
#define ILD_STALL_LCP                                                          \
    { 0x0187, PERF_TYPE_RAW, "ILD_STALL_LCP" }
#define INST_RETIRED_ANY_P                                                     \
    { 0x00c0, PERF_TYPE_RAW, "INST_RETIRED_ANY_P" }
#define INT_MISC_CLEAR_RESTEER_CYCLES                                          \
    { 0x800d, PERF_TYPE_RAW, "INT_MISC_CLEAR_RESTEER_CYCLES" }
#define INT_MISC_RECOVERY_CYCLES                                               \
    { 0x010d, PERF_TYPE_RAW, "INT_MISC_RECOVERY_CYCLES" }
#define INT_MISC_UOP_DROPPING                                                  \
    { 0x100d, PERF_TYPE_RAW, "INT_MISC_UOP_DROPPING" }
#define ITLB_MISSES_STLB_HIT                                                   \
    { 0x2085, PERF_TYPE_RAW, "ITLB_MISSES_STLB_HIT" }
#define ITLB_MISSES_WALK_COMPLETED                                             \
    { 0x0e85, PERF_TYPE_RAW, "ITLB_MISSES_WALK_COMPLETED" }
#define ITLB_MISSES_WALK_COMPLETED_2M_4M                                       \
    { 0x0485, PERF_TYPE_RAW, "ITLB_MISSES_WALK_COMPLETED_2M_4M" }
#define ITLB_MISSES_WALK_COMPLETED_4K                                          \
    { 0x0285, PERF_TYPE_RAW, "ITLB_MISSES_WALK_COMPLETED_4K" }
#define ITLB_MISSES_WALK_PENDING                                               \
    { 0x1085, PERF_TYPE_RAW, "ITLB_MISSES_WALK_PENDING" }
#define L1D_PEND_MISS_FB_FULL                                                  \
    { 0x0248, PERF_TYPE_RAW, "L1D_PEND_MISS_FB_FULL" }
#define L1D_PEND_MISS_L2_STALL                                                 \
    { 0x0448, PERF_TYPE_RAW, "L1D_PEND_MISS_L2_STALL" }
#define L1D_PEND_MISS_PENDING                                                  \
    { 0x0148, PERF_TYPE_RAW, "L1D_PEND_MISS_PENDING" }
#define L1D_REPLACEMENT                                                        \
    { 0x0151, PERF_TYPE_RAW, "L1D_REPLACEMENT" }
#define L2_LINES_IN_ALL                                                        \
    { 0x1ff1, PERF_TYPE_RAW, "L2_LINES_IN_ALL" }
#define L2_LINES_OUT_NON_SILENT                                                \
    { 0x02f2, PERF_TYPE_RAW, "L2_LINES_OUT_NON_SILENT" }
#define L2_LINES_OUT_SILENT                                                    \
    { 0x01f2, PERF_TYPE_RAW, "L2_LINES_OUT_SILENT" }
#define L2_LINES_OUT_USELESS_HWPF                                              \
    { 0x04f2, PERF_TYPE_RAW, "L2_LINES_OUT_USELESS_HWPF" }
#define L2_RQSTS_ALL_CODE_RD                                                   \
    { 0xe424, PERF_TYPE_RAW, "L2_RQSTS_ALL_CODE_RD" }
#define L2_RQSTS_ALL_DEMAND_DATA_RD                                            \
    { 0xe124, PERF_TYPE_RAW, "L2_RQSTS_ALL_DEMAND_DATA_RD" }
#define L2_RQSTS_ALL_DEMAND_MISS                                               \
    { 0x2724, PERF_TYPE_RAW, "L2_RQSTS_ALL_DEMAND_MISS" }
#define L2_RQSTS_ALL_DEMAND_REFERENCES                                         \
    { 0xe724, PERF_TYPE_RAW, "L2_RQSTS_ALL_DEMAND_REFERENCES" }
#define L2_RQSTS_ALL_RFO                                                       \
    { 0xe224, PERF_TYPE_RAW, "L2_RQSTS_ALL_RFO" }
#define L2_RQSTS_CODE_RD_HIT                                                   \
    { 0xc424, PERF_TYPE_RAW, "L2_RQSTS_CODE_RD_HIT" }
#define L2_RQSTS_CODE_RD_MISS                                                  \
    { 0x2424, PERF_TYPE_RAW, "L2_RQSTS_CODE_RD_MISS" }
#define L2_RQSTS_DEMAND_DATA_RD_HIT                                            \
    { 0xc124, PERF_TYPE_RAW, "L2_RQSTS_DEMAND_DATA_RD_HIT" }
#define L2_RQSTS_DEMAND_DATA_RD_MISS                                           \
    { 0x2124, PERF_TYPE_RAW, "L2_RQSTS_DEMAND_DATA_RD_MISS" }
#define L2_RQSTS_RFO_HIT                                                       \
    { 0xc224, PERF_TYPE_RAW, "L2_RQSTS_RFO_HIT" }
#define L2_RQSTS_RFO_MISS                                                      \
    { 0x2224, PERF_TYPE_RAW, "L2_RQSTS_RFO_MISS" }
#define L2_RQSTS_SWPF_HIT                                                      \
    { 0xc824, PERF_TYPE_RAW, "L2_RQSTS_SWPF_HIT" }
#define L2_RQSTS_SWPF_MISS                                                     \
    { 0x2824, PERF_TYPE_RAW, "L2_RQSTS_SWPF_MISS" }
#define L2_TRANS_L2_WB                                                         \
    { 0x40f0, PERF_TYPE_RAW, "L2_TRANS_L2_WB" }
#define LD_BLOCKS_NO_SR                                                        \
    { 0x0803, PERF_TYPE_RAW, "LD_BLOCKS_NO_SR" }
#define LD_BLOCKS_PARTIAL_ADDRESS_ALIAS                                        \
    { 0x0107, PERF_TYPE_RAW, "LD_BLOCKS_PARTIAL_ADDRESS_ALIAS" }
#define LD_BLOCKS_STORE_FORWARD                                                \
    { 0x0203, PERF_TYPE_RAW, "LD_BLOCKS_STORE_FORWARD" }
#define LLC_MISSES                                                             \
    { 0x412e, PERF_TYPE_RAW, "LLC_MISSES" }
#define LLC_REFERENCES                                                         \
    { 0x4f2e, PERF_TYPE_RAW, "LLC_REFERENCES" }
#define LOAD_HIT_PRE_SWPF                                                      \
    { 0x014c, PERF_TYPE_RAW, "LOAD_HIT_PRE_SWPF" }
#define LONGEST_LAT_CACHE_MISS                                                 \
    { 0x012e, PERF_TYPE_RAW, "LONGEST_LAT_CACHE_MISS" }
#define LSD_UOPS                                                               \
    { 0x01a8, PERF_TYPE_RAW, "LSD_UOPS" }
#define MACHINE_CLEARS_MEMORY_ORDERING                                         \
    { 0x02c3, PERF_TYPE_RAW, "MACHINE_CLEARS_MEMORY_ORDERING" }
#define MACHINE_CLEARS_SMC                                                     \
    { 0x04c3, PERF_TYPE_RAW, "MACHINE_CLEARS_SMC" }
#define MEM_INST_RETIRED_ALL_LOADS                                             \
    { 0x81d0, PERF_TYPE_RAW, "MEM_INST_RETIRED_ALL_LOADS" }
#define MEM_INST_RETIRED_ALL_STORES                                            \
    { 0x82d0, PERF_TYPE_RAW, "MEM_INST_RETIRED_ALL_STORES" }
#define MEM_INST_RETIRED_LOCK_LOADS                                            \
    { 0x21d0, PERF_TYPE_RAW, "MEM_INST_RETIRED_LOCK_LOADS" }
#define MEM_INST_RETIRED_SPLIT_LOADS                                           \
    { 0x41d0, PERF_TYPE_RAW, "MEM_INST_RETIRED_SPLIT_LOADS" }
#define MEM_INST_RETIRED_SPLIT_STORES                                          \
    { 0x42d0, PERF_TYPE_RAW, "MEM_INST_RETIRED_SPLIT_STORES" }
#define MEM_INST_RETIRED_STLB_MISS_LOADS                                       \
    { 0x11d0, PERF_TYPE_RAW, "MEM_INST_RETIRED_STLB_MISS_LOADS" }
#define MEM_INST_RETIRED_STLB_MISS_STORES                                      \
    { 0x12d0, PERF_TYPE_RAW, "MEM_INST_RETIRED_STLB_MISS_STORES" }
#define MEM_LOAD_L3_HIT_RETIRED_XSNP_HIT                                       \
    { 0x02d2, PERF_TYPE_RAW, "MEM_LOAD_L3_HIT_RETIRED_XSNP_HIT" }
#define MEM_LOAD_L3_HIT_RETIRED_XSNP_HITM                                      \
    { 0x04d2, PERF_TYPE_RAW, "MEM_LOAD_L3_HIT_RETIRED_XSNP_HITM" }
#define MEM_LOAD_L3_HIT_RETIRED_XSNP_MISS                                      \
    { 0x01d2, PERF_TYPE_RAW, "MEM_LOAD_L3_HIT_RETIRED_XSNP_MISS" }
#define MEM_LOAD_L3_HIT_RETIRED_XSNP_NONE                                      \
    { 0x08d2, PERF_TYPE_RAW, "MEM_LOAD_L3_HIT_RETIRED_XSNP_NONE" }
#define MEM_LOAD_RETIRED_FB_HIT                                                \
    { 0x40d1, PERF_TYPE_RAW, "MEM_LOAD_RETIRED_FB_HIT" }
#define MEM_LOAD_RETIRED_L1_HIT                                                \
    { 0x01d1, PERF_TYPE_RAW, "MEM_LOAD_RETIRED_L1_HIT" }
#define MEM_LOAD_RETIRED_L1_MISS                                               \
    { 0x08d1, PERF_TYPE_RAW, "MEM_LOAD_RETIRED_L1_MISS" }
#define MEM_LOAD_RETIRED_L2_HIT                                                \
    { 0x02d1, PERF_TYPE_RAW, "MEM_LOAD_RETIRED_L2_HIT" }
#define MEM_LOAD_RETIRED_L2_MISS                                               \
    { 0x10d1, PERF_TYPE_RAW, "MEM_LOAD_RETIRED_L2_MISS" }
#define MEM_LOAD_RETIRED_L3_HIT                                                \
    { 0x04d1, PERF_TYPE_RAW, "MEM_LOAD_RETIRED_L3_HIT" }
#define MEM_LOAD_RETIRED_L3_MISS                                               \
    { 0x20d1, PERF_TYPE_RAW, "MEM_LOAD_RETIRED_L3_MISS" }
#define MISC_RETIRED_LBR_INSERTS                                               \
    { 0x20cc, PERF_TYPE_RAW, "MISC_RETIRED_LBR_INSERTS" }
#define MISC_RETIRED_PAUSE_INST                                                \
    { 0x40cc, PERF_TYPE_RAW, "MISC_RETIRED_PAUSE_INST" }
#define MISPREDICTED_BRANCH_RETIRED                                            \
    { 0x00c5, PERF_TYPE_RAW, "MISPREDICTED_BRANCH_RETIRED" }
#define OFFCORE_REQUESTS_ALL_DATA_RD                                           \
    { 0x08b0, PERF_TYPE_RAW, "OFFCORE_REQUESTS_ALL_DATA_RD" }
#define OFFCORE_REQUESTS_ALL_REQUESTS                                          \
    { 0x80b0, PERF_TYPE_RAW, "OFFCORE_REQUESTS_ALL_REQUESTS" }
#define OFFCORE_REQUESTS_DEMAND_DATA_RD                                        \
    { 0x01b0, PERF_TYPE_RAW, "OFFCORE_REQUESTS_DEMAND_DATA_RD" }
#define OFFCORE_REQUESTS_DEMAND_RFO                                            \
    { 0x04b0, PERF_TYPE_RAW, "OFFCORE_REQUESTS_DEMAND_RFO" }
#define OFFCORE_REQUESTS_L3_MISS_DEMAND_DATA_RD                                \
    { 0x10b0, PERF_TYPE_RAW, "OFFCORE_REQUESTS_L3_MISS_DEMAND_DATA_RD" }
#define OFFCORE_REQUESTS_OUTSTANDING_ALL_DATA_RD                               \
    { 0x0860, PERF_TYPE_RAW, "OFFCORE_REQUESTS_OUTSTANDING_ALL_DATA_RD" }
#define OFFCORE_REQUESTS_OUTSTANDING_DEMAND_DATA_RD                            \
    { 0x0160, PERF_TYPE_RAW, "OFFCORE_REQUESTS_OUTSTANDING_DEMAND_DATA_RD" }
#define OFFCORE_REQUESTS_OUTSTANDING_DEMAND_RFO                                \
    { 0x0460, PERF_TYPE_RAW, "OFFCORE_REQUESTS_OUTSTANDING_DEMAND_RFO" }
#define RESOURCE_STALLS_SB                                                     \
    { 0x08a2, PERF_TYPE_RAW, "RESOURCE_STALLS_SB" }
#define RESOURCE_STALLS_SCOREBOARD                                             \
    { 0x02a2, PERF_TYPE_RAW, "RESOURCE_STALLS_SCOREBOARD" }
#define RS_EVENTS_EMPTY_CYCLES                                                 \
    { 0x015e, PERF_TYPE_RAW, "RS_EVENTS_EMPTY_CYCLES" }
#define RTM_RETIRED_ABORTED                                                    \
    { 0x04c9, PERF_TYPE_RAW, "RTM_RETIRED_ABORTED" }
#define RTM_RETIRED_ABORTED_EVENTS                                             \
    { 0x80c9, PERF_TYPE_RAW, "RTM_RETIRED_ABORTED_EVENTS" }
#define RTM_RETIRED_ABORTED_MEM                                                \
    { 0x08c9, PERF_TYPE_RAW, "RTM_RETIRED_ABORTED_MEM" }
#define RTM_RETIRED_ABORTED_MEMTYPE                                            \
    { 0x40c9, PERF_TYPE_RAW, "RTM_RETIRED_ABORTED_MEMTYPE" }
#define RTM_RETIRED_ABORTED_UNFRIENDLY                                         \
    { 0x20c9, PERF_TYPE_RAW, "RTM_RETIRED_ABORTED_UNFRIENDLY" }
#define RTM_RETIRED_COMMIT                                                     \
    { 0x02c9, PERF_TYPE_RAW, "RTM_RETIRED_COMMIT" }
#define RTM_RETIRED_START                                                      \
    { 0x01c9, PERF_TYPE_RAW, "RTM_RETIRED_START" }
#define SQ_MISC_SQ_FULL                                                        \
    { 0x04f4, PERF_TYPE_RAW, "SQ_MISC_SQ_FULL" }
#define SW_PREFETCH_NTA                                                        \
    { 0x0132, PERF_TYPE_RAW, "SW_PREFETCH_NTA" }
#define SW_PREFETCH_PREFETCHW                                                  \
    { 0x0832, PERF_TYPE_RAW, "SW_PREFETCH_PREFETCHW" }
#define SW_PREFETCH_T0                                                         \
    { 0x0232, PERF_TYPE_RAW, "SW_PREFETCH_T0" }
#define SW_PREFETCH_T1_T2                                                      \
    { 0x0432, PERF_TYPE_RAW, "SW_PREFETCH_T1_T2" }
#define TLB_FLUSH_DTLB_THREAD                                                  \
    { 0x01bd, PERF_TYPE_RAW, "TLB_FLUSH_DTLB_THREAD" }
#define TLB_FLUSH_STLB_ANY                                                     \
    { 0x20bd, PERF_TYPE_RAW, "TLB_FLUSH_STLB_ANY" }
#define TOPDOWN_BACKEND_BOUND_SLOTS                                            \
    { 0x0200, PERF_TYPE_RAW, "TOPDOWN_BACKEND_BOUND_SLOTS" }
#define TOPDOWN_BR_MISPREDICT_SLOTS                                            \
    { 0x0800, PERF_TYPE_RAW, "TOPDOWN_BR_MISPREDICT_SLOTS" }
#define TOPDOWN_SLOTS                                                          \
    { 0x0400, PERF_TYPE_RAW, "TOPDOWN_SLOTS" }
#define TOPDOWN_SLOTS_P                                                        \
    { 0x0100, PERF_TYPE_RAW, "TOPDOWN_SLOTS_P" }
#define TX_EXEC_MISC2                                                          \
    { 0x025d, PERF_TYPE_RAW, "TX_EXEC_MISC2" }
#define TX_EXEC_MISC3                                                          \
    { 0x045d, PERF_TYPE_RAW, "TX_EXEC_MISC3" }
#define TX_MEM_ABORT_CAPACITY_READ                                             \
    { 0x8054, PERF_TYPE_RAW, "TX_MEM_ABORT_CAPACITY_READ" }
#define TX_MEM_ABORT_CAPACITY_WRITE                                            \
    { 0x0254, PERF_TYPE_RAW, "TX_MEM_ABORT_CAPACITY_WRITE" }
#define TX_MEM_ABORT_CONFLICT                                                  \
    { 0x0154, PERF_TYPE_RAW, "TX_MEM_ABORT_CONFLICT" }
#define TX_MEM_ABORT_HLE_ELISION_BUFFER_MISMATCH                               \
    { 0x1054, PERF_TYPE_RAW, "TX_MEM_ABORT_HLE_ELISION_BUFFER_MISMATCH" }
#define TX_MEM_ABORT_HLE_ELISION_BUFFER_NOT_EMPTY                              \
    { 0x0854, PERF_TYPE_RAW, "TX_MEM_ABORT_HLE_ELISION_BUFFER_NOT_EMPTY" }
#define TX_MEM_ABORT_HLE_ELISION_BUFFER_UNSUPPORTED_ALIGNMENT                  \
    {                                                                          \
        0x2054, PERF_TYPE_RAW,                                                 \
            "TX_MEM_ABORT_HLE_ELISION_BUFFER_UNSUPPORTED_ALIGNMENT"            \
    }
#define TX_MEM_ABORT_HLE_STORE_TO_ELIDED_LOCK                                  \
    { 0x0454, PERF_TYPE_RAW, "TX_MEM_ABORT_HLE_STORE_TO_ELIDED_LOCK" }
#define TX_MEM_HLE_ELISION_BUFFER_FULL                                         \
    { 0x4054, PERF_TYPE_RAW, "TX_MEM_HLE_ELISION_BUFFER_FULL" }
#define UNHALTED_CORE_CYCLES                                                   \
    { 0x003c, PERF_TYPE_RAW, "UNHALTED_CORE_CYCLES" }
#define UNHALTED_REFERENCE_CYCLES                                              \
    { 0x0300, PERF_TYPE_RAW, "UNHALTED_REFERENCE_CYCLES" }
#define UOPS_DISPATCHED_PORT_0                                                 \
    { 0x01a1, PERF_TYPE_RAW, "UOPS_DISPATCHED_PORT_0" }
#define UOPS_DISPATCHED_PORT_1                                                 \
    { 0x02a1, PERF_TYPE_RAW, "UOPS_DISPATCHED_PORT_1" }
#define UOPS_DISPATCHED_PORT_2_3                                               \
    { 0x04a1, PERF_TYPE_RAW, "UOPS_DISPATCHED_PORT_2_3" }
#define UOPS_DISPATCHED_PORT_4_9                                               \
    { 0x10a1, PERF_TYPE_RAW, "UOPS_DISPATCHED_PORT_4_9" }
#define UOPS_DISPATCHED_PORT_5                                                 \
    { 0x20a1, PERF_TYPE_RAW, "UOPS_DISPATCHED_PORT_5" }
#define UOPS_DISPATCHED_PORT_6                                                 \
    { 0x40a1, PERF_TYPE_RAW, "UOPS_DISPATCHED_PORT_6" }
#define UOPS_DISPATCHED_PORT_7_8                                               \
    { 0x80a1, PERF_TYPE_RAW, "UOPS_DISPATCHED_PORT_7_8" }
#define UOPS_EXECUTED_CORE                                                     \
    { 0x02b1, PERF_TYPE_RAW, "UOPS_EXECUTED_CORE" }
#define UOPS_EXECUTED_THREAD                                                   \
    { 0x01b1, PERF_TYPE_RAW, "UOPS_EXECUTED_THREAD" }
#define UOPS_EXECUTED_X87                                                      \
    { 0x10b1, PERF_TYPE_RAW, "UOPS_EXECUTED_X87" }
#define UOPS_ISSUED_ANY                                                        \
    { 0x010e, PERF_TYPE_RAW, "UOPS_ISSUED_ANY" }
#define UOPS_RETIRED_SLOTS                                                     \
    { 0x02c2, PERF_TYPE_RAW, "UOPS_RETIRED_SLOTS" }

#endif
