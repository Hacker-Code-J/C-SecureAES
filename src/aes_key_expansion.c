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

// // AES key expansion
// void KeyExpansion(const u8* key, u32* rKey) {
// 	u32 temp;
// 	int i = 0;
	
// 	// Copy the input key to the first round key
// 	while (i < 4) {
// 		rKey[i] = (u32)key[4*i] << 0x18 | 
// 		(u32)key[4*i+1] << 0x10 | 
// 		(u32)key[4*i+2] << 0x08 | 
// 		(u32)key[4*i+3];
// 		i++;
// 	}
	
// 	i = 4;
	
// 	// Generate the remaining round keys
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

void KeyExpansion(const u8* key, u32* rKey) {
	u32 temp;
	
	// Unroll the initial key copying loop
    rKey[0] = (u32)key[0] << 0x18 | (u32)key[1] << 0x10 | (u32)key[2] << 0x08 | (u32)key[3];
    rKey[1] = (u32)key[4] << 0x18 | (u32)key[5] << 0x10 | (u32)key[6] << 0x08 | (u32)key[7];
    rKey[2] = (u32)key[8] << 0x18 | (u32)key[9] << 0x10 | (u32)key[10] << 0x08 | (u32)key[11];
    rKey[3] = (u32)key[12] << 0x18 | (u32)key[13] << 0x10 | (u32)key[14] << 0x08 | (u32)key[15];
	
    // Optimize the round key generation loop
    for (int i = 4; i < 44; ++i) {
        temp = rKey[i - 1];
        if ((i & 3) == 0) { // Equivalent to i % 4 == 0 but faster
            temp = SubWord(RotWord(temp)) ^ rCon[i / 4 - 1];
        }
        rKey[i] = rKey[i - 4] ^ temp;
    }
}
