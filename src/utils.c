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