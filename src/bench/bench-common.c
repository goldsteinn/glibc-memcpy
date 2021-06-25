#include "bench-common.h"
#include "bench-params.h"
#include "error-util.h"
#define csv_hdr                                                                \
    "%-12s,%-12s,%-8s,%-7s,%-7s,%-9s,%-7s,%-10s,%-10s,%-10s,%-10s,%-10s,%-"    \
    "10s\n"
#define csv_body                                                               \
    "%-12s,%-12s,%-8u,%-7u,%-7u,%-9u,%-7u,%-10.2E,%-10.2E,%-10.2E,%-10.2E,%-"  \
    "10.2E,%-10.2E\n"

static void
display_results(FILE * fp, const bench_result_t * result) {
    bench_stats_t *        stats  = result->stats;
    const bench_params_t * params = result->params;
    die_assert(stats != NULL);
    uint64_t nconfs = params->nconfs;
    for (uint32_t i = 0; i < nconfs; ++i) {
        fprintf(fp, csv_body, result->impl_name, params->test_name,
                params->confs[i].sz, params->confs[i].al_dst,
                params->confs[i].al_src, params->confs[i].direction,
                params->trials, stats[i].mean, stats[i].median, stats[i].gmean,
                stats[i].min, stats[i].max, stats[i].stdev);
    }
}

void
display_all_results(const char *           file_path,
                    const bench_result_t * results,
                    uint64_t               nresults) {

    FILE * fp = stdout;
    if (file_path != NULL && strlen(file_path) > 1) {
        fp = fopen(file_path, "w+");
        die_assert(fp != NULL, "Error opening: %s\n", file_path);
    }
    fprintf(fp, csv_hdr, "impl name", "test name", "size", "al dst", "al src",
            "dst > src", "trials", "mean", "median", "geomean", "min", "max",
            "stdev");

    for (uint64_t i = 0; i < nresults; ++i) {
        display_results(fp, results + i);
    }
    fclose(fp);
}


static void
get_stats(bench_result_t * result) {
    die_assert(result->stats != NULL);
    PRINTFFL;
    uint32_t nconfs = result->params->nconfs;
    PRINTFFL;

    uint32_t trial_offset = 0;
    uint32_t trials       = result->params->trials;
    PRINTFFL;
    for (uint64_t i = 0; i < nconfs; ++i) {
        make_stats(result->stats + i, result->times + trial_offset, trials);
        trial_offset += trials;
    }
}

static void
get_all_stats(bench_result_t * results, uint64_t nresults) {
    PRINTFFL;
    for (uint64_t i = 0; i < nresults; ++i) {
        PRINTFFL;
        get_stats(results + i);
    }
}


static void
bench_init(const bench_params_t * params,
           uint64_t               nparams,
           const bench_t *        bench_info,
           bench_result_t **      results_out,
           bench_char_t **        mem_lo_out,
           bench_char_t **        mem_hi_out) {

    bench_result_t * results =
        (bench_result_t *)safe_calloc(nparams, sizeof(bench_result_t));

    for (uint64_t i = 0; i < nparams; ++i) {
        results[i].times = (uint64_t *)safe_calloc(
            params[i].nconfs * params[i].trials, sizeof(uint64_t));
        results[i].stats     = (bench_stats_t *)safe_calloc(params[i].nconfs,
                                                        sizeof(bench_stats_t));
        results[i].impl_name = bench_info->name;
        results[i].params    = params;
    }
    bench_char_t * init_mem =
        (bench_char_t *)safe_mmap_alloc(2 * MAX_BENCH_SIZE);
    // set -1 so that 0-over-0 writeback optimization is disabled
    uint32_t fill = rand();
    memset(init_mem, fill | 1, 2 * MAX_BENCH_SIZE);
    // page in early pieces for src/dst
    memset(init_mem + MAX_BENCH_SIZE, fill | 4, 104876);
    memset(init_mem, fill | 2, 1048576);

    // finally get first bit hot in L1
    memset(init_mem + MAX_BENCH_SIZE, fill | 16, 8192);
    memset(init_mem, fill | 8, 8192);


    *results_out = results;
    *mem_lo_out  = init_mem;
    *mem_hi_out  = init_mem + MAX_BENCH_SIZE;
}

static void
bench_finish(const char *     file_path,
             uint64_t         nresults,
             bench_result_t * results,
             bench_char_t *   init_mem) {
    PRINTFFL;
    get_all_stats(results, nresults);
    PRINTFFL;
    display_all_results(file_path, results, nresults);
    PRINTFFL;
    for (uint64_t i = 0; i < nresults; ++i) {
        safe_free(results[i].stats);
        safe_free(results[i].times);
    }
    safe_free(results);
    safe_munmap(init_mem, 2 * MAX_BENCH_SIZE);
}

void
run_benchmark(const char *           file_path,
              const bench_params_t * params,
              uint64_t               nparams,
              const bench_t *        bench_info) {
    die_assert(inner_trials < LSD_START,
               "Benchmarkings will have unreasonable overhead from branch "
               "misses caused by entering / exiting the LSD");
    PRINTFFL;
    bench_result_t * results;
    bench_char_t *   mem_lo;
    bench_char_t *   mem_hi;
    bench_init(params, nparams, bench_info, &results, &mem_lo, &mem_hi);
    PRINTFFL;
    for (uint64_t i = 0; i < nparams; ++i) {
        PRINTFFL;
        bench_info->func(params + i, results[i].times, mem_lo, mem_hi);
        PRINTFFL;
    }
    PRINTFFL;
    bench_finish(file_path, nparams, results, mem_lo);
    PRINTFFL;
}
