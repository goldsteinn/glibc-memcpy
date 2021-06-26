#include <bench/bench-constants.h>
#include <bench/bench-entry.h>
#include "arg.h"
#include "error-util.h"

extern const bench_t  bench_funcs[];
extern const uint64_t nbench_funcs;


uint32_t verbose       = 0;
int32_t  core          = -1;
int32_t  large_trials  = 0;
int32_t  medium_trials = 0;
int32_t  small_trials  = 0;
int32_t  rand_trials   = 0;
uint32_t no_4k_alias   = 0;


char * func_name = NULL;
char * file_path = NULL;

// clang-format off
static ArgOption args[] = {
    //  Kind,           Method,		name,	    reqd,   variable,       help
  {     KindOption,     Set, 		"-v", 		0,      &verbose,       "Turn on verbosity" },
  {     KindOption,     Integer, 	"--core",	0,      &core,          "Core to pin process to for benchmark. (less than 0 skips this)" },
  {     KindOption,     Set,    	"--noalias",0,      &no_4k_alias    ,"Set to adjust alignments so that src/dst don't 4k alias (only small / medium)" },
  {     KindOption,     Integer, 	"--st",     0,      &small_trials,  "Set to number of trials for small params. (0 skips, -1 sets trials to default)" },
  {     KindOption,     Integer, 	"--mt",     0,      &medium_trials, "Set to number of trials for medium params. (0 skips, -1 sets trials to default)" },
  {     KindOption,     Integer, 	"--lt",     0,      &large_trials,  "Set to number of trials for large params. (0 skips, -1 sets trials to default)" },
  {     KindOption,     Integer, 	"--rt",     0,      &rand_trials,   "Set to number of trials for rand params. (0 skips, -1 sets trials to default)" },
  {     KindOption,     String,		"--func",	0,      &func_name,     "Memcpy implementation to benchmark" },

  {     KindHelp,       Help,       "-h",       0,      NULL,           ""  },
  {     KindEnd,        EndOptions, "",         0,      NULL,           ""  }
};
// clang-format on

static ArgDefs argp = { args, "Driver for glibc qsort development", NULL,
                        NULL };

int
main(int argc, char ** argv) {
    die_assert(!doParse(&argp, argc, argv), "Error parsing arguments\n");
    die_assert(large_trials | medium_trials | small_trials | rand_trials,
               "No benchmark requested\n");
    uint64_t dst_al_offset = no_4k_alias ? 2048 : 0;

    bench_params_t params[4];
    uint64_t       nparams = 0;
    if (small_trials) {
        if (small_trials < 0) {
            small_trials = DEFAULT_SMALL_TRIALS;
        }
        init_small_params(params + nparams, dst_al_offset, small_trials);
        ++nparams;
    }

    if (medium_trials) {
        if (medium_trials < 0) {
            medium_trials = DEFAULT_MEDIUM_TRIALS;
        }
        init_medium_params(params + nparams, dst_al_offset, medium_trials);
        ++nparams;
    }

    if (large_trials) {
        if (large_trials < 0) {
            large_trials = DEFAULT_LARGE_TRIALS;
        }
        init_large_params(params + nparams, large_trials);
        ++nparams;
    }

    if (rand_trials) {
        if (rand_trials < 0) {
            rand_trials = DEFAULT_RAND_TRIALS;
        }
        init_rand_params(params + nparams, rand_trials);
        ++nparams;
    }

    run_benchmarks(params, nparams, &bench_funcs[0], core);
    destroy_params(params, nparams);
}
