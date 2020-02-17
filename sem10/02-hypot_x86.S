    .text
    .global hypot
hypot:
    push %ebp
    mov %esp, %ebp

    fldl 8(%ebp)         // st(0) = x
    fmul 8(%ebp)         // st(0) = x^2
    fldl 16(%ebp)        // st(0) = y, st(1) = x^2
    fmul %st(0), %st(0)  // st(0) = y^2, st(1) = x^2
    faddp                // st(0) = x^2 + y^2
    fsqrt                // st(0) = sqrt(x^2 + y^2)

    mov %ebp, %esp
    pop %ebp
    ret
