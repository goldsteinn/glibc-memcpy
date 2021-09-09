#define MEMCPY	memcpy_glibc_evex_movsb


#define USE_WITH_MULTIARCH_AND_LIBC
//#define USE_WITH_ERMS

#include "evex-vecs.h"
#define NO_MOVSB
#include "core/memcpy-dev.S"
