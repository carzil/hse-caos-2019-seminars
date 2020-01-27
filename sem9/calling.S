        .section .rodata
name:
        .string "carzil"
printf_format:
        .string "Hello, %s!\n"

        .text
        .global f
f:
        push    %ebp
        movl    %esp, %ebp

        mov     0x8(%ebp), %eax // ebp + 8

        push    %eax
        push    $printf_format
        call    printf

        addl    $8, %esp
        popl    %ebp
        ret


        .global main
main:
        push    %ebp
        mov     %esp, %ebp

        push    $name // const char*
        call    f

        addl    $4, %esp
        popl    %ebp
        ret


// b = byte = 8 bit
// w = word = 16 bit
// l = long word = 32 bit
// q = quad word = 64 bit
