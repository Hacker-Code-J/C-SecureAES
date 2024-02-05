#include "aes_utils.h"
#include "aes.h"
#include "aes32.h"

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

	puts("\n\nRound Key Start");
	for (u8 i = 0; i < TSRK; i++) {
		if(i % 4 == 0 && i > 1)
			puts("");
		printf("%08X:", rKey[i]);
	}
	puts("Round Key End\n\n");

	AddRoundKey(state, rKey); // Initial round

	for (u8 r = 1; r <= NR; r++) { // Main rounds
		for (u8 i = 0; i < 16; i++)
			printf("%02X:", state[i]);
		puts("");
		SubBytes(state); ShiftRows(state);
		if (r != NR) MixColumns(state);
		// 1: roundKey[  4] | roundKey[5] | roundKey[ 6] | roundKey[    7]
		// 2: roundKey[  8] | roundKey[9] | roundKey[10] | roundKey[   11]
		// i: roundKey[4*i] |    ...      |    ...       | roundKey[4*i+3]
		AddRoundKey(state, rKey + 4 * r);
	}
	for (u8 i = 0; i < 16; i++)
		printf("%02X:", state[i]);
	puts("");
	
	// Copy state to ciphertext
	// for (int i = 0; i < 16; ++i)
	// 	dst[i] = state[i];
    memcpy(dst, state, 16);
}

/* AES on 32-bit platform */

void AddRoundKey32(u32* state, const u32* rKey) {
	for (int i = 0; i < 4; i++) {
        // XOR each 32-bit word of the state with the corresponding word of the round key
        state[i] ^= rKey[i];
    }
}

// void AES32_round(u32* state, u32* rKey) {
// 	u32 tmp[4];
// 	/*
// 	for (int k = 0; k < 4; k++) {
// 	tmp[k] = Te0[state[k] >> 24] ^ Te1[(state[(k+1)%4] >> 16) & 0xFF] ^
// 	Te2[(state[(k+2)%4] >> 8) & 0xFF] ^ Te3[state[(k+3)%4] & 0xFF] ^ rk[k];
// 	} */


// }

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
	puts("\n\nRound Key32 Start");
	for (u8 i = 0; i < TSRK; i++) {
		if(i % 4 == 0 && i > 1)
			puts("");
		printf("%08X:", rKey[i]);
	}
	puts("Round Key32 End\n\n");

	AddRoundKey32(state, rKey);

	for (u8 r = 1; r < NR; r++) {
		for (u8 i = 0; i < 4; i++)
			printf("%08X:", state[i]);
		puts("");
		tmp[0] = Te0[state[0] >> 0x18] ^ Te1[(state[1] >> 0x10) & 0xFF] ^ Te2[(state[2] >> 0x08) & 0xFF] ^ Te3[state[3] & 0xFF] ^ rKey[4 * r    ];
		tmp[1] = Te0[state[1] >> 0x18] ^ Te1[(state[2] >> 0x10) & 0xFF] ^ Te2[(state[3] >> 0x08) & 0xFF] ^ Te3[state[0] & 0xFF] ^ rKey[4 * r + 1];
		tmp[2] = Te0[state[2] >> 0x18] ^ Te1[(state[3] >> 0x10) & 0xFF] ^ Te2[(state[0] >> 0x08) & 0xFF] ^ Te3[state[1] & 0xFF] ^ rKey[4 * r + 2];
		tmp[3] = Te0[state[3] >> 0x18] ^ Te1[(state[0] >> 0x10) & 0xFF] ^ Te2[(state[1] >> 0x08) & 0xFF] ^ Te3[state[2] & 0xFF] ^ rKey[4 * r + 3];
		memcpy(state, tmp, 16);
		// for (u8 k = 0; k < 4; k++) {
		// 	state[k] = tmp[k];
		// }
	}
	for (u8 i = 0; i < 4; i++)
		printf("%08X:", state[i]);
	puts("");
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
	// for (int k = 0; k < 4; k++) {
	// 	state[k] = tmp[k];
	// }
	for (u8 i = 0; i < 4; i++)
		printf("%08X:", state[i]);
	puts("");

	wordToByte(dst, state, 4);
}