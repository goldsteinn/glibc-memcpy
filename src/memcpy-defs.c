#include "memcpy-defs.h"
#include <macro.h>


void * memcpy_glibc_evex_movsb(void *, const void *, size_t);
void * memcpy_dev_evex_movsb(void *, const void *, size_t);
void * memcpy_glibc_avx2_movsb(void *, const void *, size_t);
void * memcpy_dev_avx2_movsb(void *, const void *, size_t);
void * memcpy_glibc_avx_movsb(void *, const void *, size_t);
void * memcpy_dev_avx_movsb(void *, const void *, size_t);
void * memcpy_glibc_avx512_movsb(void *, const void *, size_t);
void * memcpy_dev_avx512_movsb(void *, const void *, size_t);

#define NAME memcpy_glibc_evex_movsb
#include <bench/bench-memcpy.h>
#undef NAME


#define NAME memcpy_dev_evex_movsb
#include <bench/bench-memcpy.h>
#undef NAME

#define NAME memcpy_glibc_avx2_movsb
#include <bench/bench-memcpy.h>
#undef NAME


#define NAME memcpy_dev_avx2_movsb
#include <bench/bench-memcpy.h>
#undef NAME

#define NAME memcpy_glibc_avx_movsb
#include <bench/bench-memcpy.h>
#undef NAME


#define NAME memcpy_dev_avx_movsb
#include <bench/bench-memcpy.h>
#undef NAME

#define NAME memcpy_glibc_avx512_movsb
#include <bench/bench-memcpy.h>
#undef NAME


#define NAME memcpy_dev_avx512_movsb
#include <bench/bench-memcpy.h>
#undef NAME

#define make_memcpy_info(name)                                                 \
    { &name, &CAT(bench_, name), V_TO_STR(name) }


const memcpy_info_t memcpy_defs[] = { make_memcpy_info(memcpy_glibc_evex_movsb),
                                      make_memcpy_info(memcpy_dev_evex_movsb),
                                      make_memcpy_info(memcpy_glibc_avx2_movsb),
                                      make_memcpy_info(memcpy_dev_avx2_movsb),
                                      make_memcpy_info(memcpy_glibc_avx_movsb),
                                      make_memcpy_info(memcpy_dev_avx_movsb),
                                      make_memcpy_info(memcpy_glibc_avx512_movsb),
                                      make_memcpy_info(memcpy_dev_avx512_movsb)

};
const uint64_t nmemcpy_defs = sizeof(memcpy_defs) / sizeof(memcpy_defs[0]);
