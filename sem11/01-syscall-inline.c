#define __NR_write 1
#define __NR_exit 60

const char HELLO[] = "Hello from shitty inline assembly!\n";

int write(int fd, const void *buf, unsigned long nbytes) {
    int res = 0;
    asm (
        "mov %1, %%rax\n"
        "mov %2, %%rdi\n"
        "mov %3, %%rsi\n"
        "mov %4, %%rdx\n"
        "syscall\n"
        "mov %%rax, %0\n"
        : "=m"(res)
        : "i"(__NR_write), "g"(fd), "g"(buf), "m"(nbytes)
        : "rax", "rdi", "rsi", "rdx"
    );

    return res;
}

void exit(int status) {
    asm (
        "mov %0, %%rax\n"
        "syscall\n"
        :
        : "i"(__NR_exit)
    );
}

unsigned long strlen(const char* s) {
    const char* pos = s;
    while (*pos != '\0') {
        pos++;
    }
    return pos - s;
}

void _start() {
    write(1, HELLO, strlen(HELLO));
    exit(0);
}
