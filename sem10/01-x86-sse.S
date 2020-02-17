    .text
    .global sum
sum:
    push %ebp
    mov %esp, %ebp

    movsd 8(%ebp), %xmm0
    addsd 16(%ebp), %xmm0

    sub $8, %esp
    movsd %xmm0, (%esp)
    fldl (%esp)
    add $8, %esp

    mov %ebp, %esp
    pop %ebp
    ret
