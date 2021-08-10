#include "memcpy-defs.h"
#include <macro.h>


void * memcpy_glibc_v32_movsb(void *, const void *, size_t);
void * memcpy_dev_v32_movsb(void *, const void *, size_t);
void * memcpy_dev_v32_movsb_v19(void *, const void *, size_t);
void * memcpy_dev_v32_movsb_v20(void *, const void *, size_t);
void * memcpy_dev_v32_movsb_v21(void *, const void *, size_t);
void * memcpy_dev_v32_movsb_v22(void *, const void *, size_t);
void * memcpy_dev_v32_movsb_v23(void *, const void *, size_t);
void * memcpy_dev_v32_movsb_avx2(void *, const void *, size_t);
void * memcpy_glibc_v32_movsb_avx2(void *, const void *, size_t);
void * memcpy_dev_v32_movsb_avx2(void *, const void *, size_t);
void * memcpy_dev_v32_movsb_avx2_v19(void *, const void *, size_t);
void * memcpy_dev_v32_movsb_avx2_v20(void *, const void *, size_t);
void * memcpy_dev_v32_movsb_avx2_v21(void *, const void *, size_t);
void * memcpy_dev_v32_movsb_avx2_v22(void *, const void *, size_t);

#define NAME memcpy_glibc_v32_movsb
#include <bench/bench-memcpy.h>
#undef NAME


#define NAME memcpy_dev_v32_movsb
#include <bench/bench-memcpy.h>
#undef NAME

#define NAME memcpy_dev_v32_movsb_v19
#include <bench/bench-memcpy.h>
#undef NAME

#define NAME memcpy_dev_v32_movsb_v20
#include <bench/bench-memcpy.h>
#undef NAME
#define NAME memcpy_dev_v32_movsb_v21
#include <bench/bench-memcpy.h>
#undef NAME
#define NAME memcpy_dev_v32_movsb_v22
#include <bench/bench-memcpy.h>
#undef NAME

#define NAME memcpy_dev_v32_movsb_v23
#include <bench/bench-memcpy.h>
#undef NAME


#define NAME memcpy_glibc_v32_movsb_avx2
#include <bench/bench-memcpy.h>
#undef NAME


#define NAME memcpy_dev_v32_movsb_avx2
#include <bench/bench-memcpy.h>
#undef NAME

#define NAME memcpy_dev_v32_movsb_avx2_v19
#include <bench/bench-memcpy.h>
#undef NAME

#define NAME memcpy_dev_v32_movsb_avx2_v20
#include <bench/bench-memcpy.h>
#undef NAME
#define NAME memcpy_dev_v32_movsb_avx2_v21
#include <bench/bench-memcpy.h>
#undef NAME
#define NAME memcpy_dev_v32_movsb_avx2_v22
#include <bench/bench-memcpy.h>
#undef NAME

#define make_memcpy_info(name)                                                 \
    { &name, &CAT(bench_, name), V_TO_STR(name) }


const memcpy_info_t memcpy_defs[] = {
    make_memcpy_info(memcpy_glibc_v32_movsb),
    make_memcpy_info(memcpy_dev_v32_movsb),
    make_memcpy_info(memcpy_dev_v32_movsb_v19),
    make_memcpy_info(memcpy_dev_v32_movsb_v20),
    make_memcpy_info(memcpy_dev_v32_movsb_v21),
    make_memcpy_info(memcpy_dev_v32_movsb_v22),
    make_memcpy_info(memcpy_dev_v32_movsb_v23),
    make_memcpy_info(memcpy_glibc_v32_movsb_avx2),
    make_memcpy_info(memcpy_dev_v32_movsb_avx2),
    make_memcpy_info(memcpy_dev_v32_movsb_avx2_v19),
    make_memcpy_info(memcpy_dev_v32_movsb_avx2_v20),
    make_memcpy_info(memcpy_dev_v32_movsb_avx2_v21),
    make_memcpy_info(memcpy_dev_v32_movsb_avx2_v22)
};
const uint64_t nmemcpy_defs = sizeof(memcpy_defs) / sizeof(memcpy_defs[0]);
