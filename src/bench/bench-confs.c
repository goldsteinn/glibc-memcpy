#include "bench-confs.h"
#include <error-util.h>
#include <memory-util.h>
#include "bench-constants.h"

typedef struct freq_data {
    uint32_t freq;
    uint32_t val;
} freq_data_t;

#define make_conf(_conf, _al_dst, _al_src, _direction, _sz)                    \
    {                                                                          \
        (_conf).al_dst    = (_al_dst);                                         \
        (_conf).al_src    = (_al_src);                                         \
        (_conf).direction = (_direction);                                      \
        (_conf).sz        = (_sz);                                             \
    }

#define make_conf_check(_conf, _al_dst, _al_src, _direction, _sz, _cur_sz,     \
                        _cur_cap, _confs)                                      \
    make_conf(_conf, _al_dst, _al_src, _direction, _sz);                       \
    {                                                                          \
        if (++(_cur_sz) >= (_cur_cap)) {                                       \
            (_confs) = (bench_conf_t *)safe_realloc(                           \
                (_confs), sizeof(bench_conf_t) * 2 * (_cur_cap));              \
            (_cur_cap) = 2 * (_cur_cap);                                       \
        }                                                                      \
    }


/* Frequency data for memcpy of less than 4096 bytes based on SPEC2017.  */
static const freq_data_t size_freq[] = {
    { 22320, 32 }, { 9554, 16 },  { 8915, 8 },  { 5327, 152 }, { 2159, 4 },
    { 2035, 292 }, { 1608, 12 },  { 1343, 24 }, { 895, 1152 }, { 813, 144 },
    { 733, 884 },  { 721, 284 },  { 661, 120 }, { 649, 2 },    { 550, 882 },
    { 475, 5 },    { 461, 7 },    { 460, 108 }, { 361, 10 },   { 361, 9 },
    { 334, 6 },    { 326, 3 },    { 308, 464 }, { 303, 2048 }, { 298, 1 },
    { 250, 64 },   { 197, 11 },   { 194, 296 }, { 187, 68 },   { 185, 15 },
    { 184, 192 },  { 183, 1764 }, { 173, 13 },  { 126, 560 },  { 115, 160 },
    { 96, 288 },   { 96, 104 },   { 83, 1144 }, { 80, 18 },    { 78, 23 },
    { 77, 40 },    { 68, 19 },    { 63, 48 },   { 57, 17 },    { 54, 72 },
    { 51, 1280 },  { 49, 20 },    { 47, 28 },   { 46, 22 },    { 45, 640 },
    { 41, 25 },    { 40, 14 },    { 37, 56 },   { 35, 27 },    { 33, 35 },
    { 33, 384 },   { 32, 29 },    { 30, 80 },   { 22, 4095 },  { 22, 232 },
    { 19, 36 },    { 17, 184 },   { 17, 21 },   { 16, 256 },   { 15, 44 },
    { 15, 26 },    { 14, 31 },    { 14, 88 },   { 13, 176 },   { 12, 33 },
    { 12, 1024 },  { 11, 208 },   { 11, 62 },   { 10, 128 },   { 10, 704 },
    { 10, 324 },   { 10, 96 },    { 9, 60 },    { 9, 136 },    { 9, 124 },
    { 8, 34 },     { 8, 30 },     { 8, 480 },   { 8, 1344 },   { 7, 273 },
    { 7, 520 },    { 6, 112 },    { 6, 52 },    { 6, 344 },    { 6, 336 },
    { 5, 504 },    { 5, 168 },    { 5, 424 },   { 4, 0 },      { 3, 76 },
    { 3, 200 },    { 3, 512 },    { 3, 312 },   { 3, 240 },    { 3, 960 },
    { 2, 264 },    { 2, 672 },    { 2, 38 },    { 2, 328 },    { 2, 84 },
    { 2, 39 },     { 2, 216 },    { 2, 42 },    { 2, 37 },     { 2, 1608 },
    { 2, 70 },     { 2, 46 },     { 2, 536 },   { 1, 280 },    { 1, 248 },
    { 1, 47 },     { 1, 1088 },   { 1, 1288 },  { 1, 224 },    { 1, 41 },
    { 1, 50 },     { 1, 49 },     { 1, 808 },   { 1, 360 },    { 1, 440 },
    { 1, 43 },     { 1, 45 },     { 1, 78 },    { 1, 968 },    { 1, 392 },
    { 1, 54 },     { 1, 53 },     { 1, 59 },    { 1, 376 },    { 1, 664 },
    { 1, 58 },     { 1, 272 },    { 1, 66 },    { 1, 2688 },   { 1, 472 },
    { 1, 568 },    { 1, 720 },    { 1, 51 },    { 1, 63 },     { 1, 86 },
    { 1, 496 },    { 1, 776 },    { 1, 57 },    { 1, 680 },    { 1, 792 },
    { 1, 122 },    { 1, 760 },    { 1, 824 },   { 1, 552 },    { 1, 67 },
    { 1, 456 },    { 1, 984 },    { 1, 74 },    { 1, 408 },    { 1, 75 },
    { 1, 92 },     { 1, 576 },    { 1, 116 },   { 1, 65 },     { 1, 117 },
    { 1, 82 },     { 1, 352 },    { 1, 55 },    { 1, 100 },    { 1, 90 },
    { 1, 696 },    { 1, 111 },    { 1, 880 },   { 1, 79 },     { 1, 488 },
    { 1, 61 },     { 1, 114 },    { 1, 94 },    { 1, 1032 },   { 1, 98 },
    { 1, 87 },     { 1, 584 },    { 1, 85 },    { 1, 648 },    { 0, 0 }
};


static const freq_data_t src_align_freq[] = { { 300, 8 },  { 292, 16 },
                                              { 168, 32 }, { 153, 64 },
                                              { 79, 4 },   { 14, 2 },
                                              { 18, 1 },   { 0, 0 } };

static const freq_data_t dst_align_freq[] = { { 265, 8 },  { 263, 16 },
                                              { 209, 64 }, { 174, 32 },
                                              { 90, 4 },   { 10, 2 },
                                              { 13, 1 },   { 0, 0 } };
static const uint64_t    n_align_freq =
    sizeof(src_align_freq) / sizeof(src_align_freq[0]);
// Scale so that alignments are equally distributed throughout page.
static const uint64_t n_scaled_align_freq = n_align_freq * (PAGE_SIZE / 64);

static const uint64_t n_size_freq = sizeof(size_freq) / sizeof(size_freq[0]);


static freq_data_t *
scale_align_freq(const freq_data_t * align_freq) {
    freq_data_t * scaled_align_freq =
        (freq_data_t *)safe_calloc(n_scaled_align_freq, sizeof(freq_data_t));

    uint64_t n = 0;
    for (uint32_t i = 0; i < PAGE_SIZE; i += 64) {
        for (uint32_t j = 0; j < n_align_freq; ++j) {
            scaled_align_freq[n].freq = align_freq[j].freq;
            scaled_align_freq[n].val  = align_freq[j].val + i;
            ++n;
        }
    }
    return scaled_align_freq;
}

static uint32_t *
_make_data_dist(const freq_data_t * freq_in,
                uint64_t            freq_in_sz,
                uint32_t *          dist_sz,
                uint32_t            min_val,
                uint32_t            max_val,
                uint32_t            scale) {
    uint64_t n = 0;
    for (uint64_t i = 0; i < freq_in_sz; ++i) {
        if (freq_in[i].val * scale < min_val ||
            freq_in[i].val * scale > max_val) {
            continue;
        }
        n += freq_in[i].freq;
    }
    uint32_t * freq_out = (uint32_t *)safe_calloc(n, sizeof(uint32_t));
    *dist_sz            = n;
    n                   = 0;
    for (uint64_t i = 0; i < freq_in_sz; ++i) {
        if (freq_in[i].val * scale < min_val ||
            freq_in[i].val * scale > max_val) {
            continue;
        }
        for (uint64_t j = 0; j < freq_in[i].freq; ++j) {
            freq_out[n] = (freq_in[i].val) * scale;
            ++n;
        }
    }
    die_assert(n == (*dist_sz));
    return freq_out;
}

static uint32_t *
make_data_dist(const freq_data_t * freq_in,
               uint64_t            freq_in_sz,
               uint32_t *          dist_sz) {
    return _make_data_dist(freq_in, freq_in_sz, dist_sz, 0, UINT32_MAX, 1);
}

bench_conf_t *
make_rand_confs(uint32_t          min_val,
                uint32_t          max_val,
                uint32_t          size_scale,
                bench_direction_t direction,
                uint32_t          nrand_confs) {
    die_assert(min_val <= max_val);
    freq_data_t * scaled_src_align_freq = scale_align_freq(src_align_freq);
    freq_data_t * scaled_dst_align_freq = scale_align_freq(dst_align_freq);

    uint32_t align_dist_sz = 0;
    uint32_t size_dist_sz  = 0;

    uint32_t * src_align_dist = make_data_dist(
        scaled_src_align_freq, n_scaled_align_freq, &align_dist_sz);
    uint32_t * dst_align_dist = make_data_dist(
        scaled_dst_align_freq, n_scaled_align_freq, &align_dist_sz);
    uint32_t * size_dist = _make_data_dist(
        size_freq, n_size_freq, &size_dist_sz, min_val, max_val, size_scale);


    bench_conf_t * confs =
        (bench_conf_t *)safe_calloc(nrand_confs, sizeof(bench_conf_t));
    die_assert(nrand_confs <= size_dist_sz, "Not enough confs!");

    for (uint64_t i = 0; i < nrand_confs; ++i) {
        int dir;
        if (direction == BIDIRECTIONAL) {
            dir = rand() % 2;
        }
        else {
            dir = !direction;
        }
        uint32_t dst_idx = rand() % align_dist_sz;
        uint32_t al_dst =
            dst_align_dist[dst_idx] + (dir ? 0 * PAGE_SIZE : 8 * PAGE_SIZE);
        uint32_t al_src =
            src_align_dist[rand() % align_dist_sz] + 4 * PAGE_SIZE;
        uint32_t sz =
            size_dist[(nrand_confs == size_dist_sz) ? i
                                                    : (rand() % size_dist_sz)];

        make_conf(confs[i], al_dst, al_src, dir, sz);
    }

    if (nrand_confs == size_dist_sz) {
        for (uint64_t i = 0; i < 2 * nrand_confs; ++i) {
            uint32_t     idx0 = rand() % nrand_confs;
            uint32_t     idx1 = rand() % nrand_confs;
            bench_conf_t tmp;
            memcpy(&tmp, confs + idx0, sizeof(bench_conf_t));
            memcpy(confs + idx0, confs + idx1, sizeof(bench_conf_t));
            memcpy(confs + idx1, &tmp, sizeof(bench_conf_t));
        }
    }


    safe_free(scaled_src_align_freq);
    safe_free(scaled_dst_align_freq);
    safe_free(src_align_dist);
    safe_free(dst_align_dist);
    safe_free(size_dist);

    return confs;
}


bench_conf_t *
make_small_confs(uint32_t dst_al_offset, uint32_t * nconfs_out) {
    uint32_t       cur_sz  = 0;
    uint32_t       cur_cap = 128 * 64;
    bench_conf_t * confs =
        (bench_conf_t *)safe_calloc(cur_cap, sizeof(bench_conf_t));
    PRINTFFL;
    for (uint32_t i = 1; i < 128; ++i) {
        for (uint32_t j = 0; j <= 64; j += 32) {
            make_conf_check(confs[cur_sz], dst_al_offset + 0, j, 0, i, cur_sz,
                            cur_cap, confs);
            make_conf_check(confs[cur_sz], dst_al_offset + j, 0, 0, i, cur_sz,
                            cur_cap, confs);
            make_conf_check(confs[cur_sz], dst_al_offset + 1, j, 0, i, cur_sz,
                            cur_cap, confs);
            make_conf_check(confs[cur_sz], dst_al_offset + j, 1, 0, i, cur_sz,
                            cur_cap, confs);
            make_conf_check(confs[cur_sz], dst_al_offset + j, j, 0, i, cur_sz,
                            cur_cap, confs);
            make_conf_check(confs[cur_sz], dst_al_offset + j + 1, j, 0, i,
                            cur_sz, cur_cap, confs);
            make_conf_check(confs[cur_sz], dst_al_offset + 0, j, 0, i, cur_sz,
                            cur_cap, confs);

            make_conf_check(confs[cur_sz], dst_al_offset + j, 31, 0, i, cur_sz,
                            cur_cap, confs);
            make_conf_check(confs[cur_sz], dst_al_offset + 31, j, 0, i, cur_sz,
                            cur_cap, confs);
            make_conf_check(confs[cur_sz], dst_al_offset + j, 31, 0, i, cur_sz,
                            cur_cap, confs);
            make_conf_check(confs[cur_sz], dst_al_offset + j, j + 31, 0, i,
                            cur_sz, cur_cap, confs);
            make_conf_check(confs[cur_sz], dst_al_offset + j + 31, j + 31, 0, i,
                            cur_sz, cur_cap, confs);
        }
    }
    *nconfs_out = cur_sz;
    return confs;
}

bench_conf_t *
make_custom_confs(uint32_t * nconfs_out,
                  uint32_t   al_dst,
                  uint32_t   al_src,
                  uint32_t   direction,
                  uint32_t   sz) {
    bench_conf_t * confs = (bench_conf_t *)safe_calloc(2, sizeof(bench_conf_t));
    make_conf(confs[0], al_dst, al_src, !!direction, sz);
    uint32_t n = 1;
    if (direction == BIDIRECTIONAL) {
        make_conf(confs[1], al_dst, al_src, 0, sz);
        n = 2;
    }
    *nconfs_out = n;
    return confs;
}

bench_conf_t *
make_summary_confs(uint32_t * nconfs_out) {
    uint32_t       cur_sz  = 0;
    uint32_t       cur_cap = 128 * 64;
    bench_conf_t * confs =
        (bench_conf_t *)safe_calloc(cur_cap, sizeof(bench_conf_t));
    for (uint32_t d = 0; d < 2; ++d) {
        for (uint32_t i = 0; i <= 64; i <<= 1) {
            make_conf_check(confs[cur_sz], 0, 0, d, i, cur_sz, cur_cap, confs);
            make_conf_check(confs[cur_sz], 1024, 0, d, i, cur_sz, cur_cap,
                            confs);
        }

        for (uint32_t i = 80; i <= 256; i += 16) {
            make_conf_check(confs[cur_sz], 0, 0, d, i, cur_sz, cur_cap, confs);
            make_conf_check(confs[cur_sz], 1, 0, d, i, cur_sz, cur_cap, confs);
            make_conf_check(confs[cur_sz], 0, 1, d, i, cur_sz, cur_cap, confs);
            make_conf_check(confs[cur_sz], 0, 63, d, i, cur_sz, cur_cap, confs);
            make_conf_check(confs[cur_sz], 63, 0, d, i, cur_sz, cur_cap, confs);

            make_conf_check(confs[cur_sz], 1024, 0, d, i, cur_sz, cur_cap,
                            confs);
            make_conf_check(confs[cur_sz], 1024 + 1, 0, d, i, cur_sz, cur_cap,
                            confs);
            make_conf_check(confs[cur_sz], 1024, 1, d, i, cur_sz, cur_cap,
                            confs);
            make_conf_check(confs[cur_sz], 1024, 63, d, i, cur_sz, cur_cap,
                            confs);
            make_conf_check(confs[cur_sz], 1024 + 63, 0, d, i, cur_sz, cur_cap,
                            confs);
        }

        for (uint32_t i = 256 + 64; i <= 1024; i += 64) {
            make_conf_check(confs[cur_sz], 0, 0, d, i, cur_sz, cur_cap, confs);
            make_conf_check(confs[cur_sz], 1, 0, d, i, cur_sz, cur_cap, confs);
            make_conf_check(confs[cur_sz], 0, 1, d, i, cur_sz, cur_cap, confs);
            make_conf_check(confs[cur_sz], 0, 63, d, i, cur_sz, cur_cap, confs);
            make_conf_check(confs[cur_sz], 63, 0, d, i, cur_sz, cur_cap, confs);

            make_conf_check(confs[cur_sz], 1024, 0, d, i, cur_sz, cur_cap,
                            confs);
            make_conf_check(confs[cur_sz], 1024 + 1, 0, d, i, cur_sz, cur_cap,
                            confs);
            make_conf_check(confs[cur_sz], 1024, 1, d, i, cur_sz, cur_cap,
                            confs);
            make_conf_check(confs[cur_sz], 1024, 63, d, i, cur_sz, cur_cap,
                            confs);
            make_conf_check(confs[cur_sz], 1024 + 63, 0, d, i, cur_sz, cur_cap,
                            confs);
        }

        for (uint32_t i = 1024 + 256; i <= 2048; i += 256) {
            make_conf_check(confs[cur_sz], 0, 0, d, i, cur_sz, cur_cap, confs);
            make_conf_check(confs[cur_sz], 1, 0, d, i, cur_sz, cur_cap, confs);
            make_conf_check(confs[cur_sz], 0, 1, d, i, cur_sz, cur_cap, confs);
            make_conf_check(confs[cur_sz], 0, 63, d, i, cur_sz, cur_cap, confs);
            make_conf_check(confs[cur_sz], 63, 0, d, i, cur_sz, cur_cap, confs);

            make_conf_check(confs[cur_sz], 1024, 0, d, i, cur_sz, cur_cap,
                            confs);
            make_conf_check(confs[cur_sz], 1024 + 1, 0, d, i, cur_sz, cur_cap,
                            confs);
            make_conf_check(confs[cur_sz], 1024, 1, d, i, cur_sz, cur_cap,
                            confs);
            make_conf_check(confs[cur_sz], 1024, 63, d, i, cur_sz, cur_cap,
                            confs);
            make_conf_check(confs[cur_sz], 1024 + 63, 0, d, i, cur_sz, cur_cap,
                            confs);
        }

        for (uint32_t i = 2048 + 512; i <= 4096; i += 512) {
            make_conf_check(confs[cur_sz], 0, 0, d, i, cur_sz, cur_cap, confs);
            make_conf_check(confs[cur_sz], 1, 0, d, i, cur_sz, cur_cap, confs);
            make_conf_check(confs[cur_sz], 0, 1, d, i, cur_sz, cur_cap, confs);
            make_conf_check(confs[cur_sz], 0, 63, d, i, cur_sz, cur_cap, confs);
            make_conf_check(confs[cur_sz], 63, 0, d, i, cur_sz, cur_cap, confs);

            make_conf_check(confs[cur_sz], 1024, 0, d, i, cur_sz, cur_cap,
                            confs);
            make_conf_check(confs[cur_sz], 1024 + 1, 0, d, i, cur_sz, cur_cap,
                            confs);
            make_conf_check(confs[cur_sz], 1024, 1, d, i, cur_sz, cur_cap,
                            confs);
            make_conf_check(confs[cur_sz], 1024, 63, d, i, cur_sz, cur_cap,
                            confs);
            make_conf_check(confs[cur_sz], 1024 + 63, 0, d, i, cur_sz, cur_cap,
                            confs);
        }

        for (uint32_t i = 4096 + 1024; i <= 16384; i += 1024) {
            make_conf_check(confs[cur_sz], 0, 0, d, i, cur_sz, cur_cap, confs);
            make_conf_check(confs[cur_sz], 1, 0, d, i, cur_sz, cur_cap, confs);
            make_conf_check(confs[cur_sz], 0, 1, d, i, cur_sz, cur_cap, confs);
            make_conf_check(confs[cur_sz], 0, 63, d, i, cur_sz, cur_cap, confs);
            make_conf_check(confs[cur_sz], 63, 0, d, i, cur_sz, cur_cap, confs);

            make_conf_check(confs[cur_sz], 1024, 0, d, i, cur_sz, cur_cap,
                            confs);
            make_conf_check(confs[cur_sz], 1024 + 1, 0, d, i, cur_sz, cur_cap,
                            confs);
            make_conf_check(confs[cur_sz], 1024, 1, d, i, cur_sz, cur_cap,
                            confs);
            make_conf_check(confs[cur_sz], 1024, 63, d, i, cur_sz, cur_cap,
                            confs);
            make_conf_check(confs[cur_sz], 1024 + 63, 0, d, i, cur_sz, cur_cap,
                            confs);
        }
    }
    *nconfs_out = cur_sz;
    return confs;
}

bench_conf_t *
make_medium_confs(uint32_t dst_al_offset, uint32_t * nconfs_out) {
    uint32_t cur_sz  = 0;
    uint32_t cur_cap = 128 * 64;
    uint32_t sz_incr = 16;

    bench_conf_t * confs =
        (bench_conf_t *)safe_calloc(cur_cap, sizeof(bench_conf_t));
    for (uint32_t i = 128; i < 8192; i += sz_incr) {
        if (i >= 256) {
            sz_incr = 64;
        }
        if (i >= 4096) {
            sz_incr = 256;
        }
        uint32_t al_incr = 32;
        if (i > 512) {
            // no longer relevant and don't want to change any potential TLB
            // behavior
            dst_al_offset = 0;
        }
        for (uint32_t j = 0; j <= 256; j += al_incr) {
            if (j >= 64) {
                al_incr = 64;
            }
            for (uint32_t k = 0; k < 2; ++k) {
                make_conf_check(confs[cur_sz], dst_al_offset + 0, j, k, i,
                                cur_sz, cur_cap, confs);
                make_conf_check(confs[cur_sz], dst_al_offset + 0, j + 1, k, i,
                                cur_sz, cur_cap, confs);
                make_conf_check(confs[cur_sz], dst_al_offset + 1, j + 1, k, i,
                                cur_sz, cur_cap, confs);
                make_conf_check(confs[cur_sz], dst_al_offset + 1, j + 31, k, i,
                                cur_sz, cur_cap, confs);
                make_conf_check(confs[cur_sz], dst_al_offset + 0, j + 31, k, i,
                                cur_sz, cur_cap, confs);
                make_conf_check(confs[cur_sz], dst_al_offset + 31, j + 31, k, i,
                                cur_sz, cur_cap, confs);

                make_conf_check(confs[cur_sz], dst_al_offset + j, 0, k, i,
                                cur_sz, cur_cap, confs);
                make_conf_check(confs[cur_sz], dst_al_offset + j + 1, 0, k, i,
                                cur_sz, cur_cap, confs);
                make_conf_check(confs[cur_sz], dst_al_offset + j + 1, 1, k, i,
                                cur_sz, cur_cap, confs);
                make_conf_check(confs[cur_sz], dst_al_offset + j + 31, 1, k, i,
                                cur_sz, cur_cap, confs);
                make_conf_check(confs[cur_sz], dst_al_offset + j + 31, 0, k, i,
                                cur_sz, cur_cap, confs);
                make_conf_check(confs[cur_sz], dst_al_offset + j + 31, 31, k, i,
                                cur_sz, cur_cap, confs);

                make_conf_check(confs[cur_sz], dst_al_offset + j, j, k, i,
                                cur_sz, cur_cap, confs);
                make_conf_check(confs[cur_sz], dst_al_offset + j + 1, j + 31, k,
                                i, cur_sz, cur_cap, confs);
                make_conf_check(confs[cur_sz], dst_al_offset + j + 1, j + 1, k,
                                i, cur_sz, cur_cap, confs);
                make_conf_check(confs[cur_sz], dst_al_offset + j + 31, j, k, i,
                                cur_sz, cur_cap, confs);
                make_conf_check(confs[cur_sz], dst_al_offset + j + 1, j + 0, k,
                                i, cur_sz, cur_cap, confs);
                make_conf_check(confs[cur_sz], dst_al_offset + j + 31, j + 1, k,
                                i, cur_sz, cur_cap, confs);
            }
        }
    }
    *nconfs_out = cur_sz;
    return confs;
}


bench_conf_t *
make_large_confs(uint32_t * nconfs_out) {
    uint32_t       cur_sz  = 0;
    uint32_t       cur_cap = 128 * 64;
    bench_conf_t * confs =
        (bench_conf_t *)safe_calloc(cur_cap, sizeof(bench_conf_t));
    for (uint32_t i = (1u << 21); i <= MAX_BENCH_SIZE / 2; i <<= 1) {
        // Size is generally less interesting to vary than alignment
        uint32_t bench_sz = i;
        for (uint32_t j = 0; j < 4; ++j) {
            uint32_t pg_offset = j * 4096;
            for (uint32_t k = 0; k < 2; ++k) {
                for (uint32_t m = 0; m <= 256; m = (m == 0 ? 32 : (m << 1))) {
                    make_conf_check(confs[cur_sz], pg_offset + 0, pg_offset + m,
                                    k, bench_sz, cur_sz, cur_cap, confs);
                    make_conf_check(confs[cur_sz], pg_offset + m, pg_offset + 0,
                                    k, bench_sz, cur_sz, cur_cap, confs);
                    make_conf_check(confs[cur_sz], pg_offset + m, pg_offset + 1,
                                    k, bench_sz, cur_sz, cur_cap, confs);
                    make_conf_check(confs[cur_sz], pg_offset + 1, pg_offset + m,
                                    k, bench_sz, cur_sz, cur_cap, confs);
                    make_conf_check(confs[cur_sz], pg_offset + m + 1,
                                    pg_offset + m, k, bench_sz, cur_sz, cur_cap,
                                    confs);
                    make_conf_check(confs[cur_sz], pg_offset + m,
                                    pg_offset + m + 1, k, bench_sz, cur_sz,
                                    cur_cap, confs);
                }
            }
        }
    }

    *nconfs_out = cur_sz;
    return confs;
}
