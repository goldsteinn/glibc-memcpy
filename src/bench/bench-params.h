#ifndef _BENCH_CONFS_H_
#define _BENCH_CONFS_H_

#include <common.h>
#include "bench-common.h"

static const uint32_t nrand_confs    = 2 * 8192;
static const uint64_t MAX_BENCH_SIZE = (1UL << 26);

bench_conf_t * make_rand_confs();
bench_conf_t * make_small_confs(uint32_t dst_al_offset, uint32_t * nconfs_out);
bench_conf_t * make_medium_confs(uint32_t dst_al_offset, uint32_t * nconfs_out);
bench_conf_t * make_large_confs(uint32_t dst_al_offset, uint32_t * nconfs_out);


#endif
