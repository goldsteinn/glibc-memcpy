#define MEMCPY	memcpy_dev_avx512_movsb

#define USE_WITH_MULTIARCH_AND_LIBC
#define USE_WITH_ERMS
#define USE_WITH_FRMS
#define USE_MOVSB
#include "avx512-vecs.h"
#include "core/memcpy-new.S"
