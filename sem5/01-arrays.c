#include <stdio.h>


void f1(int arr[12]) {
    arr[9] = 3;
}

void f2(int arr[]) {
    arr[3] = 2;
}


#define ARRAY_LEN(arr) (sizeof(arr) / sizeof((arr)[0]))


int main() {
    int arr[10];
    for (size_t i = 0; i < ARRAY_LEN(arr); ++i) {
        arr[i] = 0;
    }

    f1(arr);
    f2(arr);

    for (size_t i = 0; i < ARRAY_LEN(arr); ++i) {
        printf("%d ", arr[i]);
    }

    printf("\n");
}
