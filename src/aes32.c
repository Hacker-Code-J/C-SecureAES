#include "aes32.h"

#define RotWord32(word) (((word) << 0x08) | ((word) >> 0x18))
#define SubWord32(word) ( \
    (Te3[(word >> 0x18) & 0xff] & 0xff000000) | \
    (Te3[(word >> 0x10) & 0xff] & 0x00ff0000) | \
    (Te1[(word >> 0x08) & 0xff] & 0x0000ff00) | \
    (Te1[ word          & 0xff] & 0x000000ff)   \
)
void KeyExpansion32(const u8* uKey, u32* rKey) {
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
            temp = SubWord32(RotWord32(temp)) ^ rCon32[i / Nk - 1];
        } else if (Nk > 6 && i % Nk == 4) {
            // Additional S-box transformation for AES-256
            temp = SubWord32(temp);
        }
        rKey[i] = rKey[i - Nk] ^ temp;
    }
}

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

void SubMix32(u32* state) {
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
	for (int i = 0; i < AES_BLOCK_SIZE / 4; ++i)
		state[i] = plaintext[i];
	
	KeyExpansion32(key, roundKey); // Key expansion
	
	AddRoundKey32(state, roundKey); // Initial round
	
	for (int round = 1; round <  Nr; round++) { // Main rounds
		ShiftRows32(state);
		SubMix32(state);
		AddRoundKey32(state, roundKey + 4 * round);
	}
	for (int i = 0; i < 4; i++) {
		state[i] = SubWord32(*(state + i));
	}
	ShiftRows32(state);
	AddRoundKey32(state, roundKey + 4 * Nr);
	
	// Copy state to ciphertext
	for (int i = 0; i < AES_BLOCK_SIZE / 4; i++)
		ciphertext[i] = state[i];
}