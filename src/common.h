#ifndef _COMMON_H_
#define _COMMON_H_

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALWAYS_INLINE inline __attribute__((always_inline))
#define NEVER_INLINE  __attribute__((noinline))

#define ALIGNED(x) __attribute__((aligned(x)))
#define BENCH_FUNC __attribute__((noinline, noclone, aligned(4096)))
#define EXIT_FUNC  __attribute__((noinline, noclone, cold, noreturn))

#define LIKELY(x)   __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect((x), 0)

#define IMPOSSIBLE(x)                                                          \
    if (x) {                                                                   \
        __builtin_unreachable();                                               \
    }


#endif
