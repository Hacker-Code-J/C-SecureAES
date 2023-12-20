/**
 * @file aes.c
 * @brief Implementation of AES encryption and decryption.
 *
 * This source file contains the implementation of the AES algorithm for both
 * encryption and decryption processes. It uses the key expansion, utility functions,
 * and performs the core AES algorithm operations.
 */

#include <string.h> // For memcpy

#include "aes_key_expansion.h"
#include "aes.h"

// void AddRoundKey(u8* state, const u32* rKey) {
//     for (int i = 0; i < AES_BLOCK_SIZE; i++) {
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

void AddRoundKey(u8* state, const u32* rKey) {
    for (int i = 0; i < AES_BLOCK_SIZE; i++) {
        // Extract the corresponding byte from the round key word
        state[i] ^= (rKey[i / 4] >> (8 * (3 - (i % 4)))) & 0xFF;
    }
}

void SubBytes(u8* state) {
    for (int i = 0; i < AES_BLOCK_SIZE; i++) {
        state[i] = s_box[state[i]];
    }
}

void InvSubBytes(u8* state) {
    for (int i = 0; i < AES_BLOCK_SIZE; i++) {
        state[i] = inv_s_box[state[i]];
    }
}

void ShiftRows(u8* state) {
	u8 temp;
	
	// Row 1: shift left by 1
	temp = state[1];
	state[1] = state[5];
	state[5] = state[9];
	state[9] = state[13];
	state[13] = temp;
	
	// Row 2: shift left by 2
	temp = state[2];
	state[2] = state[10];
	state[10] = temp;
	temp = state[6];
	state[6] = state[14];
	state[14] = temp;
	
	// Row 3: shift left by 3 (or right by 1)
	temp = state[15];
	state[15] = state[11];
	state[11] = state[7];
	state[7] = state[3];
	state[3] = temp;
}

void InvShiftRows(u8* state) {
	u8 temp;
	
	// Row 1: shift left by 3 (or right by 1)
	temp = state[13];
	state[13] = state[9];
	state[9] = state[5];
	state[5] = state[1];
	state[1] = temp;
	
	// Row 2: shift left by 2
	temp = state[2];
	state[2] = state[10];
	state[10] = temp;
	temp = state[6];
	state[6] = state[14];
	state[14] = temp;
	
	// Row 3: shift left by 1
	temp = state[3];
	state[3] = state[7];
	state[7] = state[11];
	state[11] = state[15];
	state[15] = temp;
}

void MixColumns(u8* state) {
	u8 temp[4];
	// Multiply and add the elements in the column by the fixed polynomial
	for (int i = 0; i < 4; i++) { 
		temp[0] =
			MUL_GF256(0x02, state[i * 4]) ^
			MUL_GF256(0x03, state[i * 4 + 1]) ^
			state[i * 4 + 2] ^
			state[i * 4 + 3]; 
		
		temp[1] =
			state[i * 4] ^
			MUL_GF256(0x02, state[i * 4 + 1]) ^
			MUL_GF256(0x03, state[i * 4 + 2]) ^
			state[i * 4 + 3];
		
		temp[2] =
			state[i * 4] ^
			state[i * 4 + 1] ^
			MUL_GF256(0x02, state[i * 4 + 2]) ^
			MUL_GF256(0x03, state[i * 4 + 3]);
		
		temp[3] =
			MUL_GF256(0x03, state[i * 4]) ^
			state[i * 4 + 1] ^
			state[i * 4 + 2] ^
			MUL_GF256(0x02, state[i * 4 + 3]);
		
		// Copy the mixed column back to the state
		for (int j = 0; j < 4; j++)
			state[i * 4 + j] = temp[j];
	}
}

void InvMixColumns(u8* state) {
	u8 temp[4];
	
	for (int i = 0; i < 4; i++) { 
		temp[0] =
			MUL_GF256(0x0e, state[i * 4]) ^
			MUL_GF256(0x0b, state[i * 4 + 1]) ^
			MUL_GF256(0x0d, state[i * 4 + 2]) ^
			MUL_GF256(0x09, state[i * 4 + 3]); 
		
		temp[1] =
			MUL_GF256(0x09, state[i * 4]) ^
			MUL_GF256(0x0e, state[i * 4 + 1]) ^
			MUL_GF256(0x0b, state[i * 4 + 2]) ^
			MUL_GF256(0x0d, state[i * 4 + 3]);
		
		temp[2] =
			MUL_GF256(0x0d, state[i * 4]) ^
			MUL_GF256(0x09, state[i * 4 + 1]) ^
			MUL_GF256(0x0e, state[i * 4 + 2]) ^
			MUL_GF256(0x0b, state[i * 4 + 3]);
		
		temp[3] =
			MUL_GF256(0x0b, state[i * 4]) ^
			MUL_GF256(0x0d, state[i * 4 + 1]) ^
			MUL_GF256(0x09, state[i * 4 + 2]) ^
			MUL_GF256(0x0e, state[i * 4 + 3]);
		
		for (int j = 0; j < 4; j++)
			state[i * 4 + j] = temp[j];
	}
}

/*
 * Notes:
 * The `state` array is removed, and the `plaintext` array is used directly.
 * This is assuming `plaintext` is not a `const` in the calling code, otherwise, you'd need a temporary buffer.
 * The `plaintext` buffer is cast to `(u8*)` to allow writing. If `plaintext` is `const` in the calling code, this would be invalid.
 * Inline and SIMD optimizations are not explicitly shown but should be applied in the implementations of `SubBytes`, `ShiftRows`, `MixColumns`, and `AddRoundKey`.
 * The loop and memory access optimizations are applied within the limits of maintaining the algorithm's correctness and structure.
 * Remember, optimizing cryptographic code also needs to consider security implications, like side-channel attacks.
 * Ensure that any optimization does not inadvertently introduce vulnerabilities.
*/

// void AES_Encrypt(const u8* plaintext, const u8* key, u8* ciphertext) {
//     u32 roundKey[ROUND_KEYS_SIZE / sizeof(u32)];

//     // Key expansion
//     KeyExpansion(key, roundKey);

//     // Initial round
//     AddRoundKey((u8*)plaintext, roundKey);

//     // Main rounds
//     for (int round = 1; round <= Nr; round++) {
//         SubBytes((u8*)plaintext);
//         ShiftRows((u8*)plaintext);
//         if (round != Nr) {
//             MixColumns((u8*)plaintext);
//         }
//         AddRoundKey((u8*)plaintext, roundKey + round * AES_BLOCK_SIZE / sizeof(u32));
//     }

//     // Copy state to ciphertext
//     for (int i = 0; i < AES_BLOCK_SIZE; ++i) {
//         ciphertext[i] = plaintext[i];
//     }
// }

// void AES_Decrypt(const u8* ciphertext, const u8* key, u8* plaintext) {
//     u32 roundKey[ROUND_KEYS_SIZE / sizeof(u32)];
    
//     // Key expansion
//     KeyExpansion(key, roundKey);

//     // Initial round with the last round key
//     AddRoundKey((u8*)ciphertext, roundKey + Nr * AES_BLOCK_SIZE / sizeof(u32));

//     // Main rounds in reverse order
//     for (int round = Nr - 1; round >= 0; round--) {
//         InvShiftRows((u8*)ciphertext);
//         InvSubBytes((u8*)ciphertext);
//         AddRoundKey((u8*)ciphertext, roundKey + round * AES_BLOCK_SIZE / sizeof(u32));
//         if (round != 0) {
//             InvMixColumns((u8*)ciphertext);
//         }
//     }

//     // Copy state to plaintext
//     for (int i = 0; i < AES_BLOCK_SIZE; i++) {
//         plaintext[i] = ciphertext[i];
//     }
// }

// AES Encrypt function
void AES_Encrypt(const u8* plaintext, const u8* key, u8* ciphertext) {
	// AES-128/192/256: roundKey[44]/roundKey[52]/roundKey[60]
	u32 roundKey[ROUND_KEYS_SIZE / sizeof(u32)];
	u8 state[AES_BLOCK_SIZE]; // state[16]
	
	// Copy plaintext to state
	for (int i = 0; i < AES_BLOCK_SIZE; ++i)
		state[i] = plaintext[i];
	
	KeyExpansion(key, roundKey); // Key expansion
	
	AddRoundKey(state, roundKey); // Initial round
	
	for (int round = 1; round <= Nr; round++) { // Main rounds
		SubBytes(state); ShiftRows(state);
		if (round != Nr) MixColumns(state);
		// 1: roundKey[  4] | roundKey[5] | roundKey[ 6] | roundKey[    7]
		// 2: roundKey[  8] | roundKey[9] | roundKey[10] | roundKey[   11]
		// i: roundKey[4*i] |    ...      |    ...       | roundKey[4*i+3]
		AddRoundKey(state, roundKey + 4 * round);
	}
	
	// Copy state to ciphertext
	for (int i = 0; i < AES_BLOCK_SIZE; ++i)
		ciphertext[i] = state[i];
}

void AES_Decrypt(const u8* ciphertext, const u8* key, u8* plaintext) {
	u32 roundKey[ROUND_KEYS_SIZE / sizeof(u32)];
	u8 state[AES_BLOCK_SIZE];

    // Key expansion	
	KeyExpansion(key, roundKey);
	
	for (int i = 0; i < AES_BLOCK_SIZE; i++)
		state[i] = ciphertext[i];
	
	// Initial round with the last round key
	AddRoundKey(state, roundKey + 4 * Nr);
	
	// Main rounds in reverse order
	for (int round = Nr - 1; round >= 0; round--) {
		InvShiftRows(state);
		InvSubBytes(state);
		// i: roundKey[4*i] |    ...      |    ...       | roundKey[4*i+3]
		// 1: roundKey[  4] | roundKey[5] | roundKey[ 6] | roundKey[    7]
		// 0: roundKey[  0] | roundKey[1] | roundKey[ 2] | roundKey[    3]
		AddRoundKey(state, roundKey + 4 * round);
		if (round != 0)
			InvMixColumns(state);
	}

    // Copy state to plaintext
	for (int i = 0; i < AES_BLOCK_SIZE; i++)
		plaintext[i] = state[i];
}

void AES_Encrypt_32BIT(const u8* plaintext, const u8* key, u8* ciphertext) {
    u32 roundKey[ROUND_KEYS_SIZE / sizeof(u32)];
    u32 state[4];

    // Copy plaintext to state
    for (int i = 0; i < 4; i++) {
        state[i] = ((u32)plaintext[4 * i] << 0x18) |
				   ((u32)plaintext[4 * i + 1] << 0x10) |
				   ((u32)plaintext[4 * i + 2] << 0x08) |
				   (plaintext[4 * i + 3]);
    }

    // Key expansion
    KeyExpansion(key, roundKey);

    // Initial round
    AddRoundKey((u8*)state, roundKey);

    // Main rounds
    for (int round = 1; round < Nr; round++) {
        u32 temp[4];
        
        ShiftRows((u8*)plaintext);
		
		temp[0] = Te0[(state[0] >> 0x18) & 0xff] ^ 
				  Te1[(state[0] >> 0x10) & 0xff] ^
				  Te2[(state[0] >> 0x08) & 0xff] ^
				  Te3[state[0] & 0xff] ^
				  roundKey[0];
        temp[1] = Te0[(state[1] >> 0x18) & 0xff] ^ 
				  Te1[(state[1] >> 0x10) & 0xff] ^
				  Te2[(state[1] >> 0x08) & 0xff] ^
				  Te3[state[1] & 0xff] ^
				  roundKey[1];
		temp[2] = Te0[(state[2] >> 0x18) & 0xff] ^ 
				  Te1[(state[2] >> 0x10) & 0xff] ^
				  Te2[(state[2] >> 0x08) & 0xff] ^
				  Te3[state[2] & 0xff] ^
				  roundKey[2];
		temp[3] = Te0[(state[3] >> 0x18) & 0xff] ^ 
				  Te1[(state[3] >> 0x10) & 0xff] ^
				  Te2[(state[3] >> 0x08) & 0xff] ^
				  Te3[state[3] & 0xff] ^
				  roundKey[3];

		memcpy(state, temp, sizeof(temp));
    }

    // Final round (without MixColumns)
    u32 temp[4];
    temp[0] = (Te0[(state[0] >> 24)] & 0xff000000) ^
			  (Te1[(state[1] >> 16) & 0xff] & 0x00ff0000) ^
			  (Te2[(state[2] >> 8) & 0xff] & 0x0000ff00) ^
			  (Te3[state[3] & 0xff] & 0x000000ff) ^
			  roundKey[Nr];
    temp[1] = (Te0[(state[0] >> 24)] & 0xff000000) ^
			  (Te1[(state[1] >> 16) & 0xff] & 0x00ff0000) ^
			  (Te2[(state[2] >> 8) & 0xff] & 0x0000ff00) ^
			  (Te3[state[3] & 0xff] & 0x000000ff) ^
			  roundKey[Nr];
    temp[2] = (Te0[(state[0] >> 24)] & 0xff000000) ^
			  (Te1[(state[1] >> 16) & 0xff] & 0x00ff0000) ^
			  (Te2[(state[2] >> 8) & 0xff] & 0x0000ff00) ^
			  (Te3[state[3] & 0xff] & 0x000000ff) ^
			  roundKey[Nr];
    temp[3] = (Te0[(state[0] >> 24)] & 0xff000000) ^
			  (Te1[(state[1] >> 16) & 0xff] & 0x00ff0000) ^
			  (Te2[(state[2] >> 8) & 0xff] & 0x0000ff00) ^
			  (Te3[state[3] & 0xff] & 0x000000ff) ^
			  roundKey[Nr];
    // Repeat for temp[1], temp[2], temp[3]

    // Copy state to ciphertext
    for (int i = 0; i < 4; ++i) {
        ciphertext[4 * i]     = (u8)(temp[i] >> 24);
        ciphertext[4 * i + 1] = (u8)(temp[i] >> 16);
        ciphertext[4 * i + 2] = (u8)(temp[i] >> 8);
        ciphertext[4 * i + 3] = (u8)temp[i];
    }
}

void AES_Decrypt_32BIT(const u8* ciphertext, const u8* key, u8* plaintext) {
    u32 roundKey[ROUND_KEYS_SIZE / sizeof(u32)];
    u32 state[4];

    // Copy ciphertext to state
    for (int i = 0; i < AES_BLOCK_SIZE; i += 4) {
        state[i / 4] = ((u32)ciphertext[i] << 24) |
					   ((u32)ciphertext[i + 1] << 16) |
					   ((u32)ciphertext[i + 2] << 8) |
					   ciphertext[i + 3];
    }

    // Key expansion
    KeyExpansion(key, roundKey);

    // Initial round with the last round key
    AddRoundKey((u8*)state, roundKey + Nr * 4);

    // Main rounds in reverse order
    for (int round = Nr - 1; round > 0; round--) {
        u32 temp[4];
        temp[0] = Td0[state[0] >> 24] ^
				  Td1[(state[3] >> 16) & 0xff] ^
				  Td2[(state[2] >> 8) & 0xff] ^
				  Td3[state[1] & 0xff] ^
				  roundKey[round * 4];
        temp[1] = Td0[state[1] >> 24] ^
				  Td1[(state[0] >> 16) & 0xff] ^
				  Td2[(state[3] >> 8) & 0xff] ^
				  Td3[state[2] & 0xff] ^
				  roundKey[round * 4 + 1];
        temp[2] = Td0[state[2] >> 24] ^
				  Td1[(state[1] >> 16) & 0xff] ^
				  Td2[(state[0] >> 8) & 0xff] ^
				  Td3[state[3] & 0xff] ^
				  roundKey[round * 4 + 2];
        temp[3] = Td0[state[3] >> 24] ^
				  Td1[(state[2] >> 16) & 0xff] ^
				  Td2[(state[1] >> 8) & 0xff] ^
				  Td3[state[0] & 0xff] ^ roundKey[round * 4 + 3];
        memcpy(state, temp, sizeof(temp));
    }

    // Final round (without InvMixColumns)
    u32 temp[4];
    for (int i = 0; i < 4; i++) {
        temp[i] = (Td4[(state[i] >> 24)] << 24) |
				  (Td4[(state[(i + 3) % 4] >> 16) & 0xff] << 16) |
				  (Td4[(state[(i + 2) % 4] >> 8) & 0xff] << 8) |
				  (Td4[state[(i + 1) % 4] & 0xff]);
    }
    AddRoundKey((u8*)temp, roundKey);

    // Copy state to plaintext
    for (int i = 0; i < 4; i++) {
        plaintext[4 * i]     = (u8)(temp[i] >> 24);
        plaintext[4 * i + 1] = (u8)(temp[i] >> 16);
        plaintext[4 * i + 2] = (u8)(temp[i] >> 8);
        plaintext[4 * i + 3] = (u8)temp[i];
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