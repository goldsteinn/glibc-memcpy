#include "arg.h"
#include "bench/bench-entry.h"
#include "error-util.h"

extern const bench_t  bench_funcs[];
extern const uint64_t nbench_funcs;

uint32_t verbose   = 0;
uint32_t test      = 0;
uint32_t bench     = 0;
char *   func_name = NULL;
char *   file_path = NULL;

// clang-format off
static ArgOption args[] = {
    //  Kind,           Method,		name,	    reqd,   variable,		help
  {     KindOption,     Set, 		"-v", 		0,      &verbose, 		"Turn on verbosity" },
  {     KindOption,     Set, 		"--bench",	0,      &bench, 		"Set to benchmark" },
  {     KindOption,     Set, 		"--test",	0,      &test,          "Set to test" },
  {     KindOption,     String,		"--func",	0,      &func_name, 	"Function to sort" },

  {     KindHelp,       Help,       "-h",       0,      NULL,           ""  },
  {     KindEnd,        EndOptions, "",         0,      NULL,           ""  }
};
// clang-format on

static ArgDefs argp = { args, "Driver for glibc qsort development", NULL,
                        NULL };

int
main(int argc, char ** argv) {
    die_assert(!doParse(&argp, argc, argv), "Error parsing arguments");
    bench_params_t params;
    init_rand_params(&params, 10000);
    run_benchmarks(&params, 1, &bench_funcs[0]);
    destroy_params(&params, 1);
}
