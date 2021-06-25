#include <impl/impl-entry.h>
#include <macro.h>
#include <string.h>

#include "bench-params.h"


#define NAME memcpy_glibc_v32_movsb
#include "bench-memcpy.h"
#undef NAME
#define make_bench_func(name)                                                  \
    { &CAT(bench_, name), V_TO_STR(name) }


const bench_t  bench_funcs[] = { make_bench_func(memcpy_glibc_v32_movsb) };
const uint64_t nbench_funcs  = sizeof(bench_funcs) / sizeof(bench_funcs[0]);
