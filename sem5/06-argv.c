#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>


int main(int argc, char* argv[]) {
    char name[] = "AESC <3";
    memcpy(argv[0], name, sizeof(name));
    for (;;);
    return 0;
}
