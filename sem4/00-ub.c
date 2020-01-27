#include <stdio.h>

int foo(float* f, int* i) {
    *i = 1;
    *f = 0.f;
    return *i;
}

int main() {
    int x = 0;

    printf("%d\n", x);
    x = foo((float*) &x, &x);
    printf("%d\n", x);

    return 0;
}
