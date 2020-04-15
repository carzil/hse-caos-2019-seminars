#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

const char* NAMES[] = {
    "carzil\n",
    "blackav\n",
    "test\n",
};

int main() {
    int in_pipe[2];
    int out_pipe[2];
    pipe(in_pipe);
    pipe(out_pipe);

    int ret = fork();
    if (ret == 0) {
        dup2(out_pipe[1], 1);
        dup2(in_pipe[0], 0);

        close(out_pipe[0]);
        close(in_pipe[1]);

        close(out_pipe[1]);
        close(in_pipe[0]);

        char name[100];
        while (scanf("%100s", name) == 1) {
            printf("Hello, %s!\n", name);
            fflush(stdout);
        }
        exit(0);
    }

    close(out_pipe[1]);
    close(in_pipe[0]);

    for (size_t i = 0; i < 3; i++) {
        write(in_pipe[1], NAMES[i], strlen(NAMES[i]));
    }

    close(in_pipe[1]);

    char buf[101];
    ret = -1;
    while ((ret = read(out_pipe[0], buf, sizeof(buf) - 1)) > 0) {
        buf[ret] = '\0';
        printf("Got: %s", buf);
    }

    close(out_pipe[0]);

    wait(NULL);

    return 0;
}
