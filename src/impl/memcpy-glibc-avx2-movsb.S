#define MEMCPY	memcpy_glibc_avx2_movsb


#define USE_WITH_MULTIARCH_AND_LIBC
//#define USE_WITH_ERMS

#include "avx2-vecs.h"

#include "core/memcpy-new.S"
