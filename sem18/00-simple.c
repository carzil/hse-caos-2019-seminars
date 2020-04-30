#include <signal.h>
#include <stdio.h>
#include <unistd.h>

sig_atomic_t count = 0;

void handler(int sig) {
    count++;
}

int main() {
    signal(SIGUSR1, handler);

    printf("%d\n", getpid());

    for (;;) {
        pause();
        printf("Count: %d\n", count);
    }
    return 0;
}
