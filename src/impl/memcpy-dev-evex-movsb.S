#define MEMCPY	memcpy_dev_evex_movsb

#define USE_WITH_MULTIARCH_AND_LIBC
#define USE_WITH_ERMS

#include "evex-vecs.h"

#include "core/memcpy-dev-v32.S"
