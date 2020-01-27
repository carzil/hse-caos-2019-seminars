    .text
    .global sum
sum:
    add %rsi, %rdi
    mov %rdi, %rax
    ret
