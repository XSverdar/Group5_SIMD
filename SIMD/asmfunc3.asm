section .text
bits 64
default rel

global simdymm_stencil

simdymm_stencil:
    xor rax, rax
    sub rcx, 6            ; Adjusts the counter accounting for invalid operations
    js end
    mov rbx, rcx

    shr rcx, 3            ; Since the input is 32-bit int and ymm is 256-bit, we divide the counter by 8 (256/32 = 8)
    jrcxz next      

    ; "L1" iterates through the operations using SIMD YMM registers based on the given process
L1:
    vmovdqu ymm2, [rdx]
    vpaddd ymm2, ymm2, [rdx + 4]
    vpaddd ymm2, ymm2, [rdx + 8]
    vpaddd ymm2, ymm2, [rdx + 12]
    vpaddd ymm2, ymm2, [rdx + 16]
    vpaddd ymm2, ymm2, [rdx + 20]
    vpaddd ymm2, ymm2, [rdx + 24]

    vmovdqu [r8], ymm2

    add rdx, 32
    add r8, 32
    add rax, 8

    loop L1

    ; "next" calculates the n < 8 remaining values that cannot be fit inside the YMM register
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
