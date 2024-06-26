section .data

section .text
bits 64
default rel

global simdxmm_stencil
extern printf

simdxmm_stencil:
	xor rax, rax
	sub rcx, 6		; Adjusts the counter accounting for invalid operations
	js end
	mov rbx, rcx

	shr rcx, 2		; Since the input is 32-bit int and xmm is 128-bit, we divide the counter by 4 (128/32 = 4)
	jrcxz next	

	; "L1"" iterates through the operations using SIMD XMM registers based on the given process
L1:
	vmovdqu xmm1, [rdx]
	vpaddd xmm1, xmm1, [rdx + 4]
	vpaddd xmm1, xmm1, [rdx + 8]
	vpaddd xmm1, xmm1, [rdx + 12]
	vpaddd xmm1, xmm1, [rdx + 16]
	vpaddd xmm1, xmm1, [rdx + 20]
	vpaddd xmm1, xmm1, [rdx + 24]

	vmovdqu [r8], xmm1

	add rdx, 16
	add r8, 16
	add rax, 4

	loop L1

	; "next" calculates the n < 4 remaining values that cannot be fit inside the XMM register
next:
	sub rbx, rax
	mov rcx, rbx
	jrcxz end

	; "L2" iterates through the remaining operations individually
L2:
	mov eax, [rdx]
	add eax, [rdx + 4]
	add eax, [rdx + 8]
	add eax, [rdx + 12]
	add eax, [rdx + 16]
	add eax, [rdx + 20]
	add eax, [rdx + 24]

	mov [r8], eax

	add rdx, 4
	add r8, 4
	
	loop L2

end:
	ret