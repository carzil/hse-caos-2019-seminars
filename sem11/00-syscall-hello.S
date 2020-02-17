#include <asm/unistd_64.h>

    .data
str:
    .string "Hello from syscall!\n"

    .text
    .global _start
_start:
    // write(1, "Hello from syscall!\n", 20)
    mov $__NR_write, %rax
    mov $228, %rdi
    lea str(%rip), %rsi
    mov $20, %rdx
    syscall

    // exit(0)
    mov $__NR_exit, %rax
    mov $0, %rdi
    syscall
