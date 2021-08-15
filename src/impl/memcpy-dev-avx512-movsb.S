#define MEMCPY	memcpy_dev_avx512_movsb

#define USE_WITH_MULTIARCH_AND_LIBC
#define USE_WITH_ERMS

#include "avx512-vecs.h"
#include "core/memcpy-dev-avx512-final.S"
//#include "core/memcpy-dev-v64.S"
