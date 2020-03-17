#define _GNU_SOURCE
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>


int main(int argc, char* argv[]) {
    int fd = open(argv[1], O_WRONLY);
    off_t len = strtoll(argv[2], NULL, 0);
    // ftruncate(fd, len);
    fallocate(fd, 0, 0, len);
    return 0;
}
