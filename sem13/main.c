#include <stdio.h>

extern int sqr(int x);

int main() {
    printf("sqr(%d) = %d\n", 5, sqr(5));
    return 0;
}
