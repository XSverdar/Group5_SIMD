section .data

section .text
bits 64
default rel

global nonsimd_stencil
extern printf

nonsimd_stencil:
    ; Function signature: void nonsimd_stencil(size_t n, int32_t* x, int32_t* y)
    ; Arguments:
    ;   n:  rdi
    ;   x:  rsi
    ;   y:  rdx

    ; Prologue
    push rbp
    mov rbp, rsp
    push rbx

    ; Set up loop counter (rcx) and index (r8)
    xor rcx, rcx
    xor r8, r8

    ; Main loop
main_loop:
    cmp rcx, rdi                  ; if i >= n, end loop
    jge end_loop

    ; Check boundary conditions
    cmp rcx, 3
    jl next_iteration             ; if i < 3, skip this iteration

    mov rax, rdi
    sub rax, 3
    cmp rcx, rax
    jge next_iteration            ; if i > n - 4, skip this iteration

    ; Perform stencil computation
    mov eax, [rsi + 4*rcx - 12]   ; eax = x[i-3]
    add eax, [rsi + 4*rcx - 8]    ; eax += x[i-2]
    add eax, [rsi + 4*rcx - 4]    ; eax += x[i-1]
    add eax, [rsi + 4*rcx]        ; eax += x[i]
    add eax, [rsi + 4*rcx + 4]    ; eax += x[i+1]
    add eax, [rsi + 4*rcx + 8]    ; eax += x[i+2]
    add eax, [rsi + 4*rcx + 12]   ; eax += x[i+3]

    ; Store result in y
    mov [rdx + 4*r8], eax

    ; Increment index (r8)
    inc r8

next_iteration:
    ; Increment loop counter (rcx)
    inc rcx
    jmp main_loop

end_loop:
    ; Epilogue
    pop rbx
    pop rbp
    ret
