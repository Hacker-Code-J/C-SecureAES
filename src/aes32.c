#include "aes_key_expansion.h"
#include "aes32.h"

void AddRoundKey32(u32* state, const u32* rKey) {
    for (int i = 0; i < AES_BLOCK_SIZE / 4; i++) {
        state[i] ^= rKey[i];
    }
}

void ShiftRows32(u32 *state) {
    u32 temp = state[1];
    state[1] = (temp >> 0x08) | (temp << 0x18);
    temp = state[2];
    state[2] = (temp >> 0x10) | (temp << 0x10);
    temp = state[3];
    state[3] = (temp >> 0x18) | (temp << 0x08);
}

void SubMix(u32* state) {
	u32 temp[4];
	for (int i = 0; i < AES_BLOCK_SIZE / 4; i++) {
		temp[i] = Te0[(state[i] >> 0x18) & 0xff] ^
				  Te1[(state[i] >> 0x10) & 0xff] ^
				  Te2[(state[i] >> 0x08) & 0xff] ^
				  Te3[state[i] & 0xff];
	}
	for (int i = 0; i < 4; i++) 
		state[i] = temp[i];
}

void AES_Encrypt32(const u32* plaintext, const u8* key, u32* ciphertext) {
	// AES-128/192/256: roundKey[44]/roundKey[52]/roundKey[60]
	u32 roundKey[ROUND_KEYS_SIZE / 4];
	u32 state[AES_BLOCK_SIZE / 4]; // state[4]
	
	// Copy plaintext to state
	for (int i = 0; i < AES_BLOCK_SIZE; ++i)
		state[i] = plaintext[i];
	
	KeyExpansion(key, roundKey); // Key expansion
	
	AddRoundKey(state, roundKey); // Initial round
	
	for (int round = 1; round <= Nr; round++) { // Main rounds
		ShiftRows32(state);
		SubMix32(state);
		AddRoundKey(state, roundKey + 4 * round);
	}
	
	// Copy state to ciphertext
	for (int i = 0; i < AES_BLOCK_SIZE / 4; ++i)
		ciphertext[i] = state[i];
}