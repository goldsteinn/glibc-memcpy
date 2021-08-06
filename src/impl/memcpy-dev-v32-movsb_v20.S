#define MEMCPY	memcpy_dev_v32_movsb_v20

#define USE_WITH_MULTIARCH_AND_LIBC
#define USE_WITH_ERMS
#include "evex-vecs.h"

#include "core/memcpy-dev-v20.S"
