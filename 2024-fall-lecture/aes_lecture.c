#include "aes_lecture.h"

void GF256_print_bin(u8 x) {
    printf("0b");
    for (i8 i = 0; i < 8; i++) {
        printf("%d", (x >> (7 - i)) & 0x01);
    }
    puts("");
}

void GF256_print_poly(u8 a) {
    u8 coef, expo;

    for (i8 i = 0; i < 8; i++) {
        coef = (a >> (7 - i)) & 0x01;
        expo = 7 - i;
        if (i == 7 && coef == 1) {
            printf("%d\n", coef);
        } else {
            if (coef == 1) printf("x^%d + ", expo);
        }
        
    }

}

void print_mat(u8* mat, const char* name_string) {
    
    if(name_string) printf("%s = \n", name_string);
    for (u8 i = 0; i < 4; i++) {
        printf("|");
        for (u8 j = 0; j < 4; j++) {
            printf(" %02x", mat[4 * i + j]);
        }
        puts(" |");
    }
    puts("");
}

u8 GF256_xtime(u8 a) {
    return ((a & 0x80) == 0) ? a << 1 : (a << 1) ^ 0x1b;
}

u8 GF256_mul(u8 f, u8 g) {
    u8 h = 0x00;
    u8 coef;

    for (i8 i = 7; i >= 0; i--) {
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

void MixCol_Mat_Mul(u8* A, u8* B, u8* C) {
    for (u8 i = 0; i < 4; i++) {
        for (u8 j = 0; j < 4; j++) {
            C[4 * i + j] = 0;
            for (u8 k = 0; k < 4; k++) {
                C[4 * i + j] ^= GF256_mul(A[4 * i + k], B[4 * k + j]);
            }
        }
    }
}

void test_MixCol() {
    // [d4, bf, 5d, 30] ---> [04, 66, 81, e5]
    u8 b[4] = { 0xd4, 0xbf, 0x5d, 0x30 };
    u8 c[4] = { 0x00, };
    u8 d[4] = { 0x00, };

    MixCol(b, c);

    printf("b = ");
    for(u8 i = 0; i < 4; i++) printf("%02x ", b[i]);
    puts("");

    printf("c = ");
    for(u8 i = 0; i < 4; i++) printf("%02x ", c[i]);
    puts("");

    InvMixCol(c, d);
    printf("d = ");
    for(u8 i = 0; i < 4; i++) printf("%02x ", d[i]);
    puts("");

}

void test_Mat_Mul() {
    u8 MDS[16] = {
        0x02, 0x03, 0x01, 0x01,
        0x01, 0x02, 0x03, 0x01,
        0x01, 0x01, 0x02, 0x03,
        0x03, 0x01, 0x01, 0x02
    };
    u8 INV_MDS[16] = {
        0x0e, 0x0b, 0x0d, 0x09,
        0x09, 0x0e, 0x0b, 0x0d,
        0x0d, 0x09, 0x0e, 0x0b,
        0x0b, 0x0d, 0x09, 0x0e
    };

    u8 C[16] = { 0x00, };

    MixCol_Mat_Mul(MDS, INV_MDS, C);

    print_mat(MDS, "MDS");
    print_mat(INV_MDS, "INV_MDS");
    print_mat(C, "MDS * INV_MDS");
}

void test_MDS_power() {
    u8 MDS[16] = {
        0x02, 0x03, 0x01, 0x01,
        0x01, 0x02, 0x03, 0x01,
        0x01, 0x01, 0x02, 0x03,
        0x03, 0x01, 0x01, 0x02
    };
    u8 MDS2[16] = { 0x00, };
    u8 MDS3[16] = { 0x00, };
    u8 MDS4[16] = { 0x00, };

    MixCol_Mat_Mul(MDS, MDS, MDS2);
    MixCol_Mat_Mul(MDS2, MDS, MDS3);
    MixCol_Mat_Mul(MDS3, MDS, MDS4);

    print_mat(MDS, "MDS");
    print_mat(MDS2, "MDS^2");
    print_mat(MDS3, "MDS^3");
    print_mat(MDS4, "MDS^4");
}

/* ========================================== */

/*
 * AES Encryption
 * - AddRoundKey
 * (Round Function) - 9 times
 * - SubBytes
 * - ShiftRows
 * - MixColumns
 * - AddRoundKey
 * (Final Round)
 * - SubBytes
 * - ShiftRows
 * - AddRoundKey
 */

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

void AES_Round(u8* state, u8* rk) {
    // print_AES_state(state, "Round IN");
    AES_SubBytes(state);
    // print_AES_state(state, "After SubBytes");
    AES_ShiftRows(state);
    // print_AES_state(state, "After ShiftRows");
    AES_MixColumns(state);
    // print_AES_state(state, "After MixColumns");
    AES_AddRoundKey(state, rk);
    // print_AES_state(state, "After AddRoundKey");
}
void AES_InvRound(u8* state, u8* rk) {
    print_AES_state(state, "Round IN");
    
    AES_AddRoundKey(state, rk);
    print_AES_state(state, "After AddRoundKey");
    
    AES_InvMixColumns(state);
    print_AES_state(state, "After InvMixColumns");
    
    AES_InvShiftRows(state);
    print_AES_state(state, "After InvShiftRows");

    AES_InvSubBytes(state);
    print_AES_state(state, "After InvSubBytes");
    
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

void test_AES_round() {

    u8 PT[16] = {
        0x19, 0x3d, 0xe3, 0xbe,
        0xa0, 0xf4, 0xe2, 0x2b,
        0x9a, 0xc6, 0x8d, 0x2a,
        0xe9, 0xf8, 0x48, 0x08
    };
    u8 RK[16] = {
        0xa0, 0xfa, 0xfe, 0x17,
        0x88, 0x54, 0x2c, 0xb1,
        0x23, 0xa3, 0x39, 0x39,
        0x2a, 0x6c, 0x76, 0x05
    };

    u8 state[16] = { 0x00, };
    // u8 CT[16] = { 0x00, };

    // memcpy(state, PT, 16);
    for (int i = 0; i < 16; i++)
        state[i] = PT[i];
    AES_Round(state, RK);
    print_AES_state(state, NULL);

    AES_InvRound(state, RK);
}





