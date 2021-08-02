#define MEMCPY	memcpy_dev_v32_movsb
#include "evex-vecs.h"

#define USE_WITH_MULTIARCH_AND_LIBC
#define USE_WITH_ERMS

#include "core/memcpy-dev-v7.S"
