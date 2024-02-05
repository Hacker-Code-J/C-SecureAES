#include "aes32.h"
#include "aes.h"
#include "aes_utils.h"

/* AES on 32-bit platform */

void AddRoundKey32(u32* state, const u32* rKey) {
	for (int i = 0; i < 4; i++) {
        // XOR each 32-bit word of the state with the corresponding word of the round key
        state[i] ^= rKey[i];
    }
}

void AES32_Encrypt(u8* dst, const u8* src, const u8* uKey, const u8 AES_VERSION) {
	u8 NR, TSRK;
    switch (AES_VERSION)
    {
    case AES192:
        NR = 12;
        TSRK = 52;
        break;
    case AES256:
        NR = 14;
        TSRK = 60;
        break;
    default:
        NR = 10;
        TSRK = 44;
        break;
    }
    
	u32 rKey[TSRK];
	u32 state[4], tmp[4];

	byteToWord(state, src, 16);

	KeySchedule(rKey, uKey, AES_VERSION);

	AddRoundKey32(state, rKey);

	for (u8 r = 1; r < NR; r++) {
		tmp[0] = Te0[state[0] >> 0x18] ^ Te1[(state[1] >> 0x10) & 0xFF] ^ Te2[(state[2] >> 0x08) & 0xFF] ^ Te3[state[3] & 0xFF] ^ rKey[4 * r    ];
		tmp[1] = Te0[state[1] >> 0x18] ^ Te1[(state[2] >> 0x10) & 0xFF] ^ Te2[(state[3] >> 0x08) & 0xFF] ^ Te3[state[0] & 0xFF] ^ rKey[4 * r + 1];
		tmp[2] = Te0[state[2] >> 0x18] ^ Te1[(state[3] >> 0x10) & 0xFF] ^ Te2[(state[0] >> 0x08) & 0xFF] ^ Te3[state[1] & 0xFF] ^ rKey[4 * r + 2];
		tmp[3] = Te0[state[3] >> 0x18] ^ Te1[(state[0] >> 0x10) & 0xFF] ^ Te2[(state[1] >> 0x08) & 0xFF] ^ Te3[state[2] & 0xFF] ^ rKey[4 * r + 3];
		memcpy(state, tmp, 16);
	}
    
	tmp[0] = (Te2[(state[0] >> 0x18)       ] & 0xFF000000) ^
			 (Te3[(state[1] >> 0x10) & 0xFF] & 0x00FF0000) ^
			 (Te0[(state[2] >> 0x08) & 0xFF] & 0x0000FF00) ^
			 (Te1[state[3] 			 & 0xFF] & 0x000000FF) ^ rKey[4 * NR    ];

	tmp[1] = (Te2[(state[1] >> 0x18)       ] & 0xFF000000) ^
			 (Te3[(state[2] >> 0x10) & 0xFF] & 0x00FF0000) ^
			 (Te0[(state[3] >> 0x08) & 0xFF] & 0x0000FF00) ^
			 (Te1[(state[0]        ) & 0xFF] & 0x000000FF) ^ rKey[4 * NR + 1];

	tmp[2] = (Te2[(state[2] >> 0x18)       ] & 0xFF000000) ^
			 (Te3[(state[3] >> 0x10) & 0xFF] & 0x00FF0000) ^
			 (Te0[(state[0] >> 0x08) & 0xFF] & 0x0000FF00) ^
			 (Te1[(state[1]        ) & 0xFF] & 0x000000FF) ^ rKey[4 * NR + 2];
	
	tmp[3] = (Te2[(state[3] >> 0x18)       ] & 0xFF000000) ^
			 (Te3[(state[0] >> 0x10) & 0xFF] & 0x00FF0000) ^
			 (Te0[(state[1] >> 0x08) & 0xFF] & 0x0000FF00) ^
			 (Te1[(state[2]        ) & 0xFF] & 0x000000FF) ^ rKey[4 * NR + 3];

	memcpy(state, tmp, 16);
	wordToByte(dst, state, 4);
}