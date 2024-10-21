#include "aes_config.h"

static inline u8 xtime(u8 a) {
    return ((a & 0x80) == 0) ? a << 1 : (a << 1) ^ 0x1b;
}

// Te0[x] = [ 2*S(x), S(x), S(x), 3*S(x) ]
// Te1[x] = [ 3*S(x), 2*S(x), S(x), S(x) ]
// Te2[x] = [ 2*S(x), S(x), S(x), 3*S(x) ]
// Te3[x] = [ S(x), S(x), 3*S(x), 2*S(x) ]
// Te4[x] = [ S(x), S(x), S(x), S(x) ]
void AES32_Enc_Table_Generate();

// Td0[x] = [ e*S(x), 9*(x), d*S(x), b*S(x) ]
// Td1[x] = [ 9*(x), d*S(x), b*S(x), e*S(x) ]
// Td2[x] = [ d*S(x), b*S(x), e*S(x), 9*(x) ]
// Td2[x] = [ b*S(x), e*S(x), 9*(x), d*S(x) ]
void AES32_Dec_Table_Generate();


#define GETU32(pt) ((u32)(pt)[0] << 0x18) ^ \
                   ((u32)(pt)[1] << 0x10) ^ \
                   ((u32)(pt)[2] << 0x08) ^ \
                   ((u32)(pt)[3]        )

#define PUTU32(ct, st) { (ct)[0] = (u8)((st) >> 0x18); \
                         (ct)[1] = (u8)((st) >> 0x10); \
                         (ct)[2] = (u8)((st) >> 0x08); \
                         (ct)[3] = (u8)((st)        ); }

static const u32 Rcon[10] = {
    0x01000000, 0x02000000, 0x04000000, 0x08000000,
    0x10000000, 0x20000000, 0x40000000, 0x80000000,
    0x1b000000, 0x36000000
};

void byte2state(u8 b[16], u32 st[4]);
void state2byte(u32 st[4], u8 b[16]);

u32 RotWord(u32 w32);
u32 SubWord(u32 w32);

void AES32_Enc_KeySchedule(u8 k[16], u32 rk[11][4]);
void AES32_Dec_KeySchedule(u8 k[16], u32 rk[11][4]);

void AES8_KeySchedule(u8 k[16], u8 rk[11][16]);

void AES_32_EncRound(u32 st[4], u32 rk[4]);
void AES_32_Encrypt(u8 pt[16], u32 rk[11][4], u8 ct[16]);

void print_AES32_state(u8* state, const char* string);

void TEST_AES32();