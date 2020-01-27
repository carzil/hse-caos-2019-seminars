#include <stdio.h>
#include <unistd.h>

int main() {
    setbuf(stdout, NULL);

    const char chars[] = {'|', '/', '-', '\\'};
    size_t pos = 0;
    for (;;) {
        printf("\r[%c] Loading...", chars[pos]);
        pos += 1;
        pos %= sizeof(chars) / sizeof(chars[0]);
        usleep(100000);
    }
}

