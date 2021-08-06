0000000000000800 <MEMCPY>:
 800:	48 89 f8             	mov    %rdi,%rax
 803:	48 83 fa 20          	cmp    $0x20,%rdx
 807:	0f 82 93 00 00 00    	jb     8a0 <MEMCPY+0xa0>
 80d:	62 e1 fe 28 6f 06    	vmovdqu64 (%rsi),%ymm16
 813:	48 83 fa 40          	cmp    $0x40,%rdx
 817:	0f 87 a3 00 00 00    	ja     8c0 <MEMCPY+0xc0>
 81d:	62 61 fe 28 6f 7c 16 	vmovdqu64 -0x20(%rsi,%rdx,1),%ymm31
 824:	ff 
 825:	62 e1 fe 28 7f 07    	vmovdqu64 %ymm16,(%rdi)
 82b:	62 61 fe 28 7f 7c 17 	vmovdqu64 %ymm31,-0x20(%rdi,%rdx,1)
 832:	ff 
 833:	c3                   	retq   
 834:	66 66 2e 0f 1f 84 00 	data16 nopw %cs:0x0(%rax,%rax,1)
 83b:	00 00 00 00 
 83f:	90                   	nop
 840:	83 fa 04             	cmp    $0x4,%edx
 843:	73 2b                	jae    870 <MEMCPY+0x70>
 845:	83 fa 01             	cmp    $0x1,%edx
 848:	74 13                	je     85d <MEMCPY+0x5d>
 84a:	72 10                	jb     85c <MEMCPY+0x5c>
 84c:	0f b7 0e             	movzwl (%rsi),%ecx
 84f:	0f b6 74 16 ff       	movzbl -0x1(%rsi,%rdx,1),%esi
 854:	66 89 0f             	mov    %cx,(%rdi)
 857:	40 88 74 17 ff       	mov    %sil,-0x1(%rdi,%rdx,1)
 85c:	c3                   	retq   
 85d:	0f b6 0e             	movzbl (%rsi),%ecx
 860:	88 0f                	mov    %cl,(%rdi)
 862:	c3                   	retq   
 863:	66 66 2e 0f 1f 84 00 	data16 nopw %cs:0x0(%rax,%rax,1)
 86a:	00 00 00 00 
 86e:	66 90                	xchg   %ax,%ax
 870:	8b 0e                	mov    (%rsi),%ecx
 872:	8b 74 16 fc          	mov    -0x4(%rsi,%rdx,1),%esi
 876:	89 0f                	mov    %ecx,(%rdi)
 878:	89 74 17 fc          	mov    %esi,-0x4(%rdi,%rdx,1)
 87c:	c3                   	retq   
 87d:	0f 1f 00             	nopl   (%rax)
 880:	48 8b 0e             	mov    (%rsi),%rcx
 883:	48 8b 74 16 f8       	mov    -0x8(%rsi,%rdx,1),%rsi
 888:	48 89 0f             	mov    %rcx,(%rdi)
 88b:	48 89 74 17 f8       	mov    %rsi,-0x8(%rdi,%rdx,1)
 890:	c3                   	retq   
 891:	66 66 2e 0f 1f 84 00 	data16 nopw %cs:0x0(%rax,%rax,1)
 898:	00 00 00 00 
 89c:	0f 1f 40 00          	nopl   0x0(%rax)
 8a0:	83 fa 08             	cmp    $0x8,%edx
 8a3:	72 9b                	jb     840 <MEMCPY+0x40>
 8a5:	83 fa 10             	cmp    $0x10,%edx
 8a8:	72 d6                	jb     880 <MEMCPY+0x80>
 8aa:	c5 fa 6f 06          	vmovdqu (%rsi),%xmm0
 8ae:	c5 fa 6f 4c 16 f0    	vmovdqu -0x10(%rsi,%rdx,1),%xmm1
 8b4:	c5 fa 7f 07          	vmovdqu %xmm0,(%rdi)
 8b8:	c5 fa 7f 4c 17 f0    	vmovdqu %xmm1,-0x10(%rdi,%rdx,1)
 8be:	c3                   	retq   
 8bf:	90                   	nop
 8c0:	62 e1 fe 28 6f 4e 01 	vmovdqu64 0x20(%rsi),%ymm17
 8c7:	62 61 fe 28 6f 7c 16 	vmovdqu64 -0x20(%rsi,%rdx,1),%ymm31
 8ce:	ff 
 8cf:	62 61 fe 28 6f 74 16 	vmovdqu64 -0x40(%rsi,%rdx,1),%ymm30
 8d6:	fe 
 8d7:	48 81 fa 00 01 00 00 	cmp    $0x100,%rdx
 8de:	0f 87 ac 00 00 00    	ja     990 <MEMCPY+0x190>
 8e4:	81 fa 80 00 00 00    	cmp    $0x80,%edx
 8ea:	76 64                	jbe    950 <MEMCPY+0x150>
 8ec:	62 e1 fe 28 6f 56 02 	vmovdqu64 0x40(%rsi),%ymm18
 8f3:	62 e1 fe 28 6f 5e 03 	vmovdqu64 0x60(%rsi),%ymm19
 8fa:	62 61 fe 28 6f 6c 16 	vmovdqu64 -0x60(%rsi,%rdx,1),%ymm29
 901:	fd 
 902:	62 61 fe 28 6f 64 16 	vmovdqu64 -0x80(%rsi,%rdx,1),%ymm28
 909:	fc 
 90a:	62 e1 fe 28 7f 07    	vmovdqu64 %ymm16,(%rdi)
 910:	62 e1 fe 28 7f 4f 01 	vmovdqu64 %ymm17,0x20(%rdi)
 917:	62 e1 fe 28 7f 57 02 	vmovdqu64 %ymm18,0x40(%rdi)
 91e:	62 e1 fe 28 7f 5f 03 	vmovdqu64 %ymm19,0x60(%rdi)
 925:	62 61 fe 28 7f 7c 17 	vmovdqu64 %ymm31,-0x20(%rdi,%rdx,1)
 92c:	ff 
 92d:	62 61 fe 28 7f 74 17 	vmovdqu64 %ymm30,-0x40(%rdi,%rdx,1)
 934:	fe 
 935:	62 61 fe 28 7f 6c 17 	vmovdqu64 %ymm29,-0x60(%rdi,%rdx,1)
 93c:	fd 
 93d:	62 61 fe 28 7f 64 17 	vmovdqu64 %ymm28,-0x80(%rdi,%rdx,1)
 944:	fc 
 945:	c3                   	retq   
 946:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 94d:	00 00 00 
 950:	62 e1 fe 28 7f 07    	vmovdqu64 %ymm16,(%rdi)
 956:	62 e1 fe 28 7f 4f 01 	vmovdqu64 %ymm17,0x20(%rdi)
 95d:	62 61 fe 28 7f 7c 17 	vmovdqu64 %ymm31,-0x20(%rdi,%rdx,1)
 964:	ff 
 965:	62 61 fe 28 7f 74 17 	vmovdqu64 %ymm30,-0x40(%rdi,%rdx,1)
 96c:	fe 
 96d:	c3                   	retq   
 96e:	66 66 66 0f 1f 84 00 	data16 data16 nopw 0x0(%rax,%rax,1)
 975:	00 00 00 00 
 979:	66 66 66 0f 1f 84 00 	data16 data16 nopw 0x0(%rax,%rax,1)
 980:	00 00 00 00 
 984:	66 66 0f 1f 84 00 00 	data16 nopw 0x0(%rax,%rax,1)
 98b:	00 00 00 
 98e:	66 90                	xchg   %ax,%ax
 990:	48 3b 15 00 00 00 00 	cmp    0x0(%rip),%rdx        # 997 <MEMCPY+0x197>
 997:	0f 87 53 01 00 00    	ja     af0 <MEMCPY+0x2f0>
 99d:	0f 1f 00             	nopl   (%rax)
 9a0:	48 39 f7             	cmp    %rsi,%rdi
 9a3:	0f 87 b7 00 00 00    	ja     a60 <MEMCPY+0x260>
 9a9:	74 c2                	je     96d <MEMCPY+0x16d>
 9ab:	62 61 fe 28 6f 6c 16 	vmovdqu64 -0x60(%rsi,%rdx,1),%ymm29
 9b2:	fd 
 9b3:	62 61 fe 28 6f 64 16 	vmovdqu64 -0x80(%rsi,%rdx,1),%ymm28
 9ba:	fc 
 9bb:	48 29 fe             	sub    %rdi,%rsi
 9be:	48 8d 54 17 80       	lea    -0x80(%rdi,%rdx,1),%rdx
 9c3:	48 83 cf 3f          	or     $0x3f,%rdi
 9c7:	48 8d 74 37 01       	lea    0x1(%rdi,%rsi,1),%rsi
 9cc:	48 ff c7             	inc    %rdi
 9cf:	90                   	nop
 9d0:	62 e1 fe 28 6f 26    	vmovdqu64 (%rsi),%ymm20
 9d6:	62 e1 fe 28 6f 6e 01 	vmovdqu64 0x20(%rsi),%ymm21
 9dd:	62 e1 fe 28 6f 76 02 	vmovdqu64 0x40(%rsi),%ymm22
 9e4:	62 e1 fe 28 6f 7e 03 	vmovdqu64 0x60(%rsi),%ymm23
 9eb:	48 83 ee 80          	sub    $0xffffffffffffff80,%rsi
 9ef:	62 e1 fd 28 7f 27    	vmovdqa64 %ymm20,(%rdi)
 9f5:	62 e1 fd 28 7f 6f 01 	vmovdqa64 %ymm21,0x20(%rdi)
 9fc:	62 e1 fd 28 7f 77 02 	vmovdqa64 %ymm22,0x40(%rdi)
 a03:	62 e1 fd 28 7f 7f 03 	vmovdqa64 %ymm23,0x60(%rdi)
 a0a:	48 83 ef 80          	sub    $0xffffffffffffff80,%rdi
 a0e:	48 39 fa             	cmp    %rdi,%rdx
 a11:	77 bd                	ja     9d0 <MEMCPY+0x1d0>
 a13:	62 61 fe 28 7f 22    	vmovdqu64 %ymm28,(%rdx)
 a19:	62 61 fe 28 7f 6a 01 	vmovdqu64 %ymm29,0x20(%rdx)
 a20:	62 61 fe 28 7f 72 02 	vmovdqu64 %ymm30,0x40(%rdx)
 a27:	62 61 fe 28 7f 7a 03 	vmovdqu64 %ymm31,0x60(%rdx)
 a2e:	62 e1 fe 28 7f 00    	vmovdqu64 %ymm16,(%rax)
 a34:	62 e1 fe 28 7f 48 01 	vmovdqu64 %ymm17,0x20(%rax)
 a3b:	c3                   	retq   
 a3c:	66 66 66 0f 1f 84 00 	data16 data16 nopw 0x0(%rax,%rax,1)
 a43:	00 00 00 00 
 a47:	66 66 66 0f 1f 84 00 	data16 data16 nopw 0x0(%rax,%rax,1)
 a4e:	00 00 00 00 
 a52:	66 66 0f 1f 84 00 00 	data16 nopw 0x0(%rax,%rax,1)
 a59:	00 00 00 
 a5c:	0f 1f 40 00          	nopl   0x0(%rax)
 a60:	62 e1 fe 28 6f 56 02 	vmovdqu64 0x40(%rsi),%ymm18
 a67:	62 e1 fe 28 6f 5e 03 	vmovdqu64 0x60(%rsi),%ymm19
 a6e:	48 29 fe             	sub    %rdi,%rsi
 a71:	48 8d bc 17 7f ff ff 	lea    -0x81(%rdi,%rdx,1),%rdi
 a78:	ff 
 a79:	48 83 e7 c0          	and    $0xffffffffffffffc0,%rdi
 a7d:	48 01 fe             	add    %rdi,%rsi
 a80:	62 e1 fe 28 6f 66 03 	vmovdqu64 0x60(%rsi),%ymm20
 a87:	62 e1 fe 28 6f 6e 02 	vmovdqu64 0x40(%rsi),%ymm21
 a8e:	62 e1 fe 28 6f 76 01 	vmovdqu64 0x20(%rsi),%ymm22
 a95:	62 e1 fe 28 6f 3e    	vmovdqu64 (%rsi),%ymm23
 a9b:	48 83 c6 80          	add    $0xffffffffffffff80,%rsi
 a9f:	62 e1 fd 28 7f 67 03 	vmovdqa64 %ymm20,0x60(%rdi)
 aa6:	62 e1 fd 28 7f 6f 02 	vmovdqa64 %ymm21,0x40(%rdi)
 aad:	62 e1 fd 28 7f 77 01 	vmovdqa64 %ymm22,0x20(%rdi)
 ab4:	62 e1 fd 28 7f 3f    	vmovdqa64 %ymm23,(%rdi)
 aba:	48 83 c7 80          	add    $0xffffffffffffff80,%rdi
 abe:	48 39 f8             	cmp    %rdi,%rax
 ac1:	72 bd                	jb     a80 <MEMCPY+0x280>
 ac3:	62 e1 fe 28 7f 58 03 	vmovdqu64 %ymm19,0x60(%rax)
 aca:	62 e1 fe 28 7f 50 02 	vmovdqu64 %ymm18,0x40(%rax)
 ad1:	62 e1 fe 28 7f 48 01 	vmovdqu64 %ymm17,0x20(%rax)
 ad8:	62 e1 fe 28 7f 00    	vmovdqu64 %ymm16,(%rax)
 ade:	62 61 fe 28 7f 74 10 	vmovdqu64 %ymm30,-0x40(%rax,%rdx,1)
 ae5:	fe 
 ae6:	62 61 fe 28 7f 7c 10 	vmovdqu64 %ymm31,-0x20(%rax,%rdx,1)
 aed:	ff 
 aee:	c3                   	retq   
 aef:	90                   	nop
 af0:	48 3b 15 00 00 00 00 	cmp    0x0(%rip),%rdx        # af7 <MEMCPY+0x2f7>
 af7:	73 4a                	jae    b43 <MEMCPY+0x343>
 af9:	48 89 f9             	mov    %rdi,%rcx
 afc:	48 29 f1             	sub    %rsi,%rcx
 aff:	48 39 d1             	cmp    %rdx,%rcx
 b02:	0f 82 58 ff ff ff    	jb     a60 <MEMCPY+0x260>
 b08:	48 29 fe             	sub    %rdi,%rsi
 b0b:	48 8d 4c 17 fe       	lea    -0x2(%rdi,%rdx,1),%rcx
 b10:	48 83 cf 3f          	or     $0x3f,%rdi
 b14:	48 8d 74 37 01       	lea    0x1(%rdi,%rsi,1),%rsi
 b19:	48 29 f9             	sub    %rdi,%rcx
 b1c:	48 83 e1 c0          	and    $0xffffffffffffffc0,%rcx
 b20:	48 ff c7             	inc    %rdi
 b23:	f3 a4                	rep movsb %ds:(%rsi),%es:(%rdi)
 b25:	62 e1 fe 28 7f 00    	vmovdqu64 %ymm16,(%rax)
 b2b:	62 e1 fe 28 7f 48 01 	vmovdqu64 %ymm17,0x20(%rax)
 b32:	62 61 fe 28 7f 74 10 	vmovdqu64 %ymm30,-0x40(%rax,%rdx,1)
 b39:	fe 
 b3a:	62 61 fe 28 7f 7c 10 	vmovdqu64 %ymm31,-0x20(%rax,%rdx,1)
 b41:	ff 
 b42:	c3                   	retq   
 b43:	48 3b 15 00 00 00 00 	cmp    0x0(%rip),%rdx        # b4a <MEMCPY+0x34a>
 b4a:	0f 82 40 fe ff ff    	jb     990 <MEMCPY+0x190>
 b50:	c3                   	retq   
 b51:	8b 02                	mov    (%rdx),%eax
