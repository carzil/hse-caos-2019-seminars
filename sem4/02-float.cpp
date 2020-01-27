#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <math.h>
#include <limits>


int main() {
    float x_value = std::numeric_limits<float>::quiet_NaN();
    // scanf("%f", &x_value);
    uint32_t x_bits = 0;

    memcpy(&x_bits, &x_value, sizeof(float));

    uint32_t mant = x_bits & ((1 << 23) - 1);
    x_bits >>= 23;
    uint32_t exp = x_bits & 0b11111111;
    x_bits >>= 8;
    uint32_t sign = x_bits & 1;
    printf("sign = %" PRIu32 "\n", sign);
    printf("exp  = %" PRIu32 "\n", exp);
    printf("mant = %" PRIu32 "\n", mant);
    return 0;
}
