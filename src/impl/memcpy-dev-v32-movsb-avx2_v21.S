#define MEMCPY	memcpy_dev_v32_movsb_avx2_v21

#define USE_WITH_MULTIARCH_AND_LIBC
#define USE_WITH_ERMS
#include "avx2-vecs.h"
#include "core/memcpy-dev-v21.S"
