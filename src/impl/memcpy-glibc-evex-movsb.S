#define MEMCPY	memcpy_glibc_evex_movsb


#define USE_WITH_MULTIARCH_AND_LIBC
#define USE_WITH_ERMS

#include "evex-vecs.h"

#include "core/memcpy-glibc.S"
