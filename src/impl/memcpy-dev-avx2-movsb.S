#define MEMCPY	memcpy_dev_avx2_movsb

#define USE_WITH_MULTIARCH_AND_LIBC
#define USE_WITH_ERMS
//#define USE_WITH_FRMS
#include "avx2-vecs.h"

#include "core/memcpy-new.S"
