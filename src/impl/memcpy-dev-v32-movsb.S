#define MEMCPY	memcpy_dev_v32_movsb
#include "evex-vecs.h"

#define USE_WITH_MULTIARCH_AND_LIBC
#define USE_WITH_ERMS

#define OPTIMIZE_32	1
#define OPTIMIZE_1_3	1
#define OPTIMIZE_128	3
#define OPTIMIZE_MOVSB	0
#define OPTIMIZE_8x_FORWARD	1
#define OPTIMIZE_8x_BACKWARD	1

    
#include "core/memcpy-dev-v1.S"
