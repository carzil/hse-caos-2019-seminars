#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

void output(int i) {
    printf("%d\n", i + 2);
}


int main(int argc, char* argv[]) {
    // int fd = open("/proc/self/maps", O_RDONLY);
    // char buf[8192];
    // int ret = read(fd, buf, sizeof(buf));

    // if (ret > 0) {
    //     write(1, buf, ret);
    // }

    for (int i = 0; i < 10; i++) {
        output(i);
    }
}
