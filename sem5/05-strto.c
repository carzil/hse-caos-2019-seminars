#include <stdlib.h>
#include <stdio.h>
#include <errno.h>


int main(int argc, char* argv[]) {
    char* end = NULL;

    errno = 0;

    char* s = argv[1]
    long result = strtol(s, &end, 10);

    if (*s == '\0' || *end || errno) {
        printf("ooops...\n");
    } else {
        printf("res = %ld\n", result);
        printf("end = %s\n", end);
    }
    return 0;
}
