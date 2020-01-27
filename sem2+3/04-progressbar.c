#include <stdio.h>
#include <unistd.h>

float update_percent(float old) {
    usleep(10000);
    return old + 0.2;
}

enum {
    PRECISION = 5,
};

int main() {
    setbuf(stdout, NULL);

    size_t pos = 0;
    float percent = 0;
    while (percent < 100) {
        printf("[");
        for (size_t i = 0; i < (100 / PRECISION); i++) {
            if (percent / PRECISION > i) {
                putchar('#');
            } else {
                putchar('.');
            }
        }
        putchar(']');
        printf(" %.02f%%\n", percent);
        percent = update_percent(percent);
    }

    printf("\n");
}
