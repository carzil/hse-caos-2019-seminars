#include <stdio.h>
#include <string.h>


size_t my_strlen(const char* s) {
    size_t len = 0;
    while (*s != '\0') {
        ++len;
        ++s;
    }
    return len;
}


int main() {
    char a[] = "test";
    char b[] = "oops...";

    printf("a = %s, b = %s\n", a, b);

    strcpy(b, a);

    printf("a = %s, b = %s\n", a, b);
    return 0;
}
