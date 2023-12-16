#include "aes.h"

void AddRoundKey(u8* block, const u8* roundKey) {
    for (int i = 0; i < 16; i++) {
        block[i] ^= roundKey[i];
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