#include <unistd.h>
#include <stdlib.h>


int main(int argc, char* argv[]) {
    int fd = strtol(argv[1], NULL, 0);
    write(fd, "Hello, HSE!\n", 12);
    return 0;
}
