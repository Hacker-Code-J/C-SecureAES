#ifndef _AES32_H
#define _AES32_H

#include "utils.h"

void AddRoundKey32(u32* state, const u32* rKey);

void ShiftRows32(u32* state);
void InvShiftRows32(u32* state);

void SubMix32(u32* state);
void InvSubInvMix32(u32* state);

void AES_Encrypt32(const u32* plaintext, const u8* key, u32* ciphertext); 
void AES_Decrypt32(const u32* ciphertext, const u8* key, u32* plaintext);

#endif // _AES32_H