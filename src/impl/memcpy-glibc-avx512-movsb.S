#define MEMCPY	memcpy_glibc_avx512_movsb


#define USE_WITH_MULTIARCH_AND_LIBC
#define USE_WITH_ERMS

#include "avx512-vecs.h"

#include "core/memcpy-glibc.S"
