#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


enum {
    N = 1,
    STACK_SIZE = 16 * 4096,
};

void* thread_function(void* arg) {
    size_t i = (size_t)arg;
    printf("&i = %p\n", &i);
    return (void*)(i + 12);
}

int main() {
    pthread_t thread;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    char* stack = malloc(STACK_SIZE);
    printf("stack = %p\n", stack);
    pthread_attr_setstack(&attr, stack, STACK_SIZE);
    pthread_create(&thread, &attr, thread_function, (void*)1);

    void* res = NULL;
    pthread_join(thread, &res);
    printf("thread returned %zu\n", (size_t) res);
    free(stack);
}
