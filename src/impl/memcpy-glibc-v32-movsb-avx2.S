#define MEMCPY	memcpy_glibc_v32_movsb_avx2
#include "avx2-vecs.h"

#define USE_WITH_MULTIARCH_AND_LIBC
#define USE_WITH_ERMS

#include "core/memcpy-glibc.S"
