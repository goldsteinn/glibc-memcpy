#define MEMCPY	memcpy_dev_avx_movsb

#define USE_WITH_MULTIARCH_AND_LIBC
#define USE_WITH_ERMS

#include "avx-vecs.h"

#include "core/memcpy-dev-avx-final-v0.S"
