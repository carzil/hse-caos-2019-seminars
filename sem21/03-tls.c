#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>

enum {
    N = 10,
};

_Thread_local int count = 0;

void* thread_function(void* arg) {
    count = 1;
    for (size_t i = 0; i < (size_t)arg; i++) {
        count *= 2;
    }
    printf("count = %d\n", count);
    return NULL;
}

int main() {
    pthread_t threads[N];
    for (size_t i = 0; i < N; i++) {
        pthread_create(&threads[i], NULL, thread_function, (void*) i);
    }

    for (size_t i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }
}
