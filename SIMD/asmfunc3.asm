section .text
bits 64
default rel

global simdymm_stencil

simdymm_stencil:
    push rbp
    mov rbp, rsp

	mov rdi, rcx                ; rdi = n
    mov rsi, rdx                ; rsi = x
    mov rdx, r8                 ; rdx = y

    xor r8, r8                  ; i
	xor r9, r9  			    ; index


loop:
    cmp r8, rdi
    jge done

    ; Check if (i - 3 >= 0) and ((i + 3) <= (n - 1))
    mov rax, r8
    sub rax, 3
    js next

    mov rbx, rdi
    sub rbx, 4
    cmp r8, rbx
    jg next

    ; Loat 8 elements into YMM register
    vmovdqu ymm0, [rsi + (rax * 4)]
    vmovdqu ymm1, [rsi + (rax * 4) + 32]

    vpaddd ymm0, ymm0, ymm1

    ; Horizontal sum
    vextracti128 xmm1, ymm0, 1
    paddd xmm0, xmm1            ; Add lower 128 bits to higher 128 bits
    phaddd xmm0, xmm0
    phaddd xmm0, xmm0

    movd [rdx + r9 * 4], xmm0
    inc r9

next:
    inc r8
    jmp loop
    
end:
    pop rbp
    ret