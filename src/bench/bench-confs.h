
#ifndef _BENCH_CONFS_H_
#define _BENCH_CONFS_H_

#include <common.h>

typedef struct bench_conf {
    uint64_t al_dst : 16;
    uint64_t al_src : 16;
    uint64_t direction : 1;
    uint32_t sz : 31;
} bench_conf_t;


bench_conf_t * make_rand_confs();
bench_conf_t * make_small_confs(uint32_t dst_al_offset, uint32_t * nconfs_out);
bench_conf_t * make_medium_confs(uint32_t dst_al_offset, uint32_t * nconfs_out);
bench_conf_t * make_large_confs(uint32_t dst_al_offset, uint32_t * nconfs_out);


#endif
