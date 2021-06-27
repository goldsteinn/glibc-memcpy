#include <macro.h>
#include "memcpy-defs.h"


void * memcpy_glibc_v32_movsb(void *, const void *, size_t);
void * memcpy_dev_v32_movsb(void *, const void *, size_t);


#define NAME memcpy_glibc_v32_movsb
#include <bench/bench-memcpy.h>
#undef NAME


#define NAME memcpy_dev_v32_movsb
#include <bench/bench-memcpy.h>
#undef NAME

#define make_memcpy_info(name)                                                 \
    { &name, &CAT(bench_, name), V_TO_STR(name) }


const memcpy_info_t memcpy_defs[] = {
    make_memcpy_info(memcpy_glibc_v32_movsb),
    make_memcpy_info(memcpy_dev_v32_movsb)
};
const uint64_t nmemcpy_defs = sizeof(memcpy_defs) / sizeof(memcpy_defs[0]);
