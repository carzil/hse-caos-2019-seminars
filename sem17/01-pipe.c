#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int fds[2];
    pipe(fds);

    // fds[0] -- read end
    // fds[1] -- write end

    int ret = fork();
    if (ret == 0) {
        dup2(fds[1], 1);
        printf("Hello, my name is Anastasia!\n");
        printf("Oh, my goat!\n");
        exit(0);
    }

    close(fds[1]);

    char buf[101];
    while ((ret = read(fds[0], buf, sizeof(buf) - 1)) > 0) {
        buf[ret] = '\0';
        printf("New chunk: '%s'\n", buf);
    }
    return 0;
}
