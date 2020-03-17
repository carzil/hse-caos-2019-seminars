#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    printf("%d\n", getpid());
    for (;;) {
        void* addr = mmap(NULL, 4096, PROT_WRITE | PROT_READ, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
        if (addr == MAP_FAILED) {
            abort();
        }
        *(char*)addr = 'c';
        usleep(10000);
    }
    return 0;
}
