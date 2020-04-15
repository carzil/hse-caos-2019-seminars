#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
            printf("Hello, '%s!'\n", name);
            fflush(stdout);
        }
        exit(0);
    }

    close(out_pipe[1]);
    close(in_pipe[0]);
    FILE* child_in = fdopen(in_pipe[1], "w");
    FILE* child_out = fdopen(out_pipe[0], "r");

    for (size_t i = 0; i < 3; i++) {
        fprintf(child_in, "%s\n", NAMES[i]);
    }

    fclose(child_in);

    for (;;) {
        char* line = malloc(100);
        size_t sz = 100;
        int ret = getline(&line, &sz, child_out);
        if (ret < 0) {
            break;
        }
        printf("Got line: %s", line);
        free(line);
    }

    return 0;
}
