/**
 * @file aes_key_expansion.c
 * @brief Implementation of AES key expansion functions.
 *
 * This source file contains the implementations of the functions declared
 * in aes_key_expansion.h for the key expansion process in the AES algorithm.
 * It includes the routines to transform and expand the original key into
 * multiple round keys.
 */
#include <immintrin.h>

#include "aes_key_expansion.h"

/* Original ver. */

// // RotWord rotates the input word left by one byte
// u32 RotWord(u32 word) {
//     return (word << 0x08) | (word >> 0x18);
// }

// // SubWord applies the S-box to each byte of the input word
// u32 SubWord(u32 word) {
//     return (u32)s_box[word >> 0x18] << 0x18 | 
//            (u32)s_box[(word >> 0x10) & 0xFF] << 0x10 | 
//            (u32)s_box[(word >> 0x08) & 0xFF] << 0x08 | 
//            (u32)s_box[word & 0xFF];
// }

// // AES Key expansion
// void KeyExpansion(const u8* uKey, u32* rKey) {
// 	u32 temp;
// 	int i = 0;
	
// 	// Copy the input Key to the first round uKey
// 	while (i < 4) {
// 		rKey[i] = (u32)uKey[4*i] << 0x18 | 
// 		(u32)uKey[4*i+1] << 0x10 | 
// 		(u32)uKey[4*i+2] << 0x08 | 
// 		(u32)uKey[4*i+3];
// 		i++;
// 	}
	
// 	i = 4;
	
// 	// Generate the remaining round uKeys
// 	while (i < 44) {
// 		temp = rKey[i-1];
// 		if (i % 4 == 0) {
// 			temp = SubWord(RotWord(temp)) ^ rCon[i / 4 - 1];
// 		}
// 		rKey[i] = rKey[i-4] ^ temp;
// 		i++;
// 	}
// }

/* Optimized ver. */

/*
// Inlined RotWord
#define RotWord(word) (((word) << 0x08) | ((word) >> 0x18))

// Inlined SubWord (assuming s_box is a constant array)
#define SubWord(word) ( \
    ((u32)s_box[(word) >> 0x18] << 0x18) | \
    ((u32)s_box[((word) >> 0x10) & 0xFF] << 0x10) | \
    ((u32)s_box[((word) >> 0x08) & 0xFF] << 0x08) | \
    ((u32)s_box[(word) & 0xFF]) \
)
*/
// void KeyExpansion(const u8* uKey, u32* rKey) {
// 	u32 temp;
	
// 	// Unroll the initial uKey copying loop
//     rKey[0] = (u32)uKey[0] << 0x18 | (u32)uKey[1] << 0x10 | (u32)uKey[2] << 0x08 | (u32)uKey[3];
//     rKey[1] = (u32)uKey[4] << 0x18 | (u32)uKey[5] << 0x10 | (u32)uKey[6] << 0x08 | (u32)uKey[7];
//     rKey[2] = (u32)uKey[8] << 0x18 | (u32)uKey[9] << 0x10 | (u32)uKey[10] << 0x08 | (u32)uKey[11];
//     rKey[3] = (u32)uKey[12] << 0x18 | (u32)uKey[13] << 0x10 | (u32)uKey[14] << 0x08 | (u32)uKey[15];
	
//     // Optimize the round uKey generation loop
//     for (int i = 4; i < 44; ++i) {
//         temp = rKey[i - 1];
//         if ((i & 3) == 0) { // Equivalent to i % 4 == 0 but faster
//             temp = SubWord(RotWord(temp)) ^ rCon[i / 4 - 1];
//         }
//         rKey[i] = rKey[i - 4] ^ temp;
//     }
// }

/* General ver. */

// Inlined RotWord
#define RotWord(word) (((word) << 0x08) | ((word) >> 0x18))

// Inlined SubWord (assuming s_box is a constant array)
#define SubWord(word) ( \
    ((u32)s_box[(word) >> 0x18] << 0x18) | \
    ((u32)s_box[((word) >> 0x10) & 0xFF] << 0x10) | \
    ((u32)s_box[((word) >> 0x08) & 0xFF] << 0x08) | \
    ((u32)s_box[(word) & 0xFF]) \
)

void KeyExpansion(const u8* uKey, u32* rKey) {
	u32 temp;
	
	for (int i = 0; i < Nk; i++) {
        rKey[i] = (u32)uKey[4*i] << 0x18 | 
                  (u32)uKey[4*i+1] << 0x10 | 
                  (u32)uKey[4*i+2] << 0x08 | 
                  (u32)uKey[4*i+3];
    }
    
    for (int i = Nk; i < (Nr + 1) * 4; i++) {
        temp = rKey[i - 1];
        if (i % Nk == 0) {
            temp = SubWord(RotWord(temp)) ^ rCon[i / Nk - 1];
        } else if (Nk > 6 && i % Nk == 4) {
            // Additional S-box transformation for AES-256
            temp = SubWord(temp);
        }
        rKey[i] = rKey[i - Nk] ^ temp;
    }
}

void ReverseKeyExpansion(const u32* rKey, u8* originalKey) {
    u32 temp;
    u32 reversedKey[ROUND_KEYS_SIZE / sizeof(u32)];

    // Copy the last round key as the starting point
    for (int i = 0; i < Nk; i++) {
        reversedKey[(Nr * Nk) + i] = rKey[(Nr * Nk) + i];
    }

    // Reverse the key expansion process
    for (int i = (Nr * Nk) - 1; i >= 0; i--) {
        temp = reversedKey[i + 1];
        if (i % Nk == 0) {
            temp = RotWord(temp);
            temp = SubWord(temp) ^ rCon[i / Nk];
        } else if (Nk > 6 && i % Nk == 4) {
            temp = SubWord(temp);
        }
        reversedKey[i] = reversedKey[i + Nk] ^ temp;
    }

    // Extract the original key
    for (int i = 0; i < Nk; i++) {
        originalKey[4 * i]     = (u8)(reversedKey[i] >> 24);
        originalKey[4 * i + 1] = (u8)(reversedKey[i] >> 16);
        originalKey[4 * i + 2] = (u8)(reversedKey[i] >> 8);
        originalKey[4 * i + 3] = (u8)(reversedKey[i]);
    }
}

// // Vectorized RotWord
// __m128i RotWord_SSE(__m128i word) {
//     return _mm_shuffle_epi32(word, _MM_SHUFFLE(2, 1, 0, 3));
// }

// // Helper function to load a single 32-bit word into an SSE register
// __m128i load_word_sse(u32 word) {
//     return _mm_cvtsi32_si128((int)word);
// }

// // Vectorized SubWord using SSE2 intrinsics
// __m128i SubWord_SSE(__m128i word) {
//     // Split the word into 4 bytes and duplicate each byte across 32-bit lanes
//     __m128i t1 = _mm_shuffle_epi8(word, _mm_set_epi8(
//         -1, -1, -1, 3,
//         -1, -1, -1, 2,
//         -1, -1, -1, 1,
//         -1, -1, -1, 0));

//     // Use _mm_shuffle_epi8 or a combination of shifts and masks to replace each byte
//     // with its corresponding s_box value. This requires a strategy to vectorize the lookup.

//     // Placeholder for actual s_box lookup implementation
//     // ...

//     // Combine the transformed bytes back into a single 32-bit word
//     __m128i result = _mm_or_si128(_mm_or_si128(
//         _mm_srli_si128(t1, 12), 
//         _mm_srli_si128(t1, 8)), 
//         _mm_or_si128(_mm_srli_si128(t1, 4), t1));

//     return result;
// }

// // Example usage
// u32 optimized_subword(u32 word) {
//     __m128i wordVec = load_word_sse(word);
//     __m128i transformed = SubWord_SSE(wordVec);
//     return (u32)_mm_cvtsi128_si32(transformed);
// }

// void KeyExpansion_SSE(const u8* uKey, u32* rKey) {
//     __m128i temp;
//     __m128i* rKeyVec = (__m128i*)rKey;

//     // Load initial keys
//     for (int i = 0; i < Nk; i++) {
//         rKeyVec[i] = _mm_set_epi32(
//             (u32)uKey[4*i] << 0x18 | (u32)uKey[4*i+1] << 0x10 | 
//             (u32)uKey[4*i+2] << 0x08 | (u32)uKey[4*i+3],
//             0, 0, 0
//         );
//     }

//     // Rest of key expansion
//     for (int i = Nk; i < (Nr + 1) * 4; i++) {
//         temp = rKeyVec[i - 1];
//         if (i % Nk == 0) {
//             temp = _mm_xor_si128(SubWord_SSE(RotWord_SSE(temp)), _mm_load_si128((__m128i*)&rCon[i / Nk - 1]));
//         } else if (Nk > 6 && i % Nk == 4) {
//             temp = SubWord_SSE(temp);
//         }
//         rKeyVec[i] = _mm_xor_si128(rKeyVec[i - Nk], temp);
//     }
// }