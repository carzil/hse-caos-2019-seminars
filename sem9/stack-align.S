        .section .rodata
name:
        .string "carzil"
printf_format:
        .string "Hello, %s! Your number: %ld.\n"

scanf_format:
        .string "%ld"

        .text
        .global main
main:
        pushq   %rbp
        mov     %rsp, %rbp

        # long long x;
        sub     $16, %rsp

        # scanf("%ld", &x);
        lea     scanf_format(%rip), %rdi
        # rdi = rip + scanf_format
        mov     %rsp, %rsi

        call    scanf

        # printf("...", name, x);
        lea     printf_format(%rip), %rdi
        lea     name(%rip), %rsi
        movq    (%rsp), %rdx
        call    printf

        mov     %rbp, %rsp
        pop     %rbp
        ret


// addr_inst = RIP
// addr_scanf_format
// offset = addr_scanf_format - addr_inst
// rdi = rip (addr_inst) + (addr_scanf_format - addr_inst) = addr_scanf_format
