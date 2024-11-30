#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t     u8;
typedef uint32_t    u32;

#define GETU32(b) ((u32)(b)[0] << 24) ^ ((u32)(b)[1] << 16) \
            ^ ((u32)(b)[2] << 8) ^ ((u32)(b)[3]);
#define PUTU32(b, x) { \
    (b)[0] = (u8) ((x)>>24); \
    (b)[1] = (u8)((x) >> 16); \
    (b)[2] = (u8) ((x)>>8); \
    (b)[3] = (u8) (x); \
}

void print_AES_state(u8 state[16], const char* pTitle = NULL);
void AES32_Enc_KeySchedule(u8 k[16], u32 rk[11][4]);
void AES32_Dec_KeySchedule(u8 k[16], u32 rk[11][4]);
void u82state(u8 b[16], u32 st[4]);
void state2u8(u32 st[4], u8 b[16]);
void AES8_KeySchedule(u8 k[16], u8 rk[11][16]);
void AES32_EncRound(u32 st[4], u32 rk[4]);
void AES32_EqDecRound(u32 st[4], u32 rk[4]);
void AES32_Encrypt(u8 pt[16], u32 rk[11][4], u8 ct[16]);
void AES32_EqDecrypt(u8 ct[16], u32 rk[11][4], u8 pt[16]);





