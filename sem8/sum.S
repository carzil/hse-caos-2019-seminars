    .text
    .global main
main:
    // prologue
    push %rbp
    push %rbx
    mov %rsp, %rbp

    xor %rbx, %rbx

loop:
    call readi64
    jc end
    add %rax, %rbx
    jmp loop

end:
    mov %rbx, %rdi
    call writei64
    call nl

    mov $0, %rax

    mov %rbp, %rsp
    pop %rbx
    pop %rbp
    ret
