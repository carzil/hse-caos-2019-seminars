#include <stdio.h>
#include <unistd.h>

#define WHOLE (200000)

int main() {
    setbuf(stdout, NULL);

    putchar('\a');
    usleep(WHOLE);

    for (;;) {
        putchar('\a');
        usleep(WHOLE / 4);

        for (size_t i = 0; i < 2; i++) {
            putchar('\a');
            usleep(WHOLE / 8);
        }

        putchar('\a');
        usleep(WHOLE / 4);

        for (size_t i = 0; i < 3; i++) {
            putchar('\a');
            usleep(WHOLE / 8);
        }

        putchar('\a');
        usleep(WHOLE / 4);

        putchar('\a');
        usleep(WHOLE / 8);
        putchar('\a');
        usleep(WHOLE / 4);

    }
}
