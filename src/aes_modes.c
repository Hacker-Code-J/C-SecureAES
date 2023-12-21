#include <stdio.h>
#include <string.h>

#include "aes_modes.h"

void pkcs7_pad(u8* block, size_t block_len, size_t input_len) {
    if (block_len < input_len) {
        fprintf(stderr, "Block length must be greater than input length.\n");
        return;
    }

    u8 padding_value = block_len - input_len;
    for (size_t i = input_len; i < block_len; ++i) {
        block[i] = padding_value;
    }
}

// int main() {
//     // Example usage
//     u8 data[16]; // 128-bit block
//     const char *input = "Hello World";
//     size_t input_len = strlen(input);

//     if (input_len > sizeof(data)) {
//         fprintf(stderr, "Input too long for the block size.\n");
//         return 1;
//     }

//     memcpy(data, input, input_len); // Copy input to data array
//     pkcs7_pad(data, sizeof(data), input_len); // Apply PKCS#7 padding

//     // Print padded data in hex
//     for (size_t i = 0; i < sizeof(data); ++i) {
//         printf("%02x ", data[i]);
//     }
//     printf("\n");

//     return 0;
// }