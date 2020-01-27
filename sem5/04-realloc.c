#include <stdlib.h>
#include <stdio.h>


void vec_resize(struct vec* vec, size_t new_size) {
    // ...
    while (vec->capacity < new_size) {
        vec->capacity *= 2;
        vec->data = realloc(vec->data, vec->capacity);
    }
    // ...
}
