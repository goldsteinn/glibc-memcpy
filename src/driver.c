#include <bench/bench-entry.h>
#include <test/test-memcpy.h>
#include "arg.h"
#include "error-util.h"

extern const memcpy_info_t memcpy_defs[];
extern const uint64_t      nmemcpy_defs;


uint32_t          verbose         = 0;
int32_t           core            = -1;
int32_t           large_trials    = 0;
int32_t           medium_trials   = 0;
int32_t           small_trials    = 0;
int32_t           rand_trials     = 0;
int32_t           custom_trials   = 0;
uint32_t          no_4k_alias     = 0;
int32_t           human_readable  = 0;
uint32_t          test            = 0;
uint32_t          bench           = 0;
uint32_t          min_rand_size   = 0;
uint32_t          max_rand_size   = UINT32_MAX;
uint32_t          rand_size_scale = 1;
uint32_t          nrand_confs     = num_rand_confs;
bench_direction_t rand_direction  = BIDIRECTIONAL;

char * func_name = NULL;
char * file_path = NULL;

// clang-format off
static ArgOption args[] = {
    //  Kind,           Method,		name,	    reqd,   variable,           help
  {     KindOption,     Set, 		"-v", 		0,      &verbose,           "Turn on verbosity" },
  {     KindOption,     Integer, 	"--core",	0,      &core,              "Core to pin process to for benchmark. (less than 0 skips this)" },
  {     KindOption,     Set,    	"--noalias",0,      &no_4k_alias,       "Set to adjust alignments so that src/dst don't 4k alias (only small / medium)" },
  {     KindOption,     Integer, 	"--st",     0,      &small_trials,      "Set to number of trials for small params. (0 skips, -1 sets trials to default)" },
  {     KindOption,     Integer, 	"--mt",     0,      &medium_trials,     "Set to number of trials for medium params. (0 skips, -1 sets trials to default)" },
  {     KindOption,     Integer, 	"--lt",     0,      &large_trials,      "Set to number of trials for large params. (0 skips, -1 sets trials to default)" },
  {     KindOption,     Integer, 	"--rt",     0,      &rand_trials,       "Set to number of trials for rand params. (0 skips, -1 sets trials to default)" },
  {     KindOption,     Integer, 	"--ct",     0,      &custom_trials,     "Set to number of trials for custom params (0 skips, -1 sets trials to default))." },
  {     KindOption,     String,		"--func",	0,      &func_name,         "Memcpy implementation to benchmark" },
  {     KindOption,     Set,		"--hr",     0,      &human_readable,    "Set to make output more human readable" },
  {     KindOption,     Set, 		"--test",	0,      &test,              "Set to test" },
  {     KindOption,     Integer, 	"--min",    0,      &min_rand_size,     "Min size for random distribution" },
  {     KindOption,     Integer, 	"--max",    0,      &max_rand_size,     "Max size for random distribution" },
  {     KindOption,     Integer, 	"--scale",  0,      &rand_size_scale,   "Scale factor for random size distribution" },
  {     KindOption,     Integer, 	"--nconfs", 0,      &nrand_confs,       "Number of rand configs. Must be power of 2." },
  {     KindOption,     Integer, 	"--dir",    0,      &rand_direction,    "Rand direction. 0 -> FORWARD, 1 -> BACKWARD, 2 -> BIDIRECTIONAL" },

  {     KindHelp,       Help,       "-h",       0,      NULL,               ""  },
  {     KindEnd,        EndOptions, "",         0,      NULL,               ""  }
};
// clang-format on

static ArgDefs argp = { args, "Driver for glibc qsort development", NULL,
                        NULL };

void EXIT_FUNC
list_funcs_and_die() {
    fprintf(stderr,
            "Unknown implementation function: %s\nAvailable "
            "implementations are:\n",
            func_name);
    for (uint64_t i = 0; i < nmemcpy_defs; ++i) {
        fprintf(stderr, "%-2lu: %-24s\n", i, memcpy_defs[i].name);
    }
    abort();
}

int
main(int argc, char ** argv) {
    die_assert(!doParse(&argp, argc, argv), "Error parsing arguments\n");
    die_assert(large_trials | medium_trials | small_trials | rand_trials |
                   test | custom_trials,
               "No benchmark requested\n");
    uint64_t              dst_al_offset = no_4k_alias ? 2048 : 0;
    const memcpy_info_t * memcpy_def    = NULL;
    if (func_name == NULL) {
        list_funcs_and_die();
    }
    for (uint64_t i = 0; i < nmemcpy_defs; ++i) {
        if (!strcmp(func_name, memcpy_defs[i].name)) {
            memcpy_def = &memcpy_defs[i];
            break;
        }
    }
    if (memcpy_def == NULL) {
        list_funcs_and_die();
    }
    if (test) {
        run_all_tests(memcpy_def);
    }
    else {

        bench_params_t params[4];
        uint64_t       nparams = 0;
        if (small_trials) {
            init_small_params(params + nparams, dst_al_offset, small_trials);
            ++nparams;
        }

        if (medium_trials) {
            init_medium_params(params + nparams, dst_al_offset, medium_trials);
            ++nparams;
        }

        if (large_trials) {
            init_large_params(params + nparams, large_trials);
            ++nparams;
        }

        if (rand_trials) {
            init_rand_params(params + nparams, rand_trials, min_rand_size,
                             max_rand_size, rand_size_scale, rand_direction,
                             nrand_confs);
            ++nparams;
        }
        if (custom_trials) {
            init_custom_params(params + nparams, custom_trials, min_rand_size,
                               max_rand_size, rand_direction, rand_size_scale);
            ++nparams;
        }

        run_benchmarks(params, nparams, memcpy_def, core);
        destroy_params(params, nparams);
    }
}
