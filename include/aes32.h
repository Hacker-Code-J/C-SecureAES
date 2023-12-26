#ifndef _AES32_H
#define _AES32_H

#include "config.h"

static const u32 rCon32[10] = {
	0x01000000, 0x02000000, 0x04000000, 0x08000000,
	0x10000000, 0x20000000, 0x40000000, 0x80000000,
	0x1b000000, 0x36000000
};

void KeyExpansion32(const u8* uKey, u32* rKey);

void AddRoundKey32(u32* state, const u32* rKey);

void ShiftRows32(u32* state);
void InvShiftRows32(u32* state);

void SubMix32(u32* state);
void InvSubInvMix32(u32* state);

void AES_Encrypt32(const u32* plaintext, const u8* key, u32* ciphertext); 
void AES_Decrypt32(const u32* ciphertext, const u8* key, u32* plaintext);

#endif // _AES32_H