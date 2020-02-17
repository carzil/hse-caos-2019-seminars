	.file	"01-syscall-inline.c"
	.text
	.globl	HELLO
	.section	.rodata
	.align 32
	.type	HELLO, @object
	.size	HELLO, 36
HELLO:
	.string	"Hello from shitty inline assembly!\n"
	.text
	.globl	write
	.type	write, @function
write:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movl	%edi, -20(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%rdx, -40(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$0, -12(%rbp)
#APP
# 8 "01-syscall-inline.c" 1
	mov $1, %rax
mov -20(%rbp), %rdi
mov -32(%rbp), %rsi
mov -40(%rbp), %rdx
syscall
mov %rax, -12(%rbp)

# 0 "" 2
#NO_APP
	movl	-12(%rbp), %eax
	movq	-8(%rbp), %rcx
	xorq	%fs:40, %rcx
	je	.L3
	call	__stack_chk_fail@PLT
.L3:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	write, .-write
	.globl	exit
	.type	exit, @function
exit:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -4(%rbp)
#APP
# 24 "01-syscall-inline.c" 1
	mov $60, %rax
syscall

# 0 "" 2
#NO_APP
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	exit, .-exit
	.globl	strlen
	.type	strlen, @function
strlen:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, -8(%rbp)
	jmp	.L6
.L7:
	addq	$1, -8(%rbp)
.L6:
	movq	-8(%rbp), %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L7
	movq	-8(%rbp), %rax
	subq	-24(%rbp), %rax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	strlen, .-strlen
	.globl	_start
	.type	_start, @function
_start:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$35, %edx
	leaq	HELLO(%rip), %rsi
	movl	$1, %edi
	call	write
	movl	$0, %edi
	call	exit
	.cfi_endproc
.LFE3:
	.size	_start, .-_start
	.ident	"GCC: (GNU) 9.2.0"
	.section	.note.GNU-stack,"",@progbits
