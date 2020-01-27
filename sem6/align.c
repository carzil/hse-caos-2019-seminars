#include <stdio.h>

int main() {
    printf("_Alignof(char) = %d\n", _Alignof(char));
    printf("_Alignof(short) = %d\n", _Alignof(short));
    printf("_Alignof(int) = %d\n", _Alignof(int));
    printf("_Alignof(long) = %d\n", _Alignof(long));
    printf("_Alignof(long) long = %d\n", _Alignof(long long));
    printf("_Alignof(float) = %d\n", _Alignof(float));
    printf("_Alignof(double) = %d\n", _Alignof(double));
    return 0;
}
