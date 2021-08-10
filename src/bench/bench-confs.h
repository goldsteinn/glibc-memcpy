
#ifndef _BENCH_CONFS_H_
#define _BENCH_CONFS_H_

#include <common.h>

typedef struct bench_conf {
    uint64_t al_dst : 16;
    uint64_t al_src : 16;
    uint64_t direction : 1;
    uint32_t sz : 31;
} bench_conf_t;

typedef enum bench_direction {
    FORWARD       = 0,
    BACKWARD      = 1,
    BIDIRECTIONAL = 2
} bench_direction_t;


bench_conf_t * make_rand_confs(uint32_t          min_val,
                               uint32_t          max_val,
                               uint32_t          size_scale,
                               bench_direction_t direction,
                               uint32_t          nrand_confs);
bench_conf_t * make_small_confs(uint32_t dst_al_offset, uint32_t * nconfs_out);
bench_conf_t * make_medium_confs(uint32_t dst_al_offset, uint32_t * nconfs_out);
bench_conf_t * make_large_confs(uint32_t * nconfs_out);


#endif
