#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();
    if (pid == 0) {
        return 1;
    } else {
        wait(NULL);
    }
    return 0;
}
