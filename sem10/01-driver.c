#include <stdio.h>

extern double sum(double a, double b);

int main() {
    printf("1 + 2 = %f\n", sum(100, 2));
}
