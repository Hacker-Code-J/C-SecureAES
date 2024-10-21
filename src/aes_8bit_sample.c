#include "aes_8bit_sample.h"

u8 GF256_xtime(u8 a) {
    return ((a & 0x80) == 0) ? a << 1 : (a << 1) ^ 0x1b;
}
u8 GF256_mul(u8 f, u8 g) {
    u8 h = 0x00;
    u8 coef;

    for (int i = 7; i >= 0; i--) {
        coef = (f >> i) & 0x01; // a7, a6, ..., a1, a0
        h = GF256_xtime(h);
        if (coef == 1) {
            h ^= g; 
        }
    }

    return h;
}
u8 GF256_inv(u8 x) {
    if (!x) return 0x00;
    
    u8 x_inv = 0x01;
    u8 temp = x;
    for (i8 i = 0; i < 7; i++) {
        temp = GF256_mul(temp, temp); // x^2, x^4, ..., x^128 (128 = 2^7)
        x_inv = GF256_mul(x_inv, temp); // 1, x^2, x^2 * x^4, ..., x^2 * ...* x^{128}
    }

    return x_inv;
}

void MixCol(u8* in, u8* out) {
    const u8 MDS[16] = {
        0x02, 0x03, 0x01, 0x01,
        0x01, 0x02, 0x03, 0x01,
        0x01, 0x01, 0x02, 0x03,
        0x03, 0x01, 0x01, 0x02
    };

    for (u8 i = 0; i < 4; i++) {
        out[i] = 0x00;
        for (u8 j = 0; j < 4; j++) {
            out[i] ^= GF256_mul(MDS[4 * i + j], in[j]);
        }
    }

    // print_MDS_mat(MDS, NULL);
}
void InvMixCol(u8* in, u8* out) {
    const u8 INV_MDS[16] = {
        0x0e, 0x0b, 0x0d, 0x09,
        0x09, 0x0e, 0x0b, 0x0d,
        0x0d, 0x09, 0x0e, 0x0b,
        0x0b, 0x0d, 0x09, 0x0e
    };

    for (u8 i = 0; i < 4; i++) {
        out[i] = 0x00;
        for (u8 j = 0; j < 4; j++) {
            out[i] ^= GF256_mul(INV_MDS[4*i + j], in[j]);
        }
    }
}

void print_AES_state(u8* state, const char* string) {
    if(string != NULL)
        printf("%s = \n", string);

    // for (int i = 0; i < 16; i++) {
    //     printf("%02x", state[i]);
    //     if ((i % 4) == 3) puts("");
    // }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%02x ", state[4 * j + i]);
        }
        puts("");
    }
    puts("");
}

void AES_SubBytes(u8* state) {
    for (u8 i = 0; i < 16; i++)
        state[i] = Sbox[state[i]];
}
void AES_InvSubBytes(u8* state) {
    for (u8 i = 0; i < 16; i++)
        state[i] = Inv_Sbox[state[i]];
}

/**
 * 0 4  8 12       0  4  8 12
 * 1 5  9 13       5  9 13  1
 * 2 6 10 14  ->  10 14  2  6
 * 3 7 11 15      15  3  7 11
 */
void AES_ShiftRows(u8* state) {
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
/**
 * 0 4  8 12       0  4  8 12
 * 1 5  9 13      13  1  5  9
 * 2 6 10 14  ->  10 14  2  6
 * 3 7 11 15       7 11 15  3
 */
void AES_InvShiftRows(u8* state) {
    u8 temp;
	
    // Row 0: no shift

	// Row 1: shift right by 1
	temp = state[13];
	state[13] = state[9];
	state[9] = state[5];
	state[5] = state[1];
	state[1] = temp;
	
	// Row 2: shift right by 2
	temp = state[2];
	state[2] = state[10];
	state[10] = temp;
	temp = state[6];
	state[6] = state[14];
	state[14] = temp;
	
	// Row 3: shift right by 3 (or left by 1)
	temp = state[3];
	state[3] = state[7];
	state[7] = state[11];
	state[11] = state[15];
	state[15] = temp;
}

void AES_MixColumns(u8* state) {
    u8 in[16] = { 0x00, };
    u8 out[16] = { 0x00, };

    for (int col = 0; col < 4; col++) {
        for (int i = 0; i < 4; i++)
            in[i] = state[4 * col + i];
        // memcpy(in, state, 16);
        MixCol(in, out);
        for (int i = 0; i < 4; i++)
            state[4 * col + i] = out[i];
        // memcpy(state, out, 16);
    }
}
void AES_InvMixColumns(u8* state) {
    u8 in[16] = { 0x00, };
    u8 out[16] = { 0x00, };

    for (int col = 0; col < 4; col++) {
        for (int i = 0; i < 4; i++)
            in[i] = state[4 * col + i];
        InvMixCol(in, out);
        for (int i = 0; i < 4; i++)
            state[4 * col + i] = out[i];
    }
}

void AES_AddRoundKey(u8* state, u8* rk) {
    for (int i = 0; i < 16; i++)
        state[i] ^= rk[i];
}

void AES8_Encrypt(u8 pt[16], u8 rk[11][16], u8 ct[16]) {
    u8 state[16];

    for (int i = 0; i < 16; i++) state[i] = pt[i];

    AES_AddRoundKey(state, rk[0]);

    for (int r = 1; r < 10; r++) {
        AES_SubBytes(state);
        AES_ShiftRows(state);
        AES_MixColumns(state);
        AES_AddRoundKey(state, rk[r]);
    }

    AES_SubBytes(state);
    AES_ShiftRows(state);
    AES_AddRoundKey(state, rk[10]);

    for (int i = 0; i < 16; i++) ct[i] = state[i];
}

void AES8_Decrypt(u8 ct[16], u8 rk[11][16], u8 pt[16]) {
    u8 state[16];

    for (int i = 0; i < 16; i++) state[i] = ct[i];
    AES_AddRoundKey(state, rk[10]);

    for (int r = 9; r > 0; r--) {
        AES_InvShiftRows(state);
        AES_InvSubBytes(state);
        AES_AddRoundKey(state, rk[r]);
        AES_InvMixColumns(state);
    }

    AES_InvShiftRows(state);
    AES_InvSubBytes(state);
    AES_AddRoundKey(state, rk[0]);

    for (int i = 0; i < 16; i++) pt[i] = state[i];
}

void AES8_EqInvKey(u8 rk[11][16], u8 eqrk[11][16]) {
    for (int r = 0; r < 11; r++) {
        for (int i = 0; i < 16; i++) {
            eqrk[r][i] = rk[r][i];
        }
    }

    for (int r = 1; r <= 9; r++)
        AES_InvMixColumns(eqrk[r]);
}

void AES8_EqDecrypt(u8 ct[16], u8 eqrk[11][16], u8 pt[16]) {
    u8 state[16];

    for (int i = 0; i < 16; i++) state[i] = ct[i];

    AES_AddRoundKey(state, eqrk[10]);

    for (int r = 9; r > 0; r--) {
        AES_InvSubBytes(state);
        AES_InvShiftRows(state);
        AES_InvMixColumns(state);
        AES_AddRoundKey(state, eqrk[r]);
    }

    AES_InvSubBytes(state);
    AES_InvShiftRows(state);
    AES_AddRoundKey(state, eqrk[0]);

    for (int i = 0; i < 16; i++) pt[i] = state[i];
}

void TEST_AES8() {
    u8 pt[16] = { 0x00, 0x11, 0x22, 0x33, 
                  0x44, 0x55, 0x66, 0x77, 
                  0x88, 0x99, 0xAA, 0xBB, 
                  0xCC, 0xDD, 0xEE, 0xFF };
    
    u8 rk[11][16] = { 0x00, };
    u8 ct[16] = { 0x00, };

    AES8_Encrypt(pt, rk, ct);
    print_AES_state(ct, "CT");

    u8 dt[16];
    AES8_Decrypt(ct, rk, dt);
    print_AES_state(dt, "DT");

    u8 eqdt[16];

    u8 eqrk[11][16];
    AES8_EqInvKey(rk, eqrk);

    AES8_EqDecrypt(ct, eqrk, eqdt);
    print_AES_state(eqdt, "Eq_DT");
}