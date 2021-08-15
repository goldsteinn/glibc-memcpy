#if defined USE_MULTIARCH && IS_IN (libc)
# if VEC_SIZE == 16
ENTRY (__mempcpy_chk_erms)
	cmp	%RDX_LP, %RCX_LP
	jb	HIDDEN_JUMPTARGET (__chk_fail)
END (__mempcpy_chk_erms)

	/* Only used to measure performance of REP MOVSB.  */
ENTRY (__mempcpy_erms)
	mov	%RDI_LP, %RAX_LP
	/* Skip zero length.  */
	test	%RDX_LP, %RDX_LP
	jz	2f
	add	%RDX_LP, %RAX_LP
	jmp	L(start_movsb)
END (__mempcpy_erms)

ENTRY (__memmove_chk_erms)
	cmp	%RDX_LP, %RCX_LP
	jb	HIDDEN_JUMPTARGET (__chk_fail)
END (__memmove_chk_erms)

ENTRY (__memmove_erms)
	movq	%rdi, %rax
	/* Skip zero length.  */
	test	%RDX_LP, %RDX_LP
	jz	2f
L(start_movsb):
	mov	%RDX_LP, %RCX_LP
	cmp	%RSI_LP, %RDI_LP
	jb	1f
	/* Source == destination is less common.  */
	je	2f
	lea	(%rsi, %rcx), %RDX_LP
	cmp	%RDX_LP, %RDI_LP
	jb	L(movsb_backward)
1:
	rep	movsb
2:
	ret
L(movsb_backward):
	leaq	-1(%rdi, %rcx), %rdi
	leaq	-1(%rsi, %rcx), %rsi
	std
	rep	movsb
	cld
	ret
END (__memmove_erms)
	strong_alias (__memmove_erms, __memcpy_erms)
	strong_alias (__memmove_chk_erms, __memcpy_chk_erms)
# endif

# ifdef SHARED
ENTRY (MEMMOVE_CHK_SYMBOL (__mempcpy_chk, unaligned_erms))
	cmp	%RDX_LP, %RCX_LP
	jb	HIDDEN_JUMPTARGET (__chk_fail)
END (MEMMOVE_CHK_SYMBOL (__mempcpy_chk, unaligned_erms))
# endif

ENTRY (MEMMOVE_SYMBOL (__mempcpy, unaligned_erms))
    callq   MEMCPY_SYMBOL (__memcpy, unaligned_erms)
	add	%RDX_LP, %RAX_LP
	ret
END (MEMMOVE_SYMBOL (__mempcpy, unaligned_erms))

# ifdef SHARED
ENTRY (MEMMOVE_CHK_SYMBOL (__memmove_chk, unaligned_erms))
	cmp	%RDX_LP, %RCX_LP
	jb	HIDDEN_JUMPTARGET (__chk_fail)
END (MEMMOVE_CHK_SYMBOL (__memmove_chk, unaligned_erms))
# endif
#endif
