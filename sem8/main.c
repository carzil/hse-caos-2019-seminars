#include <stdio.h>


int test() {
    printf("Hi from test!\n");
    return 3;
}


int main() {
    int (*func)() = test;

    func();
    return 0;
}
