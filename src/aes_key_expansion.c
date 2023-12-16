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
	
	// Unroll the initial uKey copying loop
    rKey[0] = (u32)uKey[0] << 0x18 | (u32)uKey[1] << 0x10 | (u32)uKey[2] << 0x08 | (u32)uKey[3];
    rKey[1] = (u32)uKey[4] << 0x18 | (u32)uKey[5] << 0x10 | (u32)uKey[6] << 0x08 | (u32)uKey[7];
    rKey[2] = (u32)uKey[8] << 0x18 | (u32)uKey[9] << 0x10 | (u32)uKey[10] << 0x08 | (u32)uKey[11];
    rKey[3] = (u32)uKey[12] << 0x18 | (u32)uKey[13] << 0x10 | (u32)uKey[14] << 0x08 | (u32)uKey[15];
	
    // Optimize the round uKey generation loop
    for (int i = 4; i < 44; ++i) {
        temp = rKey[i - 1];
        if ((i & 3) == 0) { // Equivalent to i % 4 == 0 but faster
            temp = SubWord(RotWord(temp)) ^ rCon[i / 4 - 1];
        }
        rKey[i] = rKey[i - 4] ^ temp;
    }
}
