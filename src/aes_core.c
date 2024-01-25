#include "aes.h"

void AddRoundKey(u8* state, const u32* rKey) {
    for (int i = 0; i < 16; i++) {
        // Extract the corresponding byte from the round key word
        state[i] ^= (rKey[i / 4] >> (8 * (3 - (i % 4)))) & 0xFF;
    }
}

void SubBytes(u8* state) {
    for (int i = 0; i < 16; i++) {
        state[i] = sbox[state[i]];
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

// AES Encrypt function
void AES_Encrypt(u8* dst, const u8* src, const u8* uKey, const u8 AES_VERSION) {
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
	u8 state[16]; // 8 x 16 = 128
	
	// Copy plaintext to state
	// for (int i = 0; i < 16; i++)
	// 	state[i] = src[i];
	memcpy(state, src, 16);

    KeySchedule(rKey, uKey, AES_VERSION);

	AddRoundKey(state, rKey); // Initial round
	ShiftRows(state);
	for (int round = 1; round <= NR; round++) { // Main rounds
		SubBytes(state); ShiftRows(state);
		if (round != NR) MixColumns(state);
		// 1: roundKey[  4] | roundKey[5] | roundKey[ 6] | roundKey[    7]
		// 2: roundKey[  8] | roundKey[9] | roundKey[10] | roundKey[   11]
		// i: roundKey[4*i] |    ...      |    ...       | roundKey[4*i+3]
		AddRoundKey(state, rKey + 4 * round);
	}
	
	// Copy state to ciphertext
	// for (int i = 0; i < 16; ++i)
	// 	dst[i] = state[i];
    memcpy(dst, state, 16);
}