section .data

section .text
bits 64
default rel

global simdxmm_stencil
extern printf

simdxmm_stencil:
	shr rcx, 2
	sub rcx, 1

L1:
	vmovdqu xmm2, [rdx]
	vmovdqu xmm1, [rdx + 4]
	paddd xmm2, xmm1
	vmovdqu xmm1, [rdx + 8]
	paddd xmm2, xmm1
	vmovdqu xmm1, [rdx + 12]
	paddd xmm2, xmm1
	vmovdqu xmm1, [rdx + 16]
	paddd xmm2, xmm1
	vmovdqu xmm1, [rdx + 20]
	paddd xmm2, xmm1
	vmovdqu xmm1, [rdx + 24]
	paddd xmm2, xmm1

	vmovdqu [r8], xmm2

	add rdx, 16
	add r8, 16

	loop L1

	ret