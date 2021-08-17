#ifndef _ASM_COMMON_H_
#define _ASM_COMMON_H_

#define L(name) .L##name

#ifndef ALIGN_BASE
#define ALIGN_BASE 512
#endif

#ifndef ALIGN_ENTRY
#define ALIGN_ENTRY 64
#endif
#define NAME_LABEL(name) name##:
#define ENTRY_END(name)                                                        \
    .globl name;                                                               \
    .type  name, @function;                                                    \
    NAME_LABEL(name)                                                           \
    NOP4

#if ALIGN_ENTRY == 0 || ALIGN_ENTRY == ALIGN_BASE || ALIGN_ENTRY == 64
#define ENTRY(name)                                                            \
    .align 4096;                                                               \
    nop;                                                                       \
    .align ALIGN_BASE;                                                         \
    ENTRY_END(name)
#else
#if ALIGN_ENTRY == 48
#define ENTRY(name)                                                            \
    .align 4096;                                                               \
    nop;                                                                       \
    .align ALIGN_BASE;                                                         \
    nop;                                                                       \
    .align 32;                                                                 \
    nop;                                                                       \
    .align 16;                                                                 \
    ENTRY_END(name)

#else

#define ENTRY(name)                                                            \
    .align 4096;                                                               \
    nop;                                                                       \
    .align ALIGN_BASE;                                                         \
    nop;                                                                       \
    .align ALIGN_ENTRY;                                                        \
    ENTRY_END(name)
#endif
#endif


#define END(name) .size name, .- name

#define ZERO_UPPER_VEC_REGISTERS_RETURN VZEROUPPER_RETURN
#define VZEROUPPER_RETURN                                                      \
    VZEROUPPER;                                                                \
    ret
#define RSI_LP rsi
#define R8_LP  r8
#define RDX_LP rdx
#define RCX_LP rcx

#include "nops.h"

#endif
