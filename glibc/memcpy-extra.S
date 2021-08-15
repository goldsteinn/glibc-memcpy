#if defined SHARED && IS_IN (libc)
ENTRY (MEMMOVE_CHK_SYMBOL (__mempcpy_chk, unaligned))
	cmp	%RDX_LP, %RCX_LP
	jb	HIDDEN_JUMPTARGET (__chk_fail)
END (MEMMOVE_CHK_SYMBOL (__mempcpy_chk, unaligned))
#endif

ENTRY (MEMPCPY_SYMBOL (__mempcpy, unaligned))
    callq   MEMCPY_SYMBOL (__memcpy, unaligned)
	add	%RDX_LP, %RAX_LP
	ret
END (MEMPCPY_SYMBOL (__mempcpy, unaligned))

#if defined SHARED && IS_IN (libc)
ENTRY (MEMMOVE_CHK_SYMBOL (__memmove_chk, unaligned))
	cmp	%RDX_LP, %RCX_LP
	jb	HIDDEN_JUMPTARGET (__chk_fail)
END (MEMMOVE_CHK_SYMBOL (__memmove_chk, unaligned))
#endif

