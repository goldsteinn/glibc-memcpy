#ifndef _SKL_EVENTS_H_
#define _SKL_EVENTS_H_

#define BACLEARS_ANY                                                           \
    { 0x01e6, PERF_TYPE_RAW, "BACLEARS_ANY" }
#define BRANCH_INSTRUCTIONS_RETIRED                                            \
    { 0x00c4, PERF_TYPE_RAW, "BRANCH_INSTRUCTIONS_RETIRED" }
#define BR_INST_RETIRED_COND                                                   \
    { 0x01c4, PERF_TYPE_RAW, "BR_INST_RETIRED_COND" }
#define BR_INST_RETIRED_FAR_BRANCH                                             \
    { 0x40c4, PERF_TYPE_RAW, "BR_INST_RETIRED_FAR_BRANCH" }
#define BR_INST_RETIRED_NEAR_CALL                                              \
    { 0x02c4, PERF_TYPE_RAW, "BR_INST_RETIRED_NEAR_CALL" }
#define BR_INST_RETIRED_NEAR_RETURN                                            \
    { 0x08c4, PERF_TYPE_RAW, "BR_INST_RETIRED_NEAR_RETURN" }
#define BR_INST_RETIRED_NEAR_TAKEN                                             \
    { 0x20c4, PERF_TYPE_RAW, "BR_INST_RETIRED_NEAR_TAKEN" }
#define BR_INST_RETIRED_NOT_TAKEN                                              \
    { 0x10c4, PERF_TYPE_RAW, "BR_INST_RETIRED_NOT_TAKEN" }
#define BR_MISP_RETIRED_COND                                                   \
    { 0x01c5, PERF_TYPE_RAW, "BR_MISP_RETIRED_COND" }
#define BR_MISP_RETIRED_NEAR_CALL                                              \
    { 0x02c5, PERF_TYPE_RAW, "BR_MISP_RETIRED_NEAR_CALL" }
#define BR_MISP_RETIRED_NEAR_TAKEN                                             \
    { 0x20c5, PERF_TYPE_RAW, "BR_MISP_RETIRED_NEAR_TAKEN" }
#define CORE_SNOOP_RESPONSE_RSP_IFWDFE                                         \
    { 0x20ef, PERF_TYPE_RAW, "CORE_SNOOP_RESPONSE_RSP_IFWDFE" }
#define CORE_SNOOP_RESPONSE_RSP_IFWDM                                          \
    { 0x10ef, PERF_TYPE_RAW, "CORE_SNOOP_RESPONSE_RSP_IFWDM" }
#define CORE_SNOOP_RESPONSE_RSP_IHITFSE                                        \
    { 0x02ef, PERF_TYPE_RAW, "CORE_SNOOP_RESPONSE_RSP_IHITFSE" }
#define CORE_SNOOP_RESPONSE_RSP_IHITI                                          \
    { 0x01ef, PERF_TYPE_RAW, "CORE_SNOOP_RESPONSE_RSP_IHITI" }
#define CORE_SNOOP_RESPONSE_RSP_SFWDFE                                         \
    { 0x40ef, PERF_TYPE_RAW, "CORE_SNOOP_RESPONSE_RSP_SFWDFE" }
#define CORE_SNOOP_RESPONSE_RSP_SFWDM                                          \
    { 0x08ef, PERF_TYPE_RAW, "CORE_SNOOP_RESPONSE_RSP_SFWDM" }
#define CORE_SNOOP_RESPONSE_RSP_SHITFSE                                        \
    { 0x04ef, PERF_TYPE_RAW, "CORE_SNOOP_RESPONSE_RSP_SHITFSE" }
#define CPU_CLK_THREAD_UNHALTED_ONE_THREAD_ACTIVE                              \
    { 0x023c, PERF_TYPE_RAW, "CPU_CLK_THREAD_UNHALTED_ONE_THREAD_ACTIVE" }
#define CPU_CLK_UNHALTED                                                       \
    { 0x003c, PERF_TYPE_RAW, "CPU_CLK_UNHALTED" }
#define DSB2MITE_SWITCHES_PENALTY_CYCLES                                       \
    { 0x02ab, PERF_TYPE_RAW, "DSB2MITE_SWITCHES_PENALTY_CYCLES" }
#define DTLB_LOAD_MISSES_MISS_CAUSES_A_WALK                                    \
    { 0x0108, PERF_TYPE_RAW, "DTLB_LOAD_MISSES_MISS_CAUSES_A_WALK" }
#define DTLB_LOAD_MISSES_STLB_HIT                                              \
    { 0x2008, PERF_TYPE_RAW, "DTLB_LOAD_MISSES_STLB_HIT" }
#define DTLB_LOAD_MISSES_WALK_COMPLETED                                        \
    { 0x0e08, PERF_TYPE_RAW, "DTLB_LOAD_MISSES_WALK_COMPLETED" }
#define DTLB_LOAD_MISSES_WALK_COMPLETED_1G                                     \
    { 0x0808, PERF_TYPE_RAW, "DTLB_LOAD_MISSES_WALK_COMPLETED_1G" }
#define DTLB_LOAD_MISSES_WALK_COMPLETED_2M_4M                                  \
    { 0x0408, PERF_TYPE_RAW, "DTLB_LOAD_MISSES_WALK_COMPLETED_2M_4M" }
#define DTLB_LOAD_MISSES_WALK_COMPLETED_4K                                     \
    { 0x0208, PERF_TYPE_RAW, "DTLB_LOAD_MISSES_WALK_COMPLETED_4K" }
#define DTLB_LOAD_MISSES_WALK_PENDING                                          \
    { 0x1008, PERF_TYPE_RAW, "DTLB_LOAD_MISSES_WALK_PENDING" }
#define DTLB_STORE_MISSES_MISS_CAUSES_A_WALK                                   \
    { 0x0149, PERF_TYPE_RAW, "DTLB_STORE_MISSES_MISS_CAUSES_A_WALK" }
#define DTLB_STORE_MISSES_STLB_HIT                                             \
    { 0x2049, PERF_TYPE_RAW, "DTLB_STORE_MISSES_STLB_HIT" }
#define DTLB_STORE_MISSES_WALK_COMPLETED                                       \
    { 0x0e49, PERF_TYPE_RAW, "DTLB_STORE_MISSES_WALK_COMPLETED" }
#define DTLB_STORE_MISSES_WALK_COMPLETED_1G                                    \
    { 0x0849, PERF_TYPE_RAW, "DTLB_STORE_MISSES_WALK_COMPLETED_1G" }
#define DTLB_STORE_MISSES_WALK_COMPLETED_2M_4M                                 \
    { 0x0449, PERF_TYPE_RAW, "DTLB_STORE_MISSES_WALK_COMPLETED_2M_4M" }
#define DTLB_STORE_MISSES_WALK_COMPLETED_4K                                    \
    { 0x0249, PERF_TYPE_RAW, "DTLB_STORE_MISSES_WALK_COMPLETED_4K" }
#define DTLB_STORE_MISSES_WALK_PENDING                                         \
    { 0x1049, PERF_TYPE_RAW, "DTLB_STORE_MISSES_WALK_PENDING" }
#define EPT_WALK_PENDING                                                       \
    { 0x104f, PERF_TYPE_RAW, "EPT_WALK_PENDING" }
#define EXE_ACTIVITY_1_PORTS_UTIL                                              \
    { 0x02a6, PERF_TYPE_RAW, "EXE_ACTIVITY_1_PORTS_UTIL" }
#define EXE_ACTIVITY_2_PORTS_UTIL                                              \
    { 0x04a6, PERF_TYPE_RAW, "EXE_ACTIVITY_2_PORTS_UTIL" }
#define EXE_ACTIVITY_3_PORTS_UTIL                                              \
    { 0x08a6, PERF_TYPE_RAW, "EXE_ACTIVITY_3_PORTS_UTIL" }
#define EXE_ACTIVITY_4_PORTS_UTIL                                              \
    { 0x10a6, PERF_TYPE_RAW, "EXE_ACTIVITY_4_PORTS_UTIL" }
#define EXE_ACTIVITY_BOUND_ON_STORES                                           \
    { 0x40a6, PERF_TYPE_RAW, "EXE_ACTIVITY_BOUND_ON_STORES" }
#define EXE_ACTIVITY_EXE_BOUND_0_PORTS                                         \
    { 0x01a6, PERF_TYPE_RAW, "EXE_ACTIVITY_EXE_BOUND_0_PORTS" }
#define FP_ARITH_128B_PACKED_DOUBLE                                            \
    { 0x04c7, PERF_TYPE_RAW, "FP_ARITH_128B_PACKED_DOUBLE" }
#define FP_ARITH_128B_PACKED_DOUBLE_128B_PACKED_SINGLE                         \
    { 0x0cc7, PERF_TYPE_RAW, "FP_ARITH_128B_PACKED_DOUBLE_128B_PACKED_SINGLE" }
#define FP_ARITH_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE      \
    {                                                                          \
        0x1cc7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_"      \
            "DOUBLE"                                                           \
    }
#define FP_ARITH_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE \
    {                                                                                        \
        0x3cc7, PERF_TYPE_RAW,                                                               \
            "FP_ARITH_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_"                    \
            "DOUBLE_256B_PACKED_SINGLE"                                                      \
    }
#define FP_ARITH_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE \
    {                                                                                                           \
        0x7cc7, PERF_TYPE_RAW,                                                                                  \
            "FP_ARITH_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_"                                       \
            "DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE"                                                      \
    }
#define FP_ARITH_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                                              \
        0xfcc7, PERF_TYPE_RAW,                                                                                                     \
            "FP_ARITH_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_"                                                          \
            "DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE"                                                      \
    }
#define FP_ARITH_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_SINGLE \
    {                                                                                                           \
        0xbcc7, PERF_TYPE_RAW,                                                                                  \
            "FP_ARITH_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_"                                       \
            "DOUBLE_256B_PACKED_SINGLE_512B_PACKED_SINGLE"                                                      \
    }
#define FP_ARITH_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE \
    {                                                                                        \
        0x5cc7, PERF_TYPE_RAW,                                                               \
            "FP_ARITH_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_"                    \
            "DOUBLE_512B_PACKED_DOUBLE"                                                      \
    }
#define FP_ARITH_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                           \
        0xdcc7, PERF_TYPE_RAW,                                                                                  \
            "FP_ARITH_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_"                                       \
            "DOUBLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE"                                                      \
    }
#define FP_ARITH_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                        \
        0x9cc7, PERF_TYPE_RAW,                                                               \
            "FP_ARITH_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_"                    \
            "DOUBLE_512B_PACKED_SINGLE"                                                      \
    }
#define FP_ARITH_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_SINGLE      \
    {                                                                          \
        0x2cc7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_"      \
            "SINGLE"                                                           \
    }
#define FP_ARITH_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE \
    {                                                                                        \
        0x6cc7, PERF_TYPE_RAW,                                                               \
            "FP_ARITH_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_"                    \
            "SINGLE_512B_PACKED_DOUBLE"                                                      \
    }
#define FP_ARITH_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                           \
        0xecc7, PERF_TYPE_RAW,                                                                                  \
            "FP_ARITH_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_"                                       \
            "SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE"                                                      \
    }
#define FP_ARITH_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_SINGLE_512B_PACKED_SINGLE \
    {                                                                                        \
        0xacc7, PERF_TYPE_RAW,                                                               \
            "FP_ARITH_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_"                    \
            "SINGLE_512B_PACKED_SINGLE"                                                      \
    }
#define FP_ARITH_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_512B_PACKED_DOUBLE      \
    {                                                                          \
        0x4cc7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_512B_PACKED_"      \
            "DOUBLE"                                                           \
    }
#define FP_ARITH_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                        \
        0xccc7, PERF_TYPE_RAW,                                                               \
            "FP_ARITH_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_512B_PACKED_"                    \
            "DOUBLE_512B_PACKED_SINGLE"                                                      \
    }
#define FP_ARITH_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_512B_PACKED_SINGLE      \
    {                                                                          \
        0x8cc7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_512B_PACKED_"      \
            "SINGLE"                                                           \
    }
#define FP_ARITH_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE                         \
    { 0x14c7, PERF_TYPE_RAW, "FP_ARITH_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE" }
#define FP_ARITH_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE      \
    {                                                                          \
        0x34c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_256B_PACKED_"      \
            "SINGLE"                                                           \
    }
#define FP_ARITH_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE \
    {                                                                                        \
        0x74c7, PERF_TYPE_RAW,                                                               \
            "FP_ARITH_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_256B_PACKED_"                    \
            "SINGLE_512B_PACKED_DOUBLE"                                                      \
    }
#define FP_ARITH_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                           \
        0xf4c7, PERF_TYPE_RAW,                                                                                  \
            "FP_ARITH_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_256B_PACKED_"                                       \
            "SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE"                                                      \
    }
#define FP_ARITH_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_SINGLE \
    {                                                                                        \
        0xb4c7, PERF_TYPE_RAW,                                                               \
            "FP_ARITH_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_256B_PACKED_"                    \
            "SINGLE_512B_PACKED_SINGLE"                                                      \
    }
#define FP_ARITH_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE      \
    {                                                                          \
        0x54c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_512B_PACKED_"      \
            "DOUBLE"                                                           \
    }
#define FP_ARITH_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                        \
        0xd4c7, PERF_TYPE_RAW,                                                               \
            "FP_ARITH_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_512B_PACKED_"                    \
            "DOUBLE_512B_PACKED_SINGLE"                                                      \
    }
#define FP_ARITH_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_512B_PACKED_SINGLE      \
    {                                                                          \
        0x94c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_512B_PACKED_"      \
            "SINGLE"                                                           \
    }
#define FP_ARITH_128B_PACKED_DOUBLE_256B_PACKED_SINGLE                         \
    { 0x24c7, PERF_TYPE_RAW, "FP_ARITH_128B_PACKED_DOUBLE_256B_PACKED_SINGLE" }
#define FP_ARITH_128B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE      \
    {                                                                          \
        0x64c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_128B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_"      \
            "DOUBLE"                                                           \
    }
#define FP_ARITH_128B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                        \
        0xe4c7, PERF_TYPE_RAW,                                                               \
            "FP_ARITH_128B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_"                    \
            "DOUBLE_512B_PACKED_SINGLE"                                                      \
    }
#define FP_ARITH_128B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_SINGLE      \
    {                                                                          \
        0xa4c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_128B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_"      \
            "SINGLE"                                                           \
    }
#define FP_ARITH_128B_PACKED_DOUBLE_512B_PACKED_DOUBLE                         \
    { 0x44c7, PERF_TYPE_RAW, "FP_ARITH_128B_PACKED_DOUBLE_512B_PACKED_DOUBLE" }
#define FP_ARITH_128B_PACKED_DOUBLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE      \
    {                                                                          \
        0xc4c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_128B_PACKED_DOUBLE_512B_PACKED_DOUBLE_512B_PACKED_"      \
            "SINGLE"                                                           \
    }
#define FP_ARITH_128B_PACKED_DOUBLE_512B_PACKED_SINGLE                         \
    { 0x84c7, PERF_TYPE_RAW, "FP_ARITH_128B_PACKED_DOUBLE_512B_PACKED_SINGLE" }
#define FP_ARITH_128B_PACKED_SINGLE                                            \
    { 0x08c7, PERF_TYPE_RAW, "FP_ARITH_128B_PACKED_SINGLE" }
#define FP_ARITH_128B_PACKED_SINGLE_256B_PACKED_DOUBLE                         \
    { 0x18c7, PERF_TYPE_RAW, "FP_ARITH_128B_PACKED_SINGLE_256B_PACKED_DOUBLE" }
#define FP_ARITH_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE      \
    {                                                                          \
        0x38c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_"      \
            "SINGLE"                                                           \
    }
#define FP_ARITH_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE \
    {                                                                                        \
        0x78c7, PERF_TYPE_RAW,                                                               \
            "FP_ARITH_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_"                    \
            "SINGLE_512B_PACKED_DOUBLE"                                                      \
    }
#define FP_ARITH_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                           \
        0xf8c7, PERF_TYPE_RAW,                                                                                  \
            "FP_ARITH_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_"                                       \
            "SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE"                                                      \
    }
#define FP_ARITH_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_SINGLE \
    {                                                                                        \
        0xb8c7, PERF_TYPE_RAW,                                                               \
            "FP_ARITH_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_"                    \
            "SINGLE_512B_PACKED_SINGLE"                                                      \
    }
#define FP_ARITH_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE      \
    {                                                                          \
        0x58c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_512B_PACKED_"      \
            "DOUBLE"                                                           \
    }
#define FP_ARITH_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                        \
        0xd8c7, PERF_TYPE_RAW,                                                               \
            "FP_ARITH_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_512B_PACKED_"                    \
            "DOUBLE_512B_PACKED_SINGLE"                                                      \
    }
#define FP_ARITH_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_512B_PACKED_SINGLE      \
    {                                                                          \
        0x98c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_512B_PACKED_"      \
            "SINGLE"                                                           \
    }
#define FP_ARITH_128B_PACKED_SINGLE_256B_PACKED_SINGLE                         \
    { 0x28c7, PERF_TYPE_RAW, "FP_ARITH_128B_PACKED_SINGLE_256B_PACKED_SINGLE" }
#define FP_ARITH_128B_PACKED_SINGLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE      \
    {                                                                          \
        0x68c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_128B_PACKED_SINGLE_256B_PACKED_SINGLE_512B_PACKED_"      \
            "DOUBLE"                                                           \
    }
#define FP_ARITH_128B_PACKED_SINGLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                        \
        0xe8c7, PERF_TYPE_RAW,                                                               \
            "FP_ARITH_128B_PACKED_SINGLE_256B_PACKED_SINGLE_512B_PACKED_"                    \
            "DOUBLE_512B_PACKED_SINGLE"                                                      \
    }
#define FP_ARITH_128B_PACKED_SINGLE_256B_PACKED_SINGLE_512B_PACKED_SINGLE      \
    {                                                                          \
        0xa8c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_128B_PACKED_SINGLE_256B_PACKED_SINGLE_512B_PACKED_"      \
            "SINGLE"                                                           \
    }
#define FP_ARITH_128B_PACKED_SINGLE_512B_PACKED_DOUBLE                         \
    { 0x48c7, PERF_TYPE_RAW, "FP_ARITH_128B_PACKED_SINGLE_512B_PACKED_DOUBLE" }
#define FP_ARITH_128B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE      \
    {                                                                          \
        0xc8c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_128B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_"      \
            "SINGLE"                                                           \
    }
#define FP_ARITH_128B_PACKED_SINGLE_512B_PACKED_SINGLE                         \
    { 0x88c7, PERF_TYPE_RAW, "FP_ARITH_128B_PACKED_SINGLE_512B_PACKED_SINGLE" }
#define FP_ARITH_256B_PACKED_DOUBLE                                            \
    { 0x10c7, PERF_TYPE_RAW, "FP_ARITH_256B_PACKED_DOUBLE" }
#define FP_ARITH_256B_PACKED_DOUBLE_256B_PACKED_SINGLE                         \
    { 0x30c7, PERF_TYPE_RAW, "FP_ARITH_256B_PACKED_DOUBLE_256B_PACKED_SINGLE" }
#define FP_ARITH_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE      \
    {                                                                          \
        0x70c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_"      \
            "DOUBLE"                                                           \
    }
#define FP_ARITH_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                        \
        0xf0c7, PERF_TYPE_RAW,                                                               \
            "FP_ARITH_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_"                    \
            "DOUBLE_512B_PACKED_SINGLE"                                                      \
    }
#define FP_ARITH_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_SINGLE      \
    {                                                                          \
        0xb0c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_"      \
            "SINGLE"                                                           \
    }
#define FP_ARITH_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE                         \
    { 0x50c7, PERF_TYPE_RAW, "FP_ARITH_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE" }
#define FP_ARITH_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE      \
    {                                                                          \
        0xd0c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE_512B_PACKED_"      \
            "SINGLE"                                                           \
    }
#define FP_ARITH_256B_PACKED_DOUBLE_512B_PACKED_SINGLE                         \
    { 0x90c7, PERF_TYPE_RAW, "FP_ARITH_256B_PACKED_DOUBLE_512B_PACKED_SINGLE" }
#define FP_ARITH_256B_PACKED_SINGLE                                            \
    { 0x20c7, PERF_TYPE_RAW, "FP_ARITH_256B_PACKED_SINGLE" }
#define FP_ARITH_256B_PACKED_SINGLE_512B_PACKED_DOUBLE                         \
    { 0x60c7, PERF_TYPE_RAW, "FP_ARITH_256B_PACKED_SINGLE_512B_PACKED_DOUBLE" }
#define FP_ARITH_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE      \
    {                                                                          \
        0xe0c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_"      \
            "SINGLE"                                                           \
    }
#define FP_ARITH_256B_PACKED_SINGLE_512B_PACKED_SINGLE                         \
    { 0xa0c7, PERF_TYPE_RAW, "FP_ARITH_256B_PACKED_SINGLE_512B_PACKED_SINGLE" }
#define FP_ARITH_512B_PACKED_DOUBLE                                            \
    { 0x40c7, PERF_TYPE_RAW, "FP_ARITH_512B_PACKED_DOUBLE" }
#define FP_ARITH_512B_PACKED_DOUBLE_512B_PACKED_SINGLE                         \
    { 0xc0c7, PERF_TYPE_RAW, "FP_ARITH_512B_PACKED_DOUBLE_512B_PACKED_SINGLE" }
#define FP_ARITH_512B_PACKED_SINGLE                                            \
    { 0x80c7, PERF_TYPE_RAW, "FP_ARITH_512B_PACKED_SINGLE" }
#define FP_ARITH_SCALAR_DOUBLE                                                 \
    { 0x01c7, PERF_TYPE_RAW, "FP_ARITH_SCALAR_DOUBLE" }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE                              \
    { 0x05c7, PERF_TYPE_RAW, "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE" }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE           \
    {                                                                          \
        0x0dc7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE"     \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE \
    {                                                                                   \
        0x1dc7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_"             \
            "256B_PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE \
    {                                                                                                      \
        0x3dc7, PERF_TYPE_RAW,                                                                             \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_"                                \
            "256B_PACKED_DOUBLE_256B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE \
    {                                                                                                                         \
        0x7dc7, PERF_TYPE_RAW,                                                                                                \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_"                                                   \
            "256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                                                            \
        0xfdc7, PERF_TYPE_RAW,                                                                                                                   \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_"                                                                      \
            "256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_"                                                                     \
            "PACKED_SINGLE"                                                                                                                      \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_SINGLE \
    {                                                                                                                         \
        0xbdc7, PERF_TYPE_RAW,                                                                                                \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_"                                                   \
            "256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE \
    {                                                                                                      \
        0x5dc7, PERF_TYPE_RAW,                                                                             \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_"                                \
            "256B_PACKED_DOUBLE_512B_PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                                         \
        0xddc7, PERF_TYPE_RAW,                                                                                                \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_"                                                   \
            "256B_PACKED_DOUBLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                      \
        0x9dc7, PERF_TYPE_RAW,                                                                             \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_"                                \
            "256B_PACKED_DOUBLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_SINGLE \
    {                                                                                   \
        0x2dc7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_"             \
            "256B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE \
    {                                                                                                      \
        0x6dc7, PERF_TYPE_RAW,                                                                             \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_"                                \
            "256B_PACKED_SINGLE_512B_PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                                         \
        0xedc7, PERF_TYPE_RAW,                                                                                                \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_"                                                   \
            "256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_SINGLE_512B_PACKED_SINGLE \
    {                                                                                                      \
        0xadc7, PERF_TYPE_RAW,                                                                             \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_"                                \
            "256B_PACKED_SINGLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_512B_PACKED_DOUBLE \
    {                                                                                   \
        0x4dc7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_"             \
            "512B_PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                      \
        0xcdc7, PERF_TYPE_RAW,                                                                             \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_"                                \
            "512B_PACKED_DOUBLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_512B_PACKED_SINGLE \
    {                                                                                   \
        0x8dc7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_"             \
            "512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE           \
    {                                                                          \
        0x15c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE"     \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE \
    {                                                                                   \
        0x35c7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_"             \
            "256B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE \
    {                                                                                                      \
        0x75c7, PERF_TYPE_RAW,                                                                             \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_"                                \
            "256B_PACKED_SINGLE_512B_PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                                         \
        0xf5c7, PERF_TYPE_RAW,                                                                                                \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_"                                                   \
            "256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_SINGLE \
    {                                                                                                      \
        0xb5c7, PERF_TYPE_RAW,                                                                             \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_"                                \
            "256B_PACKED_SINGLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE \
    {                                                                                   \
        0x55c7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_"             \
            "512B_PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                      \
        0xd5c7, PERF_TYPE_RAW,                                                                             \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_"                                \
            "512B_PACKED_DOUBLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                   \
        0x95c7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_"             \
            "512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_256B_PACKED_SINGLE           \
    {                                                                          \
        0x25c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_256B_PACKED_SINGLE"     \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE \
    {                                                                                   \
        0x65c7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_256B_PACKED_SINGLE_"             \
            "512B_PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                      \
        0xe5c7, PERF_TYPE_RAW,                                                                             \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_256B_PACKED_SINGLE_"                                \
            "512B_PACKED_DOUBLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_SINGLE \
    {                                                                                   \
        0xa5c7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_256B_PACKED_SINGLE_"             \
            "512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_512B_PACKED_DOUBLE           \
    {                                                                          \
        0x45c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_512B_PACKED_DOUBLE"     \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                   \
        0xc5c7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_512B_PACKED_DOUBLE_"             \
            "512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_512B_PACKED_SINGLE           \
    {                                                                          \
        0x85c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_DOUBLE_512B_PACKED_SINGLE"     \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_SINGLE                              \
    { 0x09c7, PERF_TYPE_RAW, "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_SINGLE" }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE           \
    {                                                                          \
        0x19c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE"     \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE \
    {                                                                                   \
        0x39c7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_"             \
            "256B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE \
    {                                                                                                      \
        0x79c7, PERF_TYPE_RAW,                                                                             \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_"                                \
            "256B_PACKED_SINGLE_512B_PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                                         \
        0xf9c7, PERF_TYPE_RAW,                                                                                                \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_"                                                   \
            "256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_SINGLE \
    {                                                                                                      \
        0xb9c7, PERF_TYPE_RAW,                                                                             \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_"                                \
            "256B_PACKED_SINGLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE \
    {                                                                                   \
        0x59c7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_"             \
            "512B_PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                      \
        0xd9c7, PERF_TYPE_RAW,                                                                             \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_"                                \
            "512B_PACKED_DOUBLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                   \
        0x99c7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_"             \
            "512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_SINGLE           \
    {                                                                          \
        0x29c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_SINGLE"     \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE \
    {                                                                                   \
        0x69c7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_SINGLE_"             \
            "512B_PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                      \
        0xe9c7, PERF_TYPE_RAW,                                                                             \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_SINGLE_"                                \
            "512B_PACKED_DOUBLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_SINGLE_512B_PACKED_SINGLE \
    {                                                                                   \
        0xa9c7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_SINGLE_"             \
            "512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_SINGLE_512B_PACKED_DOUBLE           \
    {                                                                          \
        0x49c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_SINGLE_512B_PACKED_DOUBLE"     \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                   \
        0xc9c7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_SINGLE_512B_PACKED_DOUBLE_"             \
            "512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_128B_PACKED_SINGLE_512B_PACKED_SINGLE           \
    {                                                                          \
        0x89c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_SCALAR_DOUBLE_128B_PACKED_SINGLE_512B_PACKED_SINGLE"     \
    }
#define FP_ARITH_SCALAR_DOUBLE_256B_PACKED_DOUBLE                              \
    { 0x11c7, PERF_TYPE_RAW, "FP_ARITH_SCALAR_DOUBLE_256B_PACKED_DOUBLE" }
#define FP_ARITH_SCALAR_DOUBLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE           \
    {                                                                          \
        0x31c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_SCALAR_DOUBLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE"     \
    }
#define FP_ARITH_SCALAR_DOUBLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE \
    {                                                                                   \
        0x71c7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_DOUBLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_"             \
            "512B_PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                      \
        0xf1c7, PERF_TYPE_RAW,                                                                             \
            "FP_ARITH_SCALAR_DOUBLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_"                                \
            "512B_PACKED_DOUBLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_SINGLE \
    {                                                                                   \
        0xb1c7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_DOUBLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_"             \
            "512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE           \
    {                                                                          \
        0x51c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_SCALAR_DOUBLE_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE"     \
    }
#define FP_ARITH_SCALAR_DOUBLE_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                   \
        0xd1c7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_DOUBLE_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE_"             \
            "512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_256B_PACKED_DOUBLE_512B_PACKED_SINGLE           \
    {                                                                          \
        0x91c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_SCALAR_DOUBLE_256B_PACKED_DOUBLE_512B_PACKED_SINGLE"     \
    }
#define FP_ARITH_SCALAR_DOUBLE_256B_PACKED_SINGLE                              \
    { 0x21c7, PERF_TYPE_RAW, "FP_ARITH_SCALAR_DOUBLE_256B_PACKED_SINGLE" }
#define FP_ARITH_SCALAR_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE           \
    {                                                                          \
        0x61c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_SCALAR_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE"     \
    }
#define FP_ARITH_SCALAR_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                   \
        0xe1c7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_"             \
            "512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_SINGLE           \
    {                                                                          \
        0xa1c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_SCALAR_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_SINGLE"     \
    }
#define FP_ARITH_SCALAR_DOUBLE_512B_PACKED_DOUBLE                              \
    { 0x41c7, PERF_TYPE_RAW, "FP_ARITH_SCALAR_DOUBLE_512B_PACKED_DOUBLE" }
#define FP_ARITH_SCALAR_DOUBLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE           \
    {                                                                          \
        0xc1c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_SCALAR_DOUBLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE"     \
    }
#define FP_ARITH_SCALAR_DOUBLE_512B_PACKED_SINGLE                              \
    { 0x81c7, PERF_TYPE_RAW, "FP_ARITH_SCALAR_DOUBLE_512B_PACKED_SINGLE" }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE                                   \
    { 0x03c7, PERF_TYPE_RAW, "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE" }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE                \
    {                                                                          \
        0x07c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE"          \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE \
    {                                                                              \
        0x0fc7, PERF_TYPE_RAW,                                                     \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_"        \
            "PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE \
    {                                                                                                 \
        0x1fc7, PERF_TYPE_RAW,                                                                        \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_"                           \
            "PACKED_SINGLE_256B_PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE \
    {                                                                                                                    \
        0x3fc7, PERF_TYPE_RAW,                                                                                           \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_"                                              \
            "PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE \
    {                                                                                                                                       \
        0x7fc7, PERF_TYPE_RAW,                                                                                                              \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_"                                                                 \
            "PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_"                                                              \
            "DOUBLE"                                                                                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                                                                          \
        0xffc7, PERF_TYPE_RAW,                                                                                                                                 \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_"                                                                                    \
            "PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_"                                                                                 \
            "DOUBLE_512B_PACKED_SINGLE"                                                                                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_SINGLE \
    {                                                                                                                                       \
        0xbfc7, PERF_TYPE_RAW,                                                                                                              \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_"                                                                 \
            "PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_"                                                              \
            "SINGLE"                                                                                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE \
    {                                                                                                                    \
        0x5fc7, PERF_TYPE_RAW,                                                                                           \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_"                                              \
            "PACKED_SINGLE_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                                                       \
        0xdfc7, PERF_TYPE_RAW,                                                                                                              \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_"                                                                 \
            "PACKED_SINGLE_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE_512B_PACKED_"                                                              \
            "SINGLE"                                                                                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                                    \
        0x9fc7, PERF_TYPE_RAW,                                                                                           \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_"                                              \
            "PACKED_SINGLE_256B_PACKED_DOUBLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_SINGLE \
    {                                                                                                 \
        0x2fc7, PERF_TYPE_RAW,                                                                        \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_"                           \
            "PACKED_SINGLE_256B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE \
    {                                                                                                                    \
        0x6fc7, PERF_TYPE_RAW,                                                                                           \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_"                                              \
            "PACKED_SINGLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                                                       \
        0xefc7, PERF_TYPE_RAW,                                                                                                              \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_"                                                                 \
            "PACKED_SINGLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_"                                                              \
            "SINGLE"                                                                                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_SINGLE_512B_PACKED_SINGLE \
    {                                                                                                                    \
        0xafc7, PERF_TYPE_RAW,                                                                                           \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_"                                              \
            "PACKED_SINGLE_256B_PACKED_SINGLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_512B_PACKED_DOUBLE \
    {                                                                                                 \
        0x4fc7, PERF_TYPE_RAW,                                                                        \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_"                           \
            "PACKED_SINGLE_512B_PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                                    \
        0xcfc7, PERF_TYPE_RAW,                                                                                           \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_"                                              \
            "PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_512B_PACKED_SINGLE \
    {                                                                                                 \
        0x8fc7, PERF_TYPE_RAW,                                                                        \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_"                           \
            "PACKED_SINGLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE \
    {                                                                              \
        0x17c7, PERF_TYPE_RAW,                                                     \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_"        \
            "PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE \
    {                                                                                                 \
        0x37c7, PERF_TYPE_RAW,                                                                        \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_"                           \
            "PACKED_DOUBLE_256B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE \
    {                                                                                                                    \
        0x77c7, PERF_TYPE_RAW,                                                                                           \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_"                                              \
            "PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                                                       \
        0xf7c7, PERF_TYPE_RAW,                                                                                                              \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_"                                                                 \
            "PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_"                                                              \
            "SINGLE"                                                                                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_SINGLE \
    {                                                                                                                    \
        0xb7c7, PERF_TYPE_RAW,                                                                                           \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_"                                              \
            "PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE \
    {                                                                                                 \
        0x57c7, PERF_TYPE_RAW,                                                                        \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_"                           \
            "PACKED_DOUBLE_512B_PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                                    \
        0xd7c7, PERF_TYPE_RAW,                                                                                           \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_"                                              \
            "PACKED_DOUBLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                 \
        0x97c7, PERF_TYPE_RAW,                                                                        \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_"                           \
            "PACKED_DOUBLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_PACKED_SINGLE \
    {                                                                              \
        0x27c7, PERF_TYPE_RAW,                                                     \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_"        \
            "PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE \
    {                                                                                                 \
        0x67c7, PERF_TYPE_RAW,                                                                        \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_"                           \
            "PACKED_SINGLE_512B_PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                                    \
        0xe7c7, PERF_TYPE_RAW,                                                                                           \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_"                                              \
            "PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_SINGLE \
    {                                                                                                 \
        0xa7c7, PERF_TYPE_RAW,                                                                        \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_"                           \
            "PACKED_SINGLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_512B_PACKED_DOUBLE \
    {                                                                              \
        0x47c7, PERF_TYPE_RAW,                                                     \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_512B_"        \
            "PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                 \
        0xc7c7, PERF_TYPE_RAW,                                                                        \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_512B_"                           \
            "PACKED_DOUBLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                              \
        0x87c7, PERF_TYPE_RAW,                                                     \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_DOUBLE_512B_"        \
            "PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_SINGLE                \
    {                                                                          \
        0x0bc7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_SINGLE"          \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE \
    {                                                                              \
        0x1bc7, PERF_TYPE_RAW,                                                     \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_"        \
            "PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE \
    {                                                                                                 \
        0x3bc7, PERF_TYPE_RAW,                                                                        \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_"                           \
            "PACKED_DOUBLE_256B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE \
    {                                                                                                                    \
        0x7bc7, PERF_TYPE_RAW,                                                                                           \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_"                                              \
            "PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                                                       \
        0xfbc7, PERF_TYPE_RAW,                                                                                                              \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_"                                                                 \
            "PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_"                                                              \
            "SINGLE"                                                                                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_SINGLE \
    {                                                                                                                    \
        0xbbc7, PERF_TYPE_RAW,                                                                                           \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_"                                              \
            "PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE \
    {                                                                                                 \
        0x5bc7, PERF_TYPE_RAW,                                                                        \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_"                           \
            "PACKED_DOUBLE_512B_PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                                    \
        0xdbc7, PERF_TYPE_RAW,                                                                                           \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_"                                              \
            "PACKED_DOUBLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                 \
        0x9bc7, PERF_TYPE_RAW,                                                                        \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_"                           \
            "PACKED_DOUBLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_PACKED_SINGLE \
    {                                                                              \
        0x2bc7, PERF_TYPE_RAW,                                                     \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_"        \
            "PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE \
    {                                                                                                 \
        0x6bc7, PERF_TYPE_RAW,                                                                        \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_"                           \
            "PACKED_SINGLE_512B_PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                                    \
        0xebc7, PERF_TYPE_RAW,                                                                                           \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_"                                              \
            "PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_PACKED_SINGLE_512B_PACKED_SINGLE \
    {                                                                                                 \
        0xabc7, PERF_TYPE_RAW,                                                                        \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_"                           \
            "PACKED_SINGLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_SINGLE_512B_PACKED_DOUBLE \
    {                                                                              \
        0x4bc7, PERF_TYPE_RAW,                                                     \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_SINGLE_512B_"        \
            "PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                 \
        0xcbc7, PERF_TYPE_RAW,                                                                        \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_SINGLE_512B_"                           \
            "PACKED_DOUBLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_SINGLE_512B_PACKED_SINGLE \
    {                                                                              \
        0x8bc7, PERF_TYPE_RAW,                                                     \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_128B_PACKED_SINGLE_512B_"        \
            "PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_256B_PACKED_DOUBLE                \
    {                                                                          \
        0x13c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_256B_PACKED_DOUBLE"          \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE \
    {                                                                              \
        0x33c7, PERF_TYPE_RAW,                                                     \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_256B_PACKED_DOUBLE_256B_"        \
            "PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE \
    {                                                                                                 \
        0x73c7, PERF_TYPE_RAW,                                                                        \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_256B_PACKED_DOUBLE_256B_"                           \
            "PACKED_SINGLE_512B_PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                                    \
        0xf3c7, PERF_TYPE_RAW,                                                                                           \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_256B_PACKED_DOUBLE_256B_"                                              \
            "PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_SINGLE \
    {                                                                                                 \
        0xb3c7, PERF_TYPE_RAW,                                                                        \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_256B_PACKED_DOUBLE_256B_"                           \
            "PACKED_SINGLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE \
    {                                                                              \
        0x53c7, PERF_TYPE_RAW,                                                     \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_256B_PACKED_DOUBLE_512B_"        \
            "PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                 \
        0xd3c7, PERF_TYPE_RAW,                                                                        \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_256B_PACKED_DOUBLE_512B_"                           \
            "PACKED_DOUBLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_256B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                              \
        0x93c7, PERF_TYPE_RAW,                                                     \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_256B_PACKED_DOUBLE_512B_"        \
            "PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_256B_PACKED_SINGLE                \
    {                                                                          \
        0x23c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_256B_PACKED_SINGLE"          \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE \
    {                                                                              \
        0x63c7, PERF_TYPE_RAW,                                                     \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_256B_PACKED_SINGLE_512B_"        \
            "PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                 \
        0xe3c7, PERF_TYPE_RAW,                                                                        \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_256B_PACKED_SINGLE_512B_"                           \
            "PACKED_DOUBLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_256B_PACKED_SINGLE_512B_PACKED_SINGLE \
    {                                                                              \
        0xa3c7, PERF_TYPE_RAW,                                                     \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_256B_PACKED_SINGLE_512B_"        \
            "PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_512B_PACKED_DOUBLE                \
    {                                                                          \
        0x43c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_512B_PACKED_DOUBLE"          \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                              \
        0xc3c7, PERF_TYPE_RAW,                                                     \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_512B_PACKED_DOUBLE_512B_"        \
            "PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_512B_PACKED_SINGLE                \
    {                                                                          \
        0x83c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_SCALAR_DOUBLE_SCALAR_SINGLE_512B_PACKED_SINGLE"          \
    }
#define FP_ARITH_SCALAR_SINGLE                                                 \
    { 0x02c7, PERF_TYPE_RAW, "FP_ARITH_SCALAR_SINGLE" }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE                              \
    { 0x06c7, PERF_TYPE_RAW, "FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE" }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE           \
    {                                                                          \
        0x0ec7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE"     \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE \
    {                                                                                   \
        0x1ec7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_"             \
            "256B_PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE \
    {                                                                                                      \
        0x3ec7, PERF_TYPE_RAW,                                                                             \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_"                                \
            "256B_PACKED_DOUBLE_256B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE \
    {                                                                                                                         \
        0x7ec7, PERF_TYPE_RAW,                                                                                                \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_"                                                   \
            "256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                                                            \
        0xfec7, PERF_TYPE_RAW,                                                                                                                   \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_"                                                                      \
            "256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_"                                                                     \
            "PACKED_SINGLE"                                                                                                                      \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_SINGLE \
    {                                                                                                                         \
        0xbec7, PERF_TYPE_RAW,                                                                                                \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_"                                                   \
            "256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE \
    {                                                                                                      \
        0x5ec7, PERF_TYPE_RAW,                                                                             \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_"                                \
            "256B_PACKED_DOUBLE_512B_PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                                         \
        0xdec7, PERF_TYPE_RAW,                                                                                                \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_"                                                   \
            "256B_PACKED_DOUBLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                      \
        0x9ec7, PERF_TYPE_RAW,                                                                             \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_"                                \
            "256B_PACKED_DOUBLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_SINGLE \
    {                                                                                   \
        0x2ec7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_"             \
            "256B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE \
    {                                                                                                      \
        0x6ec7, PERF_TYPE_RAW,                                                                             \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_"                                \
            "256B_PACKED_SINGLE_512B_PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                                         \
        0xeec7, PERF_TYPE_RAW,                                                                                                \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_"                                                   \
            "256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_256B_PACKED_SINGLE_512B_PACKED_SINGLE \
    {                                                                                                      \
        0xaec7, PERF_TYPE_RAW,                                                                             \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_"                                \
            "256B_PACKED_SINGLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_512B_PACKED_DOUBLE \
    {                                                                                   \
        0x4ec7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_"             \
            "512B_PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                      \
        0xcec7, PERF_TYPE_RAW,                                                                             \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_"                                \
            "512B_PACKED_DOUBLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_512B_PACKED_SINGLE \
    {                                                                                   \
        0x8ec7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_128B_PACKED_SINGLE_"             \
            "512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE           \
    {                                                                          \
        0x16c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE"     \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE \
    {                                                                                   \
        0x36c7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_"             \
            "256B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE \
    {                                                                                                      \
        0x76c7, PERF_TYPE_RAW,                                                                             \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_"                                \
            "256B_PACKED_SINGLE_512B_PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                                         \
        0xf6c7, PERF_TYPE_RAW,                                                                                                \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_"                                                   \
            "256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_SINGLE \
    {                                                                                                      \
        0xb6c7, PERF_TYPE_RAW,                                                                             \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_"                                \
            "256B_PACKED_SINGLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE \
    {                                                                                   \
        0x56c7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_"             \
            "512B_PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                      \
        0xd6c7, PERF_TYPE_RAW,                                                                             \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_"                                \
            "512B_PACKED_DOUBLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                   \
        0x96c7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_PACKED_DOUBLE_"             \
            "512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_PACKED_SINGLE           \
    {                                                                          \
        0x26c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_PACKED_SINGLE"     \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE \
    {                                                                                   \
        0x66c7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_PACKED_SINGLE_"             \
            "512B_PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                      \
        0xe6c7, PERF_TYPE_RAW,                                                                             \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_PACKED_SINGLE_"                                \
            "512B_PACKED_DOUBLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_SINGLE \
    {                                                                                   \
        0xa6c7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_256B_PACKED_SINGLE_"             \
            "512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_512B_PACKED_DOUBLE           \
    {                                                                          \
        0x46c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_512B_PACKED_DOUBLE"     \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                   \
        0xc6c7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_512B_PACKED_DOUBLE_"             \
            "512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_512B_PACKED_SINGLE           \
    {                                                                          \
        0x86c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_DOUBLE_512B_PACKED_SINGLE"     \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_SINGLE                              \
    { 0x0ac7, PERF_TYPE_RAW, "FP_ARITH_SCALAR_SINGLE_128B_PACKED_SINGLE" }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE           \
    {                                                                          \
        0x1ac7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE"     \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE \
    {                                                                                   \
        0x3ac7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_"             \
            "256B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE \
    {                                                                                                      \
        0x7ac7, PERF_TYPE_RAW,                                                                             \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_"                                \
            "256B_PACKED_SINGLE_512B_PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                                         \
        0xfac7, PERF_TYPE_RAW,                                                                                                \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_"                                                   \
            "256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_SINGLE \
    {                                                                                                      \
        0xbac7, PERF_TYPE_RAW,                                                                             \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_"                                \
            "256B_PACKED_SINGLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE \
    {                                                                                   \
        0x5ac7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_"             \
            "512B_PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                      \
        0xdac7, PERF_TYPE_RAW,                                                                             \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_"                                \
            "512B_PACKED_DOUBLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                   \
        0x9ac7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_PACKED_DOUBLE_"             \
            "512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_PACKED_SINGLE           \
    {                                                                          \
        0x2ac7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_PACKED_SINGLE"     \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE \
    {                                                                                   \
        0x6ac7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_PACKED_SINGLE_"             \
            "512B_PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                      \
        0xeac7, PERF_TYPE_RAW,                                                                             \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_PACKED_SINGLE_"                                \
            "512B_PACKED_DOUBLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_PACKED_SINGLE_512B_PACKED_SINGLE \
    {                                                                                   \
        0xaac7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_SINGLE_256B_PACKED_SINGLE_"             \
            "512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_SINGLE_512B_PACKED_DOUBLE           \
    {                                                                          \
        0x4ac7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_SINGLE_512B_PACKED_DOUBLE"     \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                   \
        0xcac7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_SINGLE_512B_PACKED_DOUBLE_"             \
            "512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_128B_PACKED_SINGLE_512B_PACKED_SINGLE           \
    {                                                                          \
        0x8ac7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_SCALAR_SINGLE_128B_PACKED_SINGLE_512B_PACKED_SINGLE"     \
    }
#define FP_ARITH_SCALAR_SINGLE_256B_PACKED_DOUBLE                              \
    { 0x12c7, PERF_TYPE_RAW, "FP_ARITH_SCALAR_SINGLE_256B_PACKED_DOUBLE" }
#define FP_ARITH_SCALAR_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE           \
    {                                                                          \
        0x32c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_SCALAR_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE"     \
    }
#define FP_ARITH_SCALAR_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE \
    {                                                                                   \
        0x72c7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_"             \
            "512B_PACKED_DOUBLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                                      \
        0xf2c7, PERF_TYPE_RAW,                                                                             \
            "FP_ARITH_SCALAR_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_"                                \
            "512B_PACKED_DOUBLE_512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_512B_PACKED_SINGLE \
    {                                                                                   \
        0xb2c7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_SINGLE_256B_PACKED_DOUBLE_256B_PACKED_SINGLE_"             \
            "512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE           \
    {                                                                          \
        0x52c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_SCALAR_SINGLE_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE"     \
    }
#define FP_ARITH_SCALAR_SINGLE_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                   \
        0xd2c7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_SINGLE_256B_PACKED_DOUBLE_512B_PACKED_DOUBLE_"             \
            "512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_256B_PACKED_DOUBLE_512B_PACKED_SINGLE           \
    {                                                                          \
        0x92c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_SCALAR_SINGLE_256B_PACKED_DOUBLE_512B_PACKED_SINGLE"     \
    }
#define FP_ARITH_SCALAR_SINGLE_256B_PACKED_SINGLE                              \
    { 0x22c7, PERF_TYPE_RAW, "FP_ARITH_SCALAR_SINGLE_256B_PACKED_SINGLE" }
#define FP_ARITH_SCALAR_SINGLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE           \
    {                                                                          \
        0x62c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_SCALAR_SINGLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE"     \
    }
#define FP_ARITH_SCALAR_SINGLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE \
    {                                                                                   \
        0xe2c7, PERF_TYPE_RAW,                                                          \
            "FP_ARITH_SCALAR_SINGLE_256B_PACKED_SINGLE_512B_PACKED_DOUBLE_"             \
            "512B_PACKED_SINGLE"                                                        \
    }
#define FP_ARITH_SCALAR_SINGLE_256B_PACKED_SINGLE_512B_PACKED_SINGLE           \
    {                                                                          \
        0xa2c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_SCALAR_SINGLE_256B_PACKED_SINGLE_512B_PACKED_SINGLE"     \
    }
#define FP_ARITH_SCALAR_SINGLE_512B_PACKED_DOUBLE                              \
    { 0x42c7, PERF_TYPE_RAW, "FP_ARITH_SCALAR_SINGLE_512B_PACKED_DOUBLE" }
#define FP_ARITH_SCALAR_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE           \
    {                                                                          \
        0xc2c7, PERF_TYPE_RAW,                                                 \
            "FP_ARITH_SCALAR_SINGLE_512B_PACKED_DOUBLE_512B_PACKED_SINGLE"     \
    }
#define FP_ARITH_SCALAR_SINGLE_512B_PACKED_SINGLE                              \
    { 0x82c7, PERF_TYPE_RAW, "FP_ARITH_SCALAR_SINGLE_512B_PACKED_SINGLE" }
#define HLE_RETIRED_ABORTED                                                    \
    { 0x04c8, PERF_TYPE_RAW, "HLE_RETIRED_ABORTED" }
#define HLE_RETIRED_ABORTED_EVENTS                                             \
    { 0x80c8, PERF_TYPE_RAW, "HLE_RETIRED_ABORTED_EVENTS" }
#define HLE_RETIRED_ABORTED_MEM                                                \
    { 0x08c8, PERF_TYPE_RAW, "HLE_RETIRED_ABORTED_MEM" }
#define HLE_RETIRED_ABORTED_MEMTYPE                                            \
    { 0x40c8, PERF_TYPE_RAW, "HLE_RETIRED_ABORTED_MEMTYPE" }
#define HLE_RETIRED_ABORTED_TMR                                                \
    { 0x10c8, PERF_TYPE_RAW, "HLE_RETIRED_ABORTED_TMR" }
#define HLE_RETIRED_ABORTED_UNFRIENDLY                                         \
    { 0x20c8, PERF_TYPE_RAW, "HLE_RETIRED_ABORTED_UNFRIENDLY" }
#define HLE_RETIRED_COMMIT                                                     \
    { 0x02c8, PERF_TYPE_RAW, "HLE_RETIRED_COMMIT" }
#define HLE_RETIRED_START                                                      \
    { 0x01c8, PERF_TYPE_RAW, "HLE_RETIRED_START" }
#define HW_INTERRUPTS_RECEIVED                                                 \
    { 0x01cb, PERF_TYPE_RAW, "HW_INTERRUPTS_RECEIVED" }
#define ICACHE_16B_IFDATA_STALL                                                \
    { 0x0480, PERF_TYPE_RAW, "ICACHE_16B_IFDATA_STALL" }
#define ICACHE_64B_IFTAG_HIT                                                   \
    { 0x0183, PERF_TYPE_RAW, "ICACHE_64B_IFTAG_HIT" }
#define ICACHE_64B_IFTAG_MISS                                                  \
    { 0x0283, PERF_TYPE_RAW, "ICACHE_64B_IFTAG_MISS" }
#define ICACHE_64B_IFTAG_STALL                                                 \
    { 0x0483, PERF_TYPE_RAW, "ICACHE_64B_IFTAG_STALL" }
#define IDQ_ALL_MITE_UOPS                                                      \
    { 0x3c79, PERF_TYPE_RAW, "IDQ_ALL_MITE_UOPS" }
#define IDQ_DSB_UOPS                                                           \
    { 0x0879, PERF_TYPE_RAW, "IDQ_DSB_UOPS" }
#define IDQ_MITE_UOPS                                                          \
    { 0x0479, PERF_TYPE_RAW, "IDQ_MITE_UOPS" }
#define IDQ_MS_DSB_UOPS                                                        \
    { 0x1079, PERF_TYPE_RAW, "IDQ_MS_DSB_UOPS" }
#define IDQ_MS_MITE_UOPS                                                       \
    { 0x2079, PERF_TYPE_RAW, "IDQ_MS_MITE_UOPS" }
#define IDQ_MS_UOPS                                                            \
    { 0x3079, PERF_TYPE_RAW, "IDQ_MS_UOPS" }
#define IDQ_UOPS_NOT_DELIVERED_CORE                                            \
    { 0x019c, PERF_TYPE_RAW, "IDQ_UOPS_NOT_DELIVERED_CORE" }
#define ILD_STALL_LCP                                                          \
    { 0x0187, PERF_TYPE_RAW, "ILD_STALL_LCP" }
#define INST_RETIRED_ALL                                                       \
    { 0x01c0, PERF_TYPE_RAW, "INST_RETIRED_ALL" }
#define INST_RETIRED_ANY_P                                                     \
    { 0x00c0, PERF_TYPE_RAW, "INST_RETIRED_ANY_P" }
#define INT_MISC_CLEAR_RESTEER_CYCLES                                          \
    { 0x800d, PERF_TYPE_RAW, "INT_MISC_CLEAR_RESTEER_CYCLES" }
#define INT_MISC_RECOVERY_CYCLES                                               \
    { 0x010d, PERF_TYPE_RAW, "INT_MISC_RECOVERY_CYCLES" }
#define ITLB_ITLB_FLUSH                                                        \
    { 0x01ae, PERF_TYPE_RAW, "ITLB_ITLB_FLUSH" }
#define ITLB_MISSES_MISS_CAUSES_A_WALK                                         \
    { 0x0185, PERF_TYPE_RAW, "ITLB_MISSES_MISS_CAUSES_A_WALK" }
#define ITLB_MISSES_STLB_HIT                                                   \
    { 0x2085, PERF_TYPE_RAW, "ITLB_MISSES_STLB_HIT" }
#define ITLB_MISSES_WALK_COMPLETED                                             \
    { 0x0e85, PERF_TYPE_RAW, "ITLB_MISSES_WALK_COMPLETED" }
#define ITLB_MISSES_WALK_COMPLETED_1G                                          \
    { 0x0885, PERF_TYPE_RAW, "ITLB_MISSES_WALK_COMPLETED_1G" }
#define ITLB_MISSES_WALK_COMPLETED_2M_4M                                       \
    { 0x0485, PERF_TYPE_RAW, "ITLB_MISSES_WALK_COMPLETED_2M_4M" }
#define ITLB_MISSES_WALK_COMPLETED_4K                                          \
    { 0x0285, PERF_TYPE_RAW, "ITLB_MISSES_WALK_COMPLETED_4K" }
#define ITLB_MISSES_WALK_PENDING                                               \
    { 0x1085, PERF_TYPE_RAW, "ITLB_MISSES_WALK_PENDING" }
#define L1D_PEND_MISS_FB_FULL                                                  \
    { 0x0248, PERF_TYPE_RAW, "L1D_PEND_MISS_FB_FULL" }
#define L1D_PEND_MISS_PENDING                                                  \
    { 0x0148, PERF_TYPE_RAW, "L1D_PEND_MISS_PENDING" }
#define L1D_REPLACEMENT                                                        \
    { 0x0151, PERF_TYPE_RAW, "L1D_REPLACEMENT" }
#define L2_LINES_IN_ANY                                                        \
    { 0x1ff1, PERF_TYPE_RAW, "L2_LINES_IN_ANY" }
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
#define L2_RQSTS_ALL_PF                                                        \
    { 0xf824, PERF_TYPE_RAW, "L2_RQSTS_ALL_PF" }
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
#define L2_RQSTS_MISS                                                          \
    { 0x3f24, PERF_TYPE_RAW, "L2_RQSTS_MISS" }
#define L2_RQSTS_PF_HIT                                                        \
    { 0xd824, PERF_TYPE_RAW, "L2_RQSTS_PF_HIT" }
#define L2_RQSTS_PF_MISS                                                       \
    { 0x3824, PERF_TYPE_RAW, "L2_RQSTS_PF_MISS" }
#define L2_RQSTS_REFERENCES                                                    \
    { 0xff24, PERF_TYPE_RAW, "L2_RQSTS_REFERENCES" }
#define L2_RQSTS_RFO_HIT                                                       \
    { 0xc224, PERF_TYPE_RAW, "L2_RQSTS_RFO_HIT" }
#define L2_RQSTS_RFO_MISS                                                      \
    { 0x2224, PERF_TYPE_RAW, "L2_RQSTS_RFO_MISS" }
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
#define LOAD_HIT_PRE_SW_PF                                                     \
    { 0x014c, PERF_TYPE_RAW, "LOAD_HIT_PRE_SW_PF" }
#define LOCK_CYCLES_CACHE_LOCK_DURATION                                        \
    { 0x0263, PERF_TYPE_RAW, "LOCK_CYCLES_CACHE_LOCK_DURATION" }
#define LSD_UOPS                                                               \
    { 0x01a8, PERF_TYPE_RAW, "LSD_UOPS" }
#define MACHINE_CLEARS_MEMORY_ORDERING                                         \
    { 0x02c3, PERF_TYPE_RAW, "MACHINE_CLEARS_MEMORY_ORDERING" }
#define MACHINE_CLEARS_SMC                                                     \
    { 0x04c3, PERF_TYPE_RAW, "MACHINE_CLEARS_SMC" }
#define MEM_LOAD_L3_HIT_RETIRED_XSNP_HIT                                       \
    { 0x02d2, PERF_TYPE_RAW, "MEM_LOAD_L3_HIT_RETIRED_XSNP_HIT" }
#define MEM_LOAD_L3_HIT_RETIRED_XSNP_HITM                                      \
    { 0x04d2, PERF_TYPE_RAW, "MEM_LOAD_L3_HIT_RETIRED_XSNP_HITM" }
#define MEM_LOAD_L3_HIT_RETIRED_XSNP_MISS                                      \
    { 0x01d2, PERF_TYPE_RAW, "MEM_LOAD_L3_HIT_RETIRED_XSNP_MISS" }
#define MEM_LOAD_L3_HIT_RETIRED_XSNP_NONE                                      \
    { 0x08d2, PERF_TYPE_RAW, "MEM_LOAD_L3_HIT_RETIRED_XSNP_NONE" }
#define MEM_LOAD_L3_MISS_RETIRED_LOCAL_DRAM                                    \
    { 0x01d3, PERF_TYPE_RAW, "MEM_LOAD_L3_MISS_RETIRED_LOCAL_DRAM" }
#define MEM_LOAD_MISC_RETIRED_UC                                               \
    { 0x04d4, PERF_TYPE_RAW, "MEM_LOAD_MISC_RETIRED_UC" }
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
#define MEM_UOPS_RETIRED_ALL_LOADS                                             \
    { 0x81d0, PERF_TYPE_RAW, "MEM_UOPS_RETIRED_ALL_LOADS" }
#define MEM_UOPS_RETIRED_ALL_STORES                                            \
    { 0x82d0, PERF_TYPE_RAW, "MEM_UOPS_RETIRED_ALL_STORES" }
#define MEM_UOPS_RETIRED_LOCK_LOADS                                            \
    { 0x21d0, PERF_TYPE_RAW, "MEM_UOPS_RETIRED_LOCK_LOADS" }
#define MEM_UOPS_RETIRED_SPLIT_LOADS                                           \
    { 0x41d0, PERF_TYPE_RAW, "MEM_UOPS_RETIRED_SPLIT_LOADS" }
#define MEM_UOPS_RETIRED_SPLIT_STORES                                          \
    { 0x42d0, PERF_TYPE_RAW, "MEM_UOPS_RETIRED_SPLIT_STORES" }
#define MEM_UOPS_RETIRED_STLB_MISS_LOADS                                       \
    { 0x11d0, PERF_TYPE_RAW, "MEM_UOPS_RETIRED_STLB_MISS_LOADS" }
#define MEM_UOPS_RETIRED_STLB_MISS_STORES                                      \
    { 0x12d0, PERF_TYPE_RAW, "MEM_UOPS_RETIRED_STLB_MISS_STORES" }
#define MISALIGN_MEM_REF_LOADS                                                 \
    { 0x0105, PERF_TYPE_RAW, "MISALIGN_MEM_REF_LOADS" }
#define MISALIGN_MEM_REF_STORES                                                \
    { 0x0205, PERF_TYPE_RAW, "MISALIGN_MEM_REF_STORES" }
#define MISPREDICTED_BRANCH_RETIRED                                            \
    { 0x00c5, PERF_TYPE_RAW, "MISPREDICTED_BRANCH_RETIRED" }
#define MOVE_ELIMINATION_INT_ELIMINATED                                        \
    { 0x0158, PERF_TYPE_RAW, "MOVE_ELIMINATION_INT_ELIMINATED" }
#define MOVE_ELIMINATION_INT_NOT_ELIMINATED                                    \
    { 0x0458, PERF_TYPE_RAW, "MOVE_ELIMINATION_INT_NOT_ELIMINATED" }
#define MOVE_ELIMINATION_SIMD_ELIMINATED                                       \
    { 0x0258, PERF_TYPE_RAW, "MOVE_ELIMINATION_SIMD_ELIMINATED" }
#define MOVE_ELIMINATION_SIMD_NOT_ELIMINATED                                   \
    { 0x0858, PERF_TYPE_RAW, "MOVE_ELIMINATION_SIMD_NOT_ELIMINATED" }
#define OFFCORE_REQUESTS_ALL_DATA_RD                                           \
    { 0x08b0, PERF_TYPE_RAW, "OFFCORE_REQUESTS_ALL_DATA_RD" }
#define OFFCORE_REQUESTS_ALL_REQUESTS                                          \
    { 0x80b0, PERF_TYPE_RAW, "OFFCORE_REQUESTS_ALL_REQUESTS" }
#define OFFCORE_REQUESTS_BUFFER_SQ_FULL                                        \
    { 0x01b2, PERF_TYPE_RAW, "OFFCORE_REQUESTS_BUFFER_SQ_FULL" }
#define OFFCORE_REQUESTS_DEMAND_CODE_RD                                        \
    { 0x02b0, PERF_TYPE_RAW, "OFFCORE_REQUESTS_DEMAND_CODE_RD" }
#define OFFCORE_REQUESTS_DEMAND_DATA_RD                                        \
    { 0x01b0, PERF_TYPE_RAW, "OFFCORE_REQUESTS_DEMAND_DATA_RD" }
#define OFFCORE_REQUESTS_DEMAND_RFO                                            \
    { 0x04b0, PERF_TYPE_RAW, "OFFCORE_REQUESTS_DEMAND_RFO" }
#define OFFCORE_REQUESTS_L3_MISS_DEMAND_DATA_RD                                \
    { 0x10b0, PERF_TYPE_RAW, "OFFCORE_REQUESTS_L3_MISS_DEMAND_DATA_RD" }
#define OFFCORE_REQUESTS_OUTSTANDING_ALL_DATA_RD                               \
    { 0x0860, PERF_TYPE_RAW, "OFFCORE_REQUESTS_OUTSTANDING_ALL_DATA_RD" }
#define OFFCORE_REQUESTS_OUTSTANDING_DEMAND_CODE_RD                            \
    { 0x0260, PERF_TYPE_RAW, "OFFCORE_REQUESTS_OUTSTANDING_DEMAND_CODE_RD" }
#define OFFCORE_REQUESTS_OUTSTANDING_DEMAND_DATA_RD                            \
    { 0x0160, PERF_TYPE_RAW, "OFFCORE_REQUESTS_OUTSTANDING_DEMAND_DATA_RD" }
#define OFFCORE_REQUESTS_OUTSTANDING_DEMAND_RFO                                \
    { 0x0460, PERF_TYPE_RAW, "OFFCORE_REQUESTS_OUTSTANDING_DEMAND_RFO" }
#define OFFCORE_REQUESTS_OUTSTANDING_L3_MISS_DEMAND_DATA_RD                    \
    {                                                                          \
        0x1060, PERF_TYPE_RAW,                                                 \
            "OFFCORE_REQUESTS_OUTSTANDING_L3_MISS_DEMAND_DATA_RD"              \
    }
#define OTHER_ASSISTS_ANY                                                      \
    { 0x3fc1, PERF_TYPE_RAW, "OTHER_ASSISTS_ANY" }
#define PARTIAL_RAT_STALLS_SCOREBOARD                                          \
    { 0x0159, PERF_TYPE_RAW, "PARTIAL_RAT_STALLS_SCOREBOARD" }
#define RESOURCE_STALLS_ALL                                                    \
    { 0x01a2, PERF_TYPE_RAW, "RESOURCE_STALLS_ALL" }
#define RESOURCE_STALLS_ROB                                                    \
    { 0x10a2, PERF_TYPE_RAW, "RESOURCE_STALLS_ROB" }
#define RESOURCE_STALLS_RS                                                     \
    { 0x04a2, PERF_TYPE_RAW, "RESOURCE_STALLS_RS" }
#define RESOURCE_STALLS_SB                                                     \
    { 0x08a2, PERF_TYPE_RAW, "RESOURCE_STALLS_SB" }
#define ROB_MISC_EVENTS_LBR_INSERTS                                            \
    { 0x20cc, PERF_TYPE_RAW, "ROB_MISC_EVENTS_LBR_INSERTS" }
#define ROB_MISC_EVENTS_PAUSE_INST                                             \
    { 0x40cc, PERF_TYPE_RAW, "ROB_MISC_EVENTS_PAUSE_INST" }
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
#define RTM_RETIRED_ABORTED_TMR                                                \
    { 0x10c9, PERF_TYPE_RAW, "RTM_RETIRED_ABORTED_TMR" }
#define RTM_RETIRED_ABORTED_UNFRIENDLY                                         \
    { 0x20c9, PERF_TYPE_RAW, "RTM_RETIRED_ABORTED_UNFRIENDLY" }
#define RTM_RETIRED_COMMIT                                                     \
    { 0x02c9, PERF_TYPE_RAW, "RTM_RETIRED_COMMIT" }
#define RTM_RETIRED_START                                                      \
    { 0x01c9, PERF_TYPE_RAW, "RTM_RETIRED_START" }
#define SQ_MISC_SPLIT_LOCK                                                     \
    { 0x10f4, PERF_TYPE_RAW, "SQ_MISC_SPLIT_LOCK" }
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
#define TX_EXEC_MISC1                                                          \
    { 0x015d, PERF_TYPE_RAW, "TX_EXEC_MISC1" }
#define TX_EXEC_MISC2                                                          \
    { 0x025d, PERF_TYPE_RAW, "TX_EXEC_MISC2" }
#define TX_EXEC_MISC3                                                          \
    { 0x045d, PERF_TYPE_RAW, "TX_EXEC_MISC3" }
#define TX_EXEC_MISC4                                                          \
    { 0x085d, PERF_TYPE_RAW, "TX_EXEC_MISC4" }
#define TX_EXEC_MISC5                                                          \
    { 0x105d, PERF_TYPE_RAW, "TX_EXEC_MISC5" }
#define TX_MEM_ABORT_CAPACITY                                                  \
    { 0x0254, PERF_TYPE_RAW, "TX_MEM_ABORT_CAPACITY" }
#define TX_MEM_ABORT_CONFLICT                                                  \
    { 0x0154, PERF_TYPE_RAW, "TX_MEM_ABORT_CONFLICT" }
#define TX_MEM_ABORT_HLE_ELISION_BUFFER_FULL                                   \
    { 0x4054, PERF_TYPE_RAW, "TX_MEM_ABORT_HLE_ELISION_BUFFER_FULL" }
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
#define UNHALTED_REFERENCE_CYCLES                                              \
    { 0x013c, PERF_TYPE_RAW, "UNHALTED_REFERENCE_CYCLES" }
#define UNHALTED_REFERENCE_CYCLES                                              \
    { 0x0300, PERF_TYPE_RAW, "UNHALTED_REFERENCE_CYCLES" }
#define UOPS_DISPATCHED_PORT_0                                                 \
    { 0x01a1, PERF_TYPE_RAW, "UOPS_DISPATCHED_PORT_0" }
#define UOPS_DISPATCHED_PORT_1                                                 \
    { 0x02a1, PERF_TYPE_RAW, "UOPS_DISPATCHED_PORT_1" }
#define UOPS_DISPATCHED_PORT_2                                                 \
    { 0x04a1, PERF_TYPE_RAW, "UOPS_DISPATCHED_PORT_2" }
#define UOPS_DISPATCHED_PORT_3                                                 \
    { 0x08a1, PERF_TYPE_RAW, "UOPS_DISPATCHED_PORT_3" }
#define UOPS_DISPATCHED_PORT_4                                                 \
    { 0x10a1, PERF_TYPE_RAW, "UOPS_DISPATCHED_PORT_4" }
#define UOPS_DISPATCHED_PORT_5                                                 \
    { 0x20a1, PERF_TYPE_RAW, "UOPS_DISPATCHED_PORT_5" }
#define UOPS_DISPATCHED_PORT_6                                                 \
    { 0x40a1, PERF_TYPE_RAW, "UOPS_DISPATCHED_PORT_6" }
#define UOPS_DISPATCHED_PORT_7                                                 \
    { 0x80a1, PERF_TYPE_RAW, "UOPS_DISPATCHED_PORT_7" }
#define UOPS_EXECUTED_CORE                                                     \
    { 0x02b1, PERF_TYPE_RAW, "UOPS_EXECUTED_CORE" }
#define UOPS_EXECUTED_THREAD                                                   \
    { 0x01b1, PERF_TYPE_RAW, "UOPS_EXECUTED_THREAD" }
#define UOPS_EXECUTED_X87                                                      \
    { 0x10b1, PERF_TYPE_RAW, "UOPS_EXECUTED_X87" }
#define UOPS_ISSUED_ALL                                                        \
    { 0x010e, PERF_TYPE_RAW, "UOPS_ISSUED_ALL" }
#define UOPS_ISSUED_FLAGS_MERGE                                                \
    { 0x100e, PERF_TYPE_RAW, "UOPS_ISSUED_FLAGS_MERGE" }
#define UOPS_ISSUED_SINGLE_MUL                                                 \
    { 0x400e, PERF_TYPE_RAW, "UOPS_ISSUED_SINGLE_MUL" }
#define UOPS_ISSUED_SLOW_LEA                                                   \
    { 0x200e, PERF_TYPE_RAW, "UOPS_ISSUED_SLOW_LEA" }
#define UOPS_ISSUED_VECTOR_WIDTH_MISMATCH                                      \
    { 0x020e, PERF_TYPE_RAW, "UOPS_ISSUED_VECTOR_WIDTH_MISMATCH" }
#define UOPS_RETIRED_ANY                                                       \
    { 0x01c2, PERF_TYPE_RAW, "UOPS_RETIRED_ANY" }
#define UOPS_RETIRED_RETIRE_SLOTS                                              \
    { 0x02c2, PERF_TYPE_RAW, "UOPS_RETIRED_RETIRE_SLOTS" }

#endif
