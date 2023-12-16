#include <stdio.h>
#include <stdint.h>

#include "aes_key_expansion.h"

// Example usage
int main() {
    u8 uKey[AES_KEY_SIZE] = { 0x00, };
    RANDOM_KEY_GENERATION(uKey);
    for (int i = 0; i < 16; i++) {
        printf("%02x", uKey[i]);
    } printf("\n");

    u32 rKeys[44];
    KeyExpansion(uKey, rKeys);
    for (int i = 0; i < 44; i++) {
        printf("%08x\n", rKeys[i]);
    }
    
    return 0;
}