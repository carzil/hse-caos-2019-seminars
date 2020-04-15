#include <stdio.h>
#include <unistd.h>

int main() {
    int fds[2];
    pipe(fds);

    size_t count = 0;
    for (;;) {
        write(fds[1], "1", 1);
        count++;
        printf("Wrote %zu bytes\n", count);
    }

    return 0;
}
