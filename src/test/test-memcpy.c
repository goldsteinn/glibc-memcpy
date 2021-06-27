#include <common.h>
#include <memory-util.h>


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

#define NPAIRS    4
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

    pairs[S1_IDX(3)] = (alignment * 3) / 5;
    pairs[S2_IDX(3)] = alignment / 3;
}

#define START 1
#define END   0

static void
init_sentinels(const void * start, const void * end, int32_t start_or_end) {
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
check_sentinel(const void * start, const void * end, int start_or_end) {
    assert(start <= end);
    __m256i  boundary_v = _mm256_set1_epi8(-1);
    uint32_t expec      = (start + 32 <= end) ? ~0 : (1u << (end - start)) - 1;
    uint32_t shift      = (start + 32 <= end) || (start_or_end == START)
                              ? 0
                              : (start - (end - 32));
    if ((start + 32 <= end) == start_or_end) {
        PRINTLN;
        __m256i v = _mm256_loadu_si256((__m256i *)(end - 32));
        return ((_mm256_movemask_epi8(_mm256_cmpeq_epi8(v, boundary_v)) >>
                 shift) &
                expec) != expec;
    }
    else {
        PRINTLN;
        __m256i v = _mm256_loadu_si256((__m256i *)start);
        return ((_mm256_movemask_epi8(_mm256_cmpeq_epi8(v, boundary_v)) >>
                 shift) &
                expec) != expec;
    }
}

static int32_t
check_memcpy(void *       s1_start,
             void *       s1_end,
             void *       s1,
             const void * s2,
             uint64_t     len) {
    if (check_sentinel(s1_start, s1, START)) {
        return 1;
    }
    if (memcmp(s1, s2, pos)) {
        return 2;
    }
    if (check_sentinel(s1 + pos, s1_end, END)) {
        PRINTLN;
        return 3;
    }
    return 0;
}

void
run_small_tests_kernel(const char * const name, const test_func func) {
    uint8_t * s1[NPAIRS] = { NULL };
    uint8_t * s2         = make_buf(sz);
    uint8_t * test1[NPAIRS], *test2;

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
            for (int i = 0; i < NPAIRS; ++i) {
                test1[i] = s1[i] + al_pairs[S1_IDX(i)];
                for (uint64_t tlen = 0; tlen < sz - alignment; ++tlen) {
                    for (uint32_t i = 0; i < NPAIRS; ++i) {
                    }
                }
            }
        }
    }
}
