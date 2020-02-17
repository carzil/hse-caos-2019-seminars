    .text
    .global hypot
hypot:
    push %rbp
    mov %rsp, %rbp

    // %xmm0 = x
    // %xmm1 = y
    mulsd %xmm0, %xmm0
    mulsd %xmm1, %xmm1
    addsd %xmm1, %xmm0
    sqrtsd %xmm0, %xmm0  // второй аргумент — куда сохранять


    mov %rbp, %rsp
    pop %rbp
    ret
