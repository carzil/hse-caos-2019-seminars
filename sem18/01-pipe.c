#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <fcntl.h>

int fds[2];

int set_fd_non_blocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        return -1;
    }

    flags |= O_NONBLOCK;

    if (fcntl(fd, F_SETFL, flags) == -1) {
        return -1;
    }

    return 0;
}

void handler(int sig) {
    write(fds[1], &sig, sizeof(int));
}

int main() {
    if (pipe(fds) < 0) {
        perror("pipe");
        return 1;
    }

    set_fd_non_blocking(fds[1]);

    signal(SIGUSR1, handler);
    signal(SIGUSR2, handler);

    printf("%d\n", getpid());

    for (;;) {
        int sig;
        read(fds[0], &sig, sizeof(sig));
        printf("Got signal #%d\n", sig);
    }
    return 0;
}
