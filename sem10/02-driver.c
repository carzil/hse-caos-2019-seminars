#include <stdio.h>

extern double hypot(double x, double y);

int main() {
    printf("%f\n", hypot(1, 1));
    printf("%f\n", hypot(3, 4));
    printf("%f\n", hypot(5, 12));
}
