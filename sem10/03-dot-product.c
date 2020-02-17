#include <stdio.h>
#include <emmintrin.h>
#include <xmmintrin.h>


float dot(const float* x, const float* y, size_t N) {
    __m128 sum = _mm_setzero_ps();

    while (N > 4) {
        __m128 l = _mm_load_ps(x);
        __m128 r = _mm_load_ps(y);

        __m128 mul = _mm_mul_ps(l, r);
        sum = _mm_add_ps(sum, mul);

        N -= 4;
        x += 4;
        y += 4;
    }

    // float summed[4];
    // _mm_store_ps(summed, sum);
    // float result = summed[0] + summed[1] + summed[2] + summed[3];
    _mm_haddp_ps(sum, sum);
    _mm_haddp_ps(sum, sum);

    float result;
    _mm_store_ss(&result, sum);

    while (N > 0) {
        result += *x * *y;
        x++;
        y++;
        N--;
    }

    return result;
}


int main() {
    const float x[] = {1, 2, 3, 4, 5};
    const float y[] = {1, 2, 3, 4, 5};
    printf("%f\n", dot(x, y, 6));
}
