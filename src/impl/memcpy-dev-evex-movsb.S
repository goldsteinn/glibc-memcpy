#define MEMCPY	memcpy_dev_evex_movsb
#define USE_WITH_MULTIARCH_AND_LIBC
#define USE_WITH_ERMS
#define USE_MOVSB
	// #define USE_WITH_FRMS
#include "evex-vecs.h"

    // 0, 1, 3, 4
#include "core/memcpy-dev-v1.S"
