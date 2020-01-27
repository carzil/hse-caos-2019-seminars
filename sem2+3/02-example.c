#include <stdio.h>


int main() {
    int ch = EOF;

    while ((ch = fgetc(stdin)) != EOF) {
        putchar(ch);
    }

    putchar('\a'); // smth little extra...

    return 0;
}
