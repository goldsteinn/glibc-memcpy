#include <common.h>
#include <error-util.h>
#include <immintrin.h>
#include <memory-util.h>

#include <memcpy-defs.h>


#define progress_fmt "\r%-20s - [%-3lu / %-3lu]"
static const uint64_t alignments[] = {
    0,   1,   2,   3,   4,   5,   6,   7,   8,   15,  16,  17,  18,  19,
    20,  23,  24,  25,  31,  32,  33,  47,  48,  49,  50,  52,  59,  63,
    64,  65,  80,  81,  95,  96,  97,  101, 127, 128, 129, 159, 160, 161,
    191, 192, 193, 200, 210, 223, 224, 225, 255, 256, 257
};

static const uint64_t nalignments = sizeof(alignments) / sizeof(alignments[0]);


static void *
make_buf(uint64_t sz) {
    void * p = safe_mmap(NULL, 2 * PAGE_SIZE + sz, PROT_NONE,
                         MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    safe_mprotect(p + PAGE_SIZE, sz, PROT_READ | PROT_WRITE);
    return p + PAGE_SIZE;
}

static void
free_buf(void * p, uint64_t sz) {
    safe_munmap(p - PAGE_SIZE, sz + 2 * PAGE_SIZE);
}

#define NPAIRS    7
#define S1_IDX(x) ((x) << 1)
#define S2_IDX(x) (((x) << 1) + 1)

static void
make_alignment_pairs(uint64_t * pairs, uint64_t alignment) {
    pairs[S1_IDX(0)] = 0;
    pairs[S2_IDX(0)] = alignment;

    pairs[S1_IDX(1)] = alignment;
    pairs[S2_IDX(1)] = 0;

    pairs[S1_IDX(2)] = alignment;
    pairs[S2_IDX(2)] = alignment;

    pairs[S1_IDX(3)] = !!alignment;
    pairs[S2_IDX(3)] = alignment;

    pairs[S1_IDX(4)] = alignment;
    pairs[S2_IDX(4)] = !!alignment;

    pairs[S1_IDX(5)] = (alignment * 3) / 5;
    pairs[S2_IDX(5)] = alignment / 3;

    pairs[S1_IDX(6)] = alignment / 3;
    pairs[S2_IDX(6)] = (alignment * 3) / 5;
}

#define START 1
#define END   0

static void
init_sentinel(uint8_t * start, uint8_t * end, int32_t start_or_end) {
    die_assert(start <= end);
    __m256i v = _mm256_set1_epi8(-1);
    if ((start + 32 <= end) == start_or_end) {
        _mm256_storeu_si256((__m256i *)(end - 32), v);
    }
    else {
        _mm256_storeu_si256((__m256i *)start, v);
    }
}


static int32_t
check_sentinel(const uint8_t * start, const uint8_t * end, int start_or_end) {
    assert(start <= end);
    __m256i  boundary_v = _mm256_set1_epi8(-1);
    uint32_t expec      = (start + 32 <= end) ? ~0u : (1u << (end - start)) - 1;
    uint32_t shift      = (start + 32 <= end) || (start_or_end == START)
                              ? 0
                              : (start - (end - 32));
    if ((start + 32 <= end) == start_or_end) {
        PRINTFFL;
        __m256i v = _mm256_loadu_si256((__m256i *)(end - 32));
        return ((_mm256_movemask_epi8(_mm256_cmpeq_epi8(v, boundary_v)) >>
                 shift) &
                expec) != expec;
    }
    else {
        PRINTFFL;
        __m256i v = _mm256_loadu_si256((__m256i *)start);
        return ((_mm256_movemask_epi8(_mm256_cmpeq_epi8(v, boundary_v)) >>
                 shift) &
                expec) != expec;
    }
}
#define GET_EXPEC(addr, idx) (uint8_t)((((uint64_t)(addr)) + (idx)) % 255)

static void
init_region(uint8_t * s2, uint64_t len) {
    for (uint64_t i = 0; i < len; ++i) {
        s2[i] = GET_EXPEC(s2, i);
    }
}

static int32_t
check_region(const uint8_t * s1, const uint8_t * s2, uint64_t len) {
    for (uint64_t i = 0; i < len; ++i) {
        if (s1[i] != GET_EXPEC(s2, i)) {
            fprintf(stderr, "%lu / %lu\n", i, len);
            return 1;
        }
    }
    return memcmp(s1, s2, len);
}

static int32_t
check_region_mm(const uint8_t * s1, const uint8_t * s2, uint64_t len) {

    return memcmp(s1, s2, len);
}


static void
init_memmove(uint8_t * s1_start,
             uint8_t * s1_end,
             uint8_t * s1,
             uint8_t * s2_tmp,
             uint8_t * s2,
             uint64_t  len) {
    init_region(s2, len);
#if MEMMOVE_SENTINELS
    init_sentinel(s1_start, s1, START);
    init_sentinel(s1 + len, s1_end, END);
#endif
    memcpy(s2_tmp, s2, len);

    (void)(s1_start);
    (void)(s1_end);
    (void)(s1);
}


static void
init_memcpy(uint8_t * s1_start,
            uint8_t * s1_end,
            uint8_t * s1,
            uint8_t * s2,
            uint64_t  len) {
    init_sentinel(s1_start, s1, START);
    init_region(s2, len);
    init_sentinel(s1 + len, s1_end, END);
}


static void
init_memcpy_sentinels(uint8_t * s1_start,
                      uint8_t * s1_end,
                      uint8_t * s1,
                      uint64_t  len) {
    init_sentinel(s1_start, s1, START);
    init_sentinel(s1 + len, s1_end, END);
}

static int32_t
check_memcpy(const uint8_t * s1_start,
             const uint8_t * s1_end,
             const uint8_t * s1,
             const uint8_t * s2,
             uint64_t        len) {
    if (check_sentinel(s1_start, s1, START)) {
        fprintf(stderr, "Start Sentinel Error\n");
        return 1;
    }
    if (check_region(s1, s2, len)) {
        fprintf(stderr, "Region Error\n");
        return 2;
    }
    if (check_sentinel(s1 + len, s1_end, END)) {
        fprintf(stderr, "End Sentinel Error\n");
        return 3;
    }
    return 0;
}

static int32_t
check_memmove(const uint8_t * s1_start,
              const uint8_t * s1_end,
              const uint8_t * s1,
              const uint8_t * s2_tmp,
              uint64_t        len) {
#if MEMMOVE_SENTINELS
    if (check_sentinel(s1_start, s1, START)) {
        fprintf(stderr, "Start Sentinel Error\n");
        return 1;
    }
#endif
    if (check_region_mm(s1, s2_tmp, len)) {
        fprintf(stderr, "Region Error\n");
        for (uint32_t i = 0; i < len; ++i) {
            uint32_t tmp = s1[i] == s2_tmp[i];
            fprintf(stderr, "%u: %x vs %x --> %d\n", i, s1[i], s2_tmp[i], tmp);
            if (!tmp) {
                break;
            }
        }
        return 2;
    }
#if MEMMOVE_SENTINELS
    if (check_sentinel(s1 + len, s1_end, END)) {
        fprintf(stderr, "End Sentinel Error\n");
        return 3;
    }
#endif
    (void)(s1_start);
    (void)(s1_end);
    return 0;
}


static uint64_t
next_len(uint64_t len, uint64_t sz) {
    if (sz <= 2 * PAGE_SIZE) {
        return len + 1;
    }
    else {
        if (len < sz / 2) {
            return sz / 2;
        }
        return len + sz / 256;
    }
}


static void
run_overlapp_tests_kernel(const memcpy_info_t * memcpy_def, uint64_t sz) {
    const char *      name        = memcpy_def->name;
    const memcpy_func func        = memcpy_def->run_memcpy;
    size_t            progress_sz = sz;
    uint8_t *         s_base      = make_buf(sz);
    uint8_t *         s_tmp       = make_buf(sz);
    init_region(s_base, sz);
    init_region(s_tmp, sz);
    uint8_t *test1, *test2;

    uint64_t al_pairs[NPAIRS << 1] = { 0 };
    for (uint32_t repeats = 0; repeats < 2; ++repeats) {
        for (uint64_t al_idx = 0; al_idx < nalignments; ++al_idx) {

            fprintf(stderr, progress_fmt, name,
                    (repeats + (progress_sz & (~PAGE_SIZE)) / PAGE_SIZE) *
                            nalignments +
                        al_idx,
                    4 * nalignments);

            uint64_t alignment =
                repeats ? (PAGE_SIZE - alignments[al_idx]) : alignments[al_idx];
            make_alignment_pairs(al_pairs, alignment);
            for (uint64_t len = 0; len <= sz - alignment;
                 len          = next_len(len, sz)) {
                for (uint32_t i = 0; i < NPAIRS; ++i) {
                    test1 = s_base + al_pairs[S1_IDX(i)];
                    test2 = s_base + al_pairs[S2_IDX(i)];
                    init_memmove(s_base, s_base + sz, test1, s_tmp, test2, len);
                    func(test1, test2, len);
                    die_assert(
                        !check_memmove(s_base, s_base + sz, test1, s_tmp, len));


                    test1 = s_base + ((sz - len) - 0);
                    test2 = s_base + al_pairs[S2_IDX(i)];
                    init_memmove(s_base, s_base + sz, test1, s_tmp, test2, len);
                    func(test1, test2, len);
                    die_assert(
                        !check_memmove(s_base, s_base + sz, test1, s_tmp, len));

                    test1 = s_base + al_pairs[S1_IDX(i)];
                    test2 = s_base + ((sz - len) - 0);
                    init_memmove(s_base, s_base + sz, test1, s_tmp, test2, len);
                    func(test1, test2, len);
                    die_assert(
                        !check_memmove(s_base, s_base + sz, test1, s_tmp, len));
                }
            }
        }
    }

    free_buf(s_base, sz);
    free_buf(s_tmp, sz);
}


static void
run_no_overlapp_tests_kernel(const memcpy_info_t * memcpy_def, uint64_t sz) {
    const char *      name        = memcpy_def->name;
    const memcpy_func func        = memcpy_def->run_memcpy;
    size_t            progress_sz = sz;

    uint8_t * s1    = make_buf(sz);
    uint8_t * s2_lo = make_buf(sz);
    uint8_t * s2_hi = make_buf(sz);
    {
        uint8_t * tmp;
        if (s2_lo > s1) {
            tmp   = s1;
            s1    = s2_lo;
            s2_lo = tmp;
        }
        if (s1 > s2_hi) {
            tmp   = s1;
            s1    = s2_hi;
            s2_hi = tmp;
            if (s2_lo > s1) {
                tmp   = s1;
                s1    = s2_lo;
                s2_lo = tmp;
            }
        }
    }
    init_region(s1, sz);
    init_region(s2_lo, sz);
    init_region(s2_hi, sz);

    uint8_t *test1, *test2;
    uint64_t al_pairs[NPAIRS << 1] = { 0 };
    for (uint32_t repeats = 0; repeats < 2; ++repeats) {
        for (uint64_t al_idx = 0; al_idx < nalignments; ++al_idx) {

            fprintf(stderr, progress_fmt, name,
                    (repeats + (progress_sz & (~PAGE_SIZE)) / PAGE_SIZE) *
                            nalignments +
                        al_idx,
                    4 * nalignments);

            uint64_t alignment =
                repeats ? (PAGE_SIZE - alignments[al_idx]) : alignments[al_idx];
            make_alignment_pairs(al_pairs, alignment);
            for (uint64_t len = 0; len <= sz - alignment;
                 len          = next_len(len, sz)) {
                //                fprintf(stderr, "%d:%lu:%lu\n", repeats, al_idx, len);
                for (uint32_t i = 0; i < NPAIRS; ++i) {
                    test1 = s1 + al_pairs[S1_IDX(i)];
                    test2 = s2_lo + al_pairs[S2_IDX(i)];
                    init_memcpy_sentinels(s1, s1 + sz, test1, len);
                    func(test1, test2, len);
                    die_assert(!check_memcpy(s1, s1 + sz, test1, test2, len));

                    test1 = s1 + al_pairs[S1_IDX(i)];
                    test2 = s2_hi + al_pairs[S2_IDX(i)];
                    init_memcpy_sentinels(s1, s1 + sz, test1, len);
                    func(test1, test2, len);
                    die_assert(!check_memcpy(s1, s1 + sz, test1, test2, len));
                }
            }
        }
    }
    free_buf(s1, sz);
    free_buf(s2_lo, sz);
    free_buf(s2_hi, sz);
}


void
run_small_tests(const memcpy_info_t * memcpy_def, int32_t with_overlap) {
    fprintf(stderr, progress_fmt, memcpy_def->name, 0UL, 4 * nalignments);
    for (uint64_t sz = 1 * PAGE_SIZE; sz <= 2 * PAGE_SIZE; sz += PAGE_SIZE) {
        if (with_overlap) {
            run_overlapp_tests_kernel(memcpy_def, sz);
        }
        else {
            run_no_overlapp_tests_kernel(memcpy_def, sz);
        }
    }
    fprintf(stderr, progress_fmt "\n", memcpy_def->name, 4 * nalignments,
            4 * nalignments);
}

void
run_all_tests(const memcpy_info_t * memcpy_def) {
    run_small_tests(memcpy_def, 1);
    run_small_tests(memcpy_def, 0);

    for (uint64_t i = 13; i < 26; ++i) {
        //        run_no_overlapp_tests_kernel(memcpy_def, (1UL << i));
        //        run_overlapp_tests_kernel(memcpy_def, (1UL << i));
    }
}
