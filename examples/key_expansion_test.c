#include <stdio.h>
#include <stdint.h>

#include "aes_key_expansion.h"

// Example usage
int main() {
    u8 key[16] = {
        0xf7, 0xe7, 0x8d, 0x3d, 0x3d, 0xee, 0x64, 0xba,
        0xc7, 0x88, 0xd0, 0xcb, 0x89, 0xc8, 0x82, 0x72
    };
    u32 roundKeys[44];

    KeyExpansion(key, roundKeys);
    
    // printf("Key Array (16-byte):\n");
    // for (int i = 0; i < 16; i++) {
    //     printf("%02x:", key[i]);
    //     if ((i + 1) % 4 == 0) {
    //         printf("\n");
    //     }
    // }

    // printf("\nRound Keys Array:\n");
    for (int i = 0; i < 44; i++) {
        printf("%08x\n", roundKeys[i]);
        // if ((i + 1) % 4 == 0) {
        //     printf("\n");
        // }
    }
    
    return 0;
}