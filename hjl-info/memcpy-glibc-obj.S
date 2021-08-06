0000000000000800 <MEMCPY>:
 800:	48 89 f8             	mov    %rdi,%rax
 803:	48 83 fa 20          	cmp    $0x20,%rdx
 807:	72 27                	jb     830 <MEMCPY+0x30>
 809:	48 83 fa 40          	cmp    $0x40,%rdx
 80d:	0f 87 00 00 00 00    	ja     813 <MEMCPY+0x13>
 813:	62 e1 fe 28 6f 06    	vmovdqu64 (%rsi),%ymm16
 819:	62 e1 fe 28 6f 4c 16 	vmovdqu64 -0x20(%rsi,%rdx,1),%ymm17
 820:	ff 
 821:	62 e1 fe 28 7f 07    	vmovdqu64 %ymm16,(%rdi)
 827:	62 e1 fe 28 7f 4c 17 	vmovdqu64 %ymm17,-0x20(%rdi,%rdx,1)
 82e:	ff 
 82f:	c3                   	retq   
 830:	80 fa 10             	cmp    $0x10,%dl
 833:	73 17                	jae    84c <MEMCPY+0x4c>
 835:	80 fa 08             	cmp    $0x8,%dl
 838:	73 2f                	jae    869 <MEMCPY+0x69>
 83a:	80 fa 04             	cmp    $0x4,%dl
 83d:	73 3b                	jae    87a <MEMCPY+0x7a>
 83f:	80 fa 01             	cmp    $0x1,%dl
 842:	77 43                	ja     887 <MEMCPY+0x87>
 844:	72 05                	jb     84b <MEMCPY+0x4b>
 846:	0f b6 0e             	movzbl (%rsi),%ecx
 849:	88 0f                	mov    %cl,(%rdi)
 84b:	c3                   	retq   
 84c:	62 f1 fe 08 6f 06    	vmovdqu64 (%rsi),%xmm0
 852:	62 f1 fe 08 6f 4c 16 	vmovdqu64 -0x10(%rsi,%rdx,1),%xmm1
 859:	ff 
 85a:	62 f1 fe 08 7f 07    	vmovdqu64 %xmm0,(%rdi)
 860:	62 f1 fe 08 7f 4c 17 	vmovdqu64 %xmm1,-0x10(%rdi,%rdx,1)
 867:	ff 
 868:	c3                   	retq   
 869:	48 8b 4c 16 f8       	mov    -0x8(%rsi,%rdx,1),%rcx
 86e:	48 8b 36             	mov    (%rsi),%rsi
 871:	48 89 4c 17 f8       	mov    %rcx,-0x8(%rdi,%rdx,1)
 876:	48 89 37             	mov    %rsi,(%rdi)
 879:	c3                   	retq   
 87a:	8b 4c 16 fc          	mov    -0x4(%rsi,%rdx,1),%ecx
 87e:	8b 36                	mov    (%rsi),%esi
 880:	89 4c 17 fc          	mov    %ecx,-0x4(%rdi,%rdx,1)
 884:	89 37                	mov    %esi,(%rdi)
 886:	c3                   	retq   
 887:	0f b7 4c 16 fe       	movzwl -0x2(%rsi,%rdx,1),%ecx
 88c:	0f b7 36             	movzwl (%rsi),%esi
 88f:	66 89 4c 17 fe       	mov    %cx,-0x2(%rdi,%rdx,1)
 894:	66 89 37             	mov    %si,(%rdi)
 897:	c3                   	retq   
 898:	48 81 fa 00 01 00 00 	cmp    $0x100,%rdx
 89f:	0f 87 bb 00 00 00    	ja     960 <MEMCPY+0x160>
 8a5:	48 81 fa 80 00 00 00 	cmp    $0x80,%rdx
 8ac:	76 77                	jbe    925 <MEMCPY+0x125>
 8ae:	62 e1 fe 28 6f 06    	vmovdqu64 (%rsi),%ymm16
 8b4:	62 e1 fe 28 6f 4e 01 	vmovdqu64 0x20(%rsi),%ymm17
 8bb:	62 e1 fe 28 6f 56 02 	vmovdqu64 0x40(%rsi),%ymm18
 8c2:	62 e1 fe 28 6f 5e 03 	vmovdqu64 0x60(%rsi),%ymm19
 8c9:	62 e1 fe 28 6f 64 16 	vmovdqu64 -0x20(%rsi,%rdx,1),%ymm20
 8d0:	ff 
 8d1:	62 e1 fe 28 6f 6c 16 	vmovdqu64 -0x40(%rsi,%rdx,1),%ymm21
 8d8:	fe 
 8d9:	62 e1 fe 28 6f 74 16 	vmovdqu64 -0x60(%rsi,%rdx,1),%ymm22
 8e0:	fd 
 8e1:	62 e1 fe 28 6f 7c 16 	vmovdqu64 -0x80(%rsi,%rdx,1),%ymm23
 8e8:	fc 
 8e9:	62 e1 fe 28 7f 07    	vmovdqu64 %ymm16,(%rdi)
 8ef:	62 e1 fe 28 7f 4f 01 	vmovdqu64 %ymm17,0x20(%rdi)
 8f6:	62 e1 fe 28 7f 57 02 	vmovdqu64 %ymm18,0x40(%rdi)
 8fd:	62 e1 fe 28 7f 5f 03 	vmovdqu64 %ymm19,0x60(%rdi)
 904:	62 e1 fe 28 7f 64 17 	vmovdqu64 %ymm20,-0x20(%rdi,%rdx,1)
 90b:	ff 
 90c:	62 e1 fe 28 7f 6c 17 	vmovdqu64 %ymm21,-0x40(%rdi,%rdx,1)
 913:	fe 
 914:	62 e1 fe 28 7f 74 17 	vmovdqu64 %ymm22,-0x60(%rdi,%rdx,1)
 91b:	fd 
 91c:	62 e1 fe 28 7f 7c 17 	vmovdqu64 %ymm23,-0x80(%rdi,%rdx,1)
 923:	fc 
 924:	c3                   	retq   
 925:	62 e1 fe 28 6f 06    	vmovdqu64 (%rsi),%ymm16
 92b:	62 e1 fe 28 6f 4e 01 	vmovdqu64 0x20(%rsi),%ymm17
 932:	62 e1 fe 28 6f 54 16 	vmovdqu64 -0x20(%rsi,%rdx,1),%ymm18
 939:	ff 
 93a:	62 e1 fe 28 6f 5c 16 	vmovdqu64 -0x40(%rsi,%rdx,1),%ymm19
 941:	fe 
 942:	62 e1 fe 28 7f 07    	vmovdqu64 %ymm16,(%rdi)
 948:	62 e1 fe 28 7f 4f 01 	vmovdqu64 %ymm17,0x20(%rdi)
 94f:	62 e1 fe 28 7f 54 17 	vmovdqu64 %ymm18,-0x20(%rdi,%rdx,1)
 956:	ff 
 957:	62 e1 fe 28 7f 5c 17 	vmovdqu64 %ymm19,-0x40(%rdi,%rdx,1)
 95e:	fe 
 95f:	c3                   	retq   
 960:	48 39 f7             	cmp    %rsi,%rdi
 963:	0f 87 c4 00 00 00    	ja     a2d <MEMCPY+0x22d>
 969:	0f 84 00 00 00 00    	je     96f <MEMCPY+0x16f>
 96f:	62 e1 fe 28 6f 26    	vmovdqu64 (%rsi),%ymm20
 975:	62 e1 fe 28 6f 6c 16 	vmovdqu64 -0x20(%rsi,%rdx,1),%ymm21
 97c:	ff 
 97d:	62 e1 fe 28 6f 74 16 	vmovdqu64 -0x40(%rsi,%rdx,1),%ymm22
 984:	fe 
 985:	62 e1 fe 28 6f 7c 16 	vmovdqu64 -0x60(%rsi,%rdx,1),%ymm23
 98c:	fd 
 98d:	62 61 fe 28 6f 44 16 	vmovdqu64 -0x80(%rsi,%rdx,1),%ymm24
 994:	fc 
 995:	49 89 fb             	mov    %rdi,%r11
 998:	48 8d 4c 17 e0       	lea    -0x20(%rdi,%rdx,1),%rcx
 99d:	49 89 f8             	mov    %rdi,%r8
 9a0:	49 83 e0 1f          	and    $0x1f,%r8
 9a4:	49 83 e8 20          	sub    $0x20,%r8
 9a8:	4c 29 c6             	sub    %r8,%rsi
 9ab:	4c 29 c7             	sub    %r8,%rdi
 9ae:	4c 01 c2             	add    %r8,%rdx
 9b1:	66 66 2e 0f 1f 84 00 	data16 nopw %cs:0x0(%rax,%rax,1)
 9b8:	00 00 00 00 
 9bc:	0f 1f 40 00          	nopl   0x0(%rax)
 9c0:	62 e1 fe 28 6f 06    	vmovdqu64 (%rsi),%ymm16
 9c6:	62 e1 fe 28 6f 4e 01 	vmovdqu64 0x20(%rsi),%ymm17
 9cd:	62 e1 fe 28 6f 56 02 	vmovdqu64 0x40(%rsi),%ymm18
 9d4:	62 e1 fe 28 6f 5e 03 	vmovdqu64 0x60(%rsi),%ymm19
 9db:	48 83 ee 80          	sub    $0xffffffffffffff80,%rsi
 9df:	48 83 c2 80          	add    $0xffffffffffffff80,%rdx
 9e3:	62 e1 fd 28 7f 07    	vmovdqa64 %ymm16,(%rdi)
 9e9:	62 e1 fd 28 7f 4f 01 	vmovdqa64 %ymm17,0x20(%rdi)
 9f0:	62 e1 fd 28 7f 57 02 	vmovdqa64 %ymm18,0x40(%rdi)
 9f7:	62 e1 fd 28 7f 5f 03 	vmovdqa64 %ymm19,0x60(%rdi)
 9fe:	48 83 ef 80          	sub    $0xffffffffffffff80,%rdi
 a02:	48 81 fa 80 00 00 00 	cmp    $0x80,%rdx
 a09:	77 b5                	ja     9c0 <MEMCPY+0x1c0>
 a0b:	62 e1 fe 28 7f 29    	vmovdqu64 %ymm21,(%rcx)
 a11:	62 e1 fe 28 7f 71 ff 	vmovdqu64 %ymm22,-0x20(%rcx)
 a18:	62 e1 fe 28 7f 79 fe 	vmovdqu64 %ymm23,-0x40(%rcx)
 a1f:	62 61 fe 28 7f 41 fd 	vmovdqu64 %ymm24,-0x60(%rcx)
 a26:	62 c1 fe 28 7f 23    	vmovdqu64 %ymm20,(%r11)
 a2c:	c3                   	retq   
 a2d:	62 e1 fe 28 6f 26    	vmovdqu64 (%rsi),%ymm20
 a33:	62 e1 fe 28 6f 6e 01 	vmovdqu64 0x20(%rsi),%ymm21
 a3a:	62 e1 fe 28 6f 76 02 	vmovdqu64 0x40(%rsi),%ymm22
 a41:	62 e1 fe 28 6f 7e 03 	vmovdqu64 0x60(%rsi),%ymm23
 a48:	62 61 fe 28 6f 44 16 	vmovdqu64 -0x20(%rsi,%rdx,1),%ymm24
 a4f:	ff 
 a50:	4c 8d 5c 17 e0       	lea    -0x20(%rdi,%rdx,1),%r11
 a55:	48 8d 4c 16 e0       	lea    -0x20(%rsi,%rdx,1),%rcx
 a5a:	4d 89 d9             	mov    %r11,%r9
 a5d:	4d 89 d8             	mov    %r11,%r8
 a60:	49 83 e0 1f          	and    $0x1f,%r8
 a64:	4c 29 c1             	sub    %r8,%rcx
 a67:	4d 29 c1             	sub    %r8,%r9
 a6a:	4c 29 c2             	sub    %r8,%rdx
 a6d:	0f 1f 00             	nopl   (%rax)
 a70:	62 e1 fe 28 6f 01    	vmovdqu64 (%rcx),%ymm16
 a76:	62 e1 fe 28 6f 49 ff 	vmovdqu64 -0x20(%rcx),%ymm17
 a7d:	62 e1 fe 28 6f 51 fe 	vmovdqu64 -0x40(%rcx),%ymm18
 a84:	62 e1 fe 28 6f 59 fd 	vmovdqu64 -0x60(%rcx),%ymm19
 a8b:	48 83 c1 80          	add    $0xffffffffffffff80,%rcx
 a8f:	48 83 c2 80          	add    $0xffffffffffffff80,%rdx
 a93:	62 c1 fd 28 7f 01    	vmovdqa64 %ymm16,(%r9)
 a99:	62 c1 fd 28 7f 49 ff 	vmovdqa64 %ymm17,-0x20(%r9)
 aa0:	62 c1 fd 28 7f 51 fe 	vmovdqa64 %ymm18,-0x40(%r9)
 aa7:	62 c1 fd 28 7f 59 fd 	vmovdqa64 %ymm19,-0x60(%r9)
 aae:	49 83 c1 80          	add    $0xffffffffffffff80,%r9
 ab2:	48 81 fa 80 00 00 00 	cmp    $0x80,%rdx
 ab9:	77 b5                	ja     a70 <MEMCPY+0x270>
 abb:	62 e1 fe 28 7f 27    	vmovdqu64 %ymm20,(%rdi)
 ac1:	62 e1 fe 28 7f 6f 01 	vmovdqu64 %ymm21,0x20(%rdi)
 ac8:	62 e1 fe 28 7f 77 02 	vmovdqu64 %ymm22,0x40(%rdi)
 acf:	62 e1 fe 28 7f 7f 03 	vmovdqu64 %ymm23,0x60(%rdi)
 ad6:	62 41 fe 28 7f 03    	vmovdqu64 %ymm24,(%r11)
 adc:	c3                   	retq   
