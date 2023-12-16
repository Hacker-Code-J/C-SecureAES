#include "aes.h"

/* Original Ver. */
// void AddRoundKey(u8* state, const u32* rKey) {
//     // Iterate over each byte of the AES block.
//     for (int i = 0; i < AES_KEY_SIZE; i++) {
//         // Calculate the index of the word in rKey array. Since each word of rKey is 4 bytes (32 bits),
//         // and we are processing 1 byte at a time, we divide i by 4 to get the correct word index.
//         int wordIndex = i / 4;

//         // Calculate the byte position within the 32-bit word. We use modulo 4 operation on i
//         // to get the position of the byte within the word (0, 1, 2, or 3).
//         int bytePosition = i % 4;

//         // Shift the 32-bit word to align the target byte with the least significant byte (LSB) position.
//         // The shift amount depends on the byte position (0 to 3) and is calculated as 8 times (3 - position),
//         // effectively getting the byte in reverse order (as AES uses little-endian byte order).
//         u32 shiftedWord = rKey[wordIndex] >> (8 * (3 - bytePosition));

//         // Apply a bitmask to extract only the least significant byte from the shifted word.
//         // The bitmask 0xFF (255 in decimal) ensures that only the last 8 bits (1 byte) are retained.
//         u8 keyByte = shiftedWord & 0xFF;

//         // XOR the current byte of the state with the corresponding byte of the round key.
//         // This operation combines the state with the round key to add cryptographic diffusion.
//         state[i] ^= keyByte;
//     }
// }

/* Optimized Ver.*/

void AddRoundKey(u8* state, const u32* rKey) {
    for (int i = 0; i < AES_KEY_SIZE; i++) {
        // Extract the corresponding byte from the round key word
        state[i] ^= (rKey[i / 4] >> (8 * (3 - (i % 4)))) & 0xFF;
    }
}






















// aes_error_t aes_init(aes_ctx_t *ctx, const uint8_t *key, size_t key_length) {
//     // Check for null pointers to prevent undefined behavior
//     if (ctx == NULL || key == NULL) {
//         return AES_INVALID_INPUT_LENGTH;
//     }

//     // Validate the key length
//     switch (key_length) {
//         case AES_KEY_SIZE_128:
//         case AES_KEY_SIZE_192:
//         case AES_KEY_SIZE_256:
//             ctx->key.key_length = key_length;
//             break;
//         default:
//             // Return an error if the key length is not supported
//             return AES_INVALID_KEY_LENGTH;
//     }

//     // Copy the key into the context
//     // It's crucial to use a secure function like memcpy to handle the key
//     memcpy(ctx->key.key, key, key_length);

//     // Additional initialization steps can be performed here, such as
//     // key expansion or setting up initial vectors for certain AES modes.

//     return AES_SUCCESS;
// }

// aes_error_t aes_encrypt_block(const aes_ctx_t *ctx, const uint8_t plaintext[AES_BLOCK_SIZE], uint8_t ciphertext[AES_BLOCK_SIZE]) {
//     // Ensure that the pointers are not NULL
//     if (ctx == NULL || plaintext == NULL || ciphertext == NULL) {
//         return AES_INVALID_INPUT_LENGTH;
//     }

//     // Check if the key length is properly set
//     if (ctx->key.key_length != AES_KEY_SIZE_128 && 
//         ctx->key.key_length != AES_KEY_SIZE_192 && 
//         ctx->key.key_length != AES_KEY_SIZE_256) {
//         return AES_INVALID_KEY_LENGTH;
//     }

//     // Here, you would implement the actual encryption steps of the AES algorithm.
//     // This includes key expansion, initial AddRoundKey, followed by multiple rounds
//     // of SubBytes, ShiftRows, MixColumns, and AddRoundKey, and a final round without
//     // MixColumns.

//     // As an example, a pseudo-implementation might look like this:
//     // aes_expand_key(&ctx->key);  // Key expansion
//     // aes_add_round_key(...);     // Initial round key addition
//     // for (int round = 1; round <= total_rounds; ++round) {
//     //     aes_sub_bytes(...);
//     //     aes_shift_rows(...);
//     //     if (round != total_rounds) {
//     //         aes_mix_columns(...);
//     //     }
//     //     aes_add_round_key(...);
//     // }

//     // Placeholder for actual encryption
//     // memcpy(ciphertext, plaintext, AES_BLOCK_SIZE);

//     return AES_SUCCESS;
// }

// aes_error_t aes_decrypt_block(const aes_ctx_t *ctx, const uint8_t ciphertext[AES_BLOCK_SIZE], uint8_t plaintext[AES_BLOCK_SIZE]) {
//     // Similar checks and structure as the aes_encrypt_block function
//     // The decryption process is the inverse of the encryption
//     // It involves the InvSubBytes, InvShiftRows, InvMixColumns, and AddRoundKey steps

//     // Placeholder for actual decryption
//     // memcpy(plaintext, ciphertext, AES_BLOCK_SIZE);

//     return AES_SUCCESS;
// }