#include "bench-results.h"
#include "bench-constants.h"

#include <error-util.h>

#define csv_hdr                                                                \
    "%-24s,%-16s,%-8s,%-7s,%-7s,%-9s,%-7s,%-12s,%-10s,%-10s,%-10s,%-10s,%-"    \
    "10s,%-"                                                                   \
    "10s"

#define csv_body                                                               \
    "%-24s,%-16s,%-8d,%-7d,%-7d,%-9d,%-7d,%-12d,%-10.3E,%-10.3E,%-10.3E,%-10." \
    "2E,%-"                                                                    \
    "10.3E,%-10.3E"

#define hr_csv_hdr "%-16s,%-8s,%-7s,%-7s,%-9s,%-10s\n"

#define hr_csv_body "%-16s,%-8d,%-7d,%-7d,%-9d,%-10.4E\n"

extern char *    file_path;
extern int32_t   human_readable;
extern perf_ev_t perf_events_in_use[];

static void
display_results(FILE *                 fp,
                const bench_result_t * result,
                const perf_ev_t *      perf_events) {
    bench_stats_t *        stats  = result->stats;
    const bench_params_t * params = result->params;
    die_assert(stats != NULL);
    uint64_t nconfs = params->nconfs;
    for (uint32_t i = 0; i < nconfs; ++i) {
        if (human_readable) {
            fprintf(fp, hr_csv_body, params->test_name,
                    params_get_conf_sz(params, i),
                    params_get_conf_al_dst(params, i),
                    params_get_conf_al_src(params, i),
                    params_get_conf_direction(params, i), stats[i].gmean);
            display_counters(fp, &(result->ev_results), perf_events,
                             human_readable);
        }
        else {

            fprintf(fp, csv_body, result->impl_name, params->test_name,
                    params_get_conf_sz(params, i),
                    params_get_conf_al_dst(params, i),
                    params_get_conf_al_src(params, i),
                    params_get_conf_direction(params, i), params->trials,
                    params->todo == FIXED ? inner_trials : params->nrand_confs,
                    stats[i].mean, stats[i].median, stats[i].gmean,
                    stats[i].min, stats[i].max, stats[i].stdev);
            display_counters(fp, &(result->ev_results), perf_events,
                             human_readable);
            fprintf(fp, "\n");
        }
    }
}


static void
get_stats(const bench_result_t * result) {
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
    PRINTFFL;
}

static void
get_all_stats(const bench_result_t * results, uint64_t nresults) {
    PRINTFFL;
    for (uint64_t i = 0; i < nresults; ++i) {
        PRINTFFL;
        get_stats(results + i);
    }
    PRINTFFL;
}

bench_result_t *
init_results(const bench_params_t * params,
             uint64_t               nparams,
             const char *           impl_name) {

    bench_result_t * results =
        (bench_result_t *)safe_calloc(nparams, sizeof(bench_result_t));

    for (uint64_t i = 0; i < nparams; ++i) {
        results[i].times = (uint64_t *)safe_calloc(
            params[i].nconfs * params[i].trials, sizeof(uint64_t));
        results[i].stats     = (bench_stats_t *)safe_calloc(params[i].nconfs,
                                                        sizeof(bench_stats_t));
        results[i].impl_name = impl_name;
        results[i].params    = params + i;
    }
    return results;
}

void
destroy_results(bench_result_t * results, uint64_t nresults) {
    PRINTFFL;
    for (uint64_t i = 0; i < nresults; ++i) {
        safe_free(results[i].stats);
        safe_free(results[i].times);
    }
    PRINTFFL;
    safe_free(results);
    PRINTFFL;
}


void
_display_all_results(const bench_result_t * results,
                     uint64_t               nresults,
                     const perf_ev_t *      perf_events) {
    get_all_stats(results, nresults);

    FILE * fp = stdout;
    if (file_path != NULL && strlen(file_path) > 1) {
        fp = fopen(file_path, "w+");
        err_assert(fp != NULL, "Error opening: %s\n", file_path);
    }
    if (human_readable) {
        fprintf(fp, hr_csv_hdr, "test name", "size", "al dst", "al src",
                "dst > src", "geomean");
    }
    else {
        fprintf(fp, csv_hdr, "impl name", "test name", "size", "al dst",
                "al src", "dst > src", "trials", "inner trials", "mean",
                "median", "geomean", "min", "max", "stdev");
        display_counters_hdr(fp, perf_events);
        fprintf(fp, "\n");
    }

    for (uint64_t i = 0; i < nresults; ++i) {
        display_results(fp, results + i, perf_events);
    }
    fclose(fp);
}

void
display_all_results(const bench_result_t * results, uint64_t nresults) {
    _display_all_results(results, nresults, perf_events_in_use);
}
