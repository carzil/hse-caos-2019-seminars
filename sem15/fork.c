#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {
    pid_t pid = fork();

    int in_fd = open("in.txt", O_RDONLY);
    if (in_fd == -1) {
        perror("open");
        return 1;
    }

    int out_fd = open("out.txt", O_WRONLY | O_CREAT, 0666);
    if (out_fd == -1) {
        perror("open");
        return 1;
    }

    if (pid == -1) {
        perror("fork");
        return 1;
    } else if (pid == 0) {
        // execl("./test.py", "./test.py", "test", "abc", NULL);

        // const char* envp[] = {"TEST=test", "NETEST=test???", NULL};
        // execle("./test.py", "./test.py", "test", "abc", NULL, envp);

        // char* argv[] = {"./test.py", "test", "abc", NULL};
        // execvp("test.py", argv);

        dup2(in_fd, 0);
        dup2(out_fd, 1);
        dup2(out_fd, 2);
        close(in_fd);
        close(out_fd);

        char* argv[] = {"./test.py", "test", "abc", NULL};
        char* envp[] = {"TEST=test", "NETEST=test???", NULL};
        execve("test.py", argv, envp);

        return 31;
    } else {
        close(in_fd);
        close(out_fd);
        int status = 0;
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid");
            return 1;
        }

        if (WIFEXITED(status)) {
            printf("exited, code = %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("signaled, signal = %d\n", WTERMSIG(status));
        } else {
            // пока не бывает
        }
    }
}
