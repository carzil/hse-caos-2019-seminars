#include <stdio.h>
#include <string.h>


void memory_leak() {
    void* ptr = malloc(100);
    // no free
}


void double_free() {
    void* ptr = malloc(100);
    // ...
    free(ptr);
    // ...
    free(ptr);
}

void use_after_free() {
    int* ptr = malloc(100);
    // ...
    free(ptr);
    // ...
    ptr[0] = 12;
}
