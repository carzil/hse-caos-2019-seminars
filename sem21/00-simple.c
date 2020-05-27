#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>

enum {
    N = 10,
};

void* thread_function(void* arg) {
    size_t i = (size_t)arg;
    printf("Hi from %zu!\n", i);
    return (void*)(i + 12);
}

int main() {
    pthread_t threads[N];
    for (size_t i = 0; i < N; i++) {
        pthread_create(&threads[i], NULL, thread_function, (void*) i);
    }

    for (size_t i = 0; i < N; i++) {
        void* res = NULL;
        pthread_join(threads[i], &res);
        printf("%zu returned %zu\n", i, (size_t) res);
    }
}
