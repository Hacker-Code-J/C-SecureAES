#include <stdlib.h>
#include <time.h>

#include "utils.h"

void RANDOM_KEY_GENERATION(u8* key) {
    srand((u32)time(NULL));
    
    // Initialize pointer to the start of the key array
    u8* p = key;

    // Set the counter to 16 bytes
    int cnt = 0;

    // Loop until all 16 bytes are filled
    while (cnt < 16) {
        *p = rand() & 0xff; // Assign a random byte (0 to 255) to the current location pointed by p
        p++;                // Move to the next byte
        cnt++;              // Decrement the byte count
    }
}

u8 gmul(u8 a, u8 b) {
    u8 p = 0;  // Accumulator for the product
    u8 high_bit_mask = 0x80;  // Mask for detecting the high bit (0x80 in hexadecimal is 10000000 in binary)
    u8 high_bit;  // Variable to store the high bit
    u8 modulo = 0x1B;  // The reduction polynomial (x^8 + x^4 + x^3 + x + 1) for AES, represented in hexadecimal

    // Iterate over all bits of b
    for (int i = 0; i < 8; i++) {
        // If the least significant bit of b is 1, add a to the product
        if (b & 1) {
            p ^= a;
        }

        // Store the high bit of a
        high_bit = a & high_bit_mask;

        // Shift a to the left, effectively multiplying it by x
        a <<= 1;

        // If the high bit was 1, reduce the result modulo the reduction polynomial
        if (high_bit) {
            a ^= modulo;
        }

        // Shift b to the right, moving to the next bit
        b >>= 1;
    }

    return p;
}