#ifndef _AES_H
#define _AES_H

#include "config.h"

#include <stdint.h>
#include <stddef.h>

/* AES block size in bytes */
#define AES_BLOCK_SIZE 16

/* Key lengths */
#define AES_KEY_SIZE_128 (128 / 8)
#define AES_KEY_SIZE_192 (192 / 8)
#define AES_KEY_SIZE_256 (256 / 8)

/* Error codes */
typedef enum {
    AES_SUCCESS = 0,
    AES_INVALID_KEY_LENGTH,
    AES_INVALID_INPUT_LENGTH
} aes_error_t;

/* Key structure for AES */
typedef struct {
    uint8_t key[AES_KEY_SIZE_256];  // Maximum key size
    uint16_t key_length;            // Key length in bytes
} aes_key_t;

/* Context for AES operation, includes key and any other state */
typedef struct {
    aes_key_t key;
    // Additional state can be added here (like for modes that use an IV)
} aes_ctx_t;

/* Function Prototypes */

/**
 * Initialize AES context with the specified key.
 *
 * This function sets up the AES context with the provided key. It performs
 * necessary checks on the key length and prepares the context for encryption
 * or decryption operations.
 *
 * @param ctx Pointer to the AES context to be initialized.
 * @param key Pointer to the key.
 * @param key_length Length of the key in bytes (16, 24, or 32).
 * @return AES_SUCCESS if successful, otherwise an error code.
 */
aes_error_t aes_init(aes_ctx_t *ctx, const uint8_t *key, size_t key_length);

/**
 * Encrypt a single block using AES.
 *
 * This function encrypts a single 16-byte block of plaintext using the AES
 * algorithm and the provided context (which includes the key).
 *
 * @param ctx Pointer to the AES context, which includes the key.
 * @param plaintext Pointer to the 16-byte block of plaintext.
 * @param ciphertext Pointer to the buffer for the encrypted data.
 * @return AES_SUCCESS if successful, otherwise an error code.
 */
aes_error_t aes_encrypt_block(const aes_ctx_t *ctx, const uint8_t plaintext[AES_BLOCK_SIZE], uint8_t ciphertext[AES_BLOCK_SIZE]);

/**
 * Decrypt a single block using AES.
 *
 * This function decrypts a single 16-byte block of ciphertext using the AES
 * algorithm and the provided context (which includes the key).
 *
 * @param ctx Pointer to the AES context, which includes the key.
 * @param ciphertext Pointer to the 16-byte block of ciphertext.
 * @param plaintext Pointer to the buffer for the decrypted data.
 * @return AES_SUCCESS if successful, otherwise an error code.
 */
aes_error_t aes_decrypt_block(const aes_ctx_t *ctx, const uint8_t ciphertext[AES_BLOCK_SIZE], uint8_t plaintext[AES_BLOCK_SIZE]);

/* Additional function prototypes for key expansion, different modes (CBC, CTR, etc.) can be added here. */

#endif // AES_H