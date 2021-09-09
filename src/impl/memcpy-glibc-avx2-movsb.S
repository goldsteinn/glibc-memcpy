#define MEMCPY	memcpy_glibc_avx2_movsb


#define USE_WITH_MULTIARCH_AND_LIBC
//#define USE_WITH_ERMS

#include "avx2-vecs.h"

#define NO_MOVSB
    #include "core/memcpy-dev.S"
    //#include "core/memcpy-glibc.S"
