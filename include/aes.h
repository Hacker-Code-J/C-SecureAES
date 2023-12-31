/**
 * @file aes.h
 * @brief AES encryption and decryption functions.
 *
 * This header file declares the core functions for AES encryption and decryption.
 * It includes function prototypes for initializing the AES context, encrypting data,
 * and decrypting data.
 */

#ifndef _AES_H
#define _AES_H

#include "utils.h"

/**
 * AddRoundKey - XORs the state with a round key.
 * 
 * In this step, each byte of the state is XORed with the corresponding byte of the round key.
 * 
 * @param state A pointer to the state, an array of bytes representing the current state of the AES encryption.
 * @param rKey A pointer to the round keys, an array of 32-bit unsigned integers (u32) containing the expanded keys.
 */
void AddRoundKey(u8* state, const u32* rKey);

void SubBytes(u8* state);
void InvSubBytes(u8* state);

void ShiftRows(u8* state);
void InvShiftRows(u8* state);

#define MUL_GF256(a, b) ({ \
    u8 res = 0; \
    u8 MSB_mask = 0x80; \
    u8 MSB; \
    u8 modulo = 0x1B; \
    u8 temp_a = (a); \
    u8 temp_b = (b); \
    for (int i = 0; i < 8; i++) { \
        if (temp_b & 1) \
            res ^= temp_a; \
        MSB = temp_a & MSB_mask; \
        temp_a <<= 1; \
        if (MSB) \
            temp_a ^= modulo; \
        temp_b >>= 1; \
    } \
    res; \
})

/**
 * Performs the MixColumns step in AES.
 * This operation treats each column of the state as a polynomial over GF(2^8) and multiplies it
 * with a fixed polynomial c(x) = 03.x^3 + 01.x^2 + 01.x + 02, modulo x^4 + 1.
 * 
 * @param state The current state (128 bits) of the AES algorithm, represented as a 16-byte array.
 */
void MixColumns(u8 *state);
void InvMixColumns(u8 *state);

void SubMix(u8* state);
void InvSubInvMix(u8* state);

/**
 * @brief Encrypts a single block of plaintext using AES-128.
 *
 * This function performs AES-128 encryption on a single block (16 bytes) of plaintext.
 * It follows the AES encryption process including the key expansion, initial round,
 * main rounds, and the final round. The KeyExpansion function is used to generate
 * the round keys from the original key. Each round of the AES process involves
 * SubBytes, ShiftRows, and MixColumns transformations, followed by an AddRoundKey step.
 * The MixColumns step is omitted in the final round.
 *
 * @param plaintext A pointer to the block of plaintext to be encrypted.
 * @param key A pointer to the 128-bit key used for encryption.
 * @param ciphertext A pointer to the buffer where the encrypted data will be stored.
 */
void AES_Encrypt(const u8* plaintext, const u8* key, u8* ciphertext);
void AES_Encrypt_Precomp(const u8* plaintext, const u8* key, u8* ciphertext);


/**
 * @brief Decrypts a single block of ciphertext using AES-128.
 *
 * This function performs AES-128 decryption on a single block (16 bytes) of ciphertext.
 * It utilizes the inverse AES operations - InvSubBytes, InvShiftRows, and InvMixColumns,
 * along with the AddRoundKey function. The KeyExpansion function is used to generate
 * the round keys from the original key.
 *
 * @param ciphertext A pointer to the block of ciphertext to be decrypted.
 * @param key A pointer to the 128-bit key used for decryption.
 * @param plaintext A pointer to the buffer where the decrypted plaintext will be stored.
 */
void AES_Decrypt(const u8* ciphertext, const u8* key, u8* plaintext);
void AES_Decrypt_Precomp(const u8* ciphertext, const u8* key, u8* plaintext);

void AES_Encrypt_32BIT(const u8* plaintext, const u8* key, u8* ciphertext);
void AES_Decrypt_32BIT(const u8* plaintext, const u8* key, u8* ciphertext);

// /* Error codes */
// typedef enum {
//     AES_SUCCESS = 0,
//     AES_INVALID_KEY_LENGTH,
//     AES_INVALID_INPUT_LENGTH
// } aes_error_t;

// /* Key structure for AES */
// typedef struct {
//     uint8_t key[AES_KEY_SIZE_256];  // Maximum key size
//     uint16_t key_length;            // Key length in bytes
// } aes_key_t;

// /* Context for AES operation, includes key and any other state */
// typedef struct {
//     aes_key_t key;
//     // Additional state can be added here (like for modes that use an IV)
// } aes_ctx_t;

// /* Function Prototypes */

// /**
//  * Initialize AES context with the specified key.
//  *
//  * This function sets up the AES context with the provided key. It performs
//  * necessary checks on the key length and prepares the context for encryption
//  * or decryption operations.
//  *
//  * @param ctx Pointer to the AES context to be initialized.
//  * @param key Pointer to the key.
//  * @param key_length Length of the key in bytes (16, 24, or 32).
//  * @return AES_SUCCESS if successful, otherwise an error code.
//  */
// aes_error_t aes_init(aes_ctx_t *ctx, const uint8_t *key, size_t key_length);

// /**
//  * Encrypt a single block using AES.
//  *
//  * This function encrypts a single 16-byte block of plaintext using the AES
//  * algorithm and the provided context (which includes the key).
//  *
//  * @param ctx Pointer to the AES context, which includes the key.
//  * @param plaintext Pointer to the 16-byte block of plaintext.
//  * @param ciphertext Pointer to the buffer for the encrypted data.
//  * @return AES_SUCCESS if successful, otherwise an error code.
//  */
// aes_error_t aes_encrypt_block(const aes_ctx_t *ctx, const uint8_t plaintext[AES_BLOCK_SIZE], uint8_t ciphertext[AES_BLOCK_SIZE]);

// /**
//  * Decrypt a single block using AES.
//  *
//  * This function decrypts a single 16-byte block of ciphertext using the AES
//  * algorithm and the provided context (which includes the key).
//  *
//  * @param ctx Pointer to the AES context, which includes the key.
//  * @param ciphertext Pointer to the 16-byte block of ciphertext.
//  * @param plaintext Pointer to the buffer for the decrypted data.
//  * @return AES_SUCCESS if successful, otherwise an error code.
//  */
// aes_error_t aes_decrypt_block(const aes_ctx_t *ctx, const uint8_t ciphertext[AES_BLOCK_SIZE], uint8_t plaintext[AES_BLOCK_SIZE]);

// /* Additional function prototypes for key expansion, different modes (CBC, CTR, etc.) can be added here. */

#endif // AES_H