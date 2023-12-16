#include <stdio.h>
#include <stdint.h>

#include "aes_key_expansion.h"

// Example usage
int main() {
    u8 key[16] = { 0x00, };
    RANDOM_KEY_GENERATION(key);
    for (int i = 0; i < 16; i++) {
        printf("%02x", key[i]);
    } printf("\n");

    u32 roundKeys[44];
    KeyExpansion(key, roundKeys);
    for (int i = 0; i < 44; i++) {
        printf("%08x\n", roundKeys[i]);
    }
    
    return 0;
}