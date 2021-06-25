#include "memory-util.h"
#include "common.h"
#include "error-util.h"

void *
_safe_calloc(uint64_t n, uint64_t sz, const char * const fn, int32_t ln) {
    void * p = calloc(n, sz);
    if (UNLIKELY(p == NULL)) {
        _errdie(fn, ln, errno, NULL);
    }
    return p;
}

void *
_safe_malloc(uint64_t sz, const char * const fn, int32_t ln) {
    void * p = malloc(sz);
    if (UNLIKELY(p == NULL)) {
        _errdie(fn, ln, errno, NULL);
    }
    return p;
}
void *
_safe_realloc(void * p, uint64_t sz, const char * const fn, int32_t ln) {
    void * newp = realloc(p, sz);
    if (UNLIKELY(p == NULL)) {
        _errdie(fn, ln, errno, NULL);
    }
    return newp;
}

void *
_safe_mmap(void *             addr,
           uint64_t           sz,
           int32_t            prot_flags,
           int32_t            mmap_flags,
           int32_t            fd,
           int32_t            offset,
           const char * const fn,
           const int32_t      ln) {
    void * p = mmap(addr, sz, prot_flags, mmap_flags, fd, offset);
    if (UNLIKELY(p == NULL)) {
        _errdie(fn, ln, errno, NULL);
    }
    return p;
}

void
_safe_munmap(void *             addr,
             uint64_t           sz,
             const char * const fn,
             const int32_t      ln) {
    if (UNLIKELY(munmap(addr, sz))) {
        _errdie(fn, ln, errno, NULL);
    }
}

void
_safe_mprotect(void *             addr,
               uint64_t           sz,
               int32_t            prot_flags,
               const char * const fn,
               const int32_t      ln) {
    if (UNLIKELY(mprotect(addr, sz, prot_flags))) {
        _errdie(fn, ln, errno, NULL);
    }
}

void
_safe_free(void * addr, const char * const fn, const int32_t ln) {
    if (UNLIKELY(addr == NULL)) {
        _die(fn, ln, NULL);
    }
    free(addr);
}
