#include "aes_config.h"
#include "aes_32bit_sample.h"
#include "aes_table_sample.h"

// 2*y = xtime(y)
// 3*y = xtime(y) ^ y
void AES32_Enc_Table_Generate() {
    u32 Te0[256], Te1[256], Te2[256], Te3[256], Te4[256];
    u8 Sx; // S(x)
    u32 entry;

    for (int i = 0; i < 256; i++) {
        Sx = Sbox[i];
        entry = (xtime(Sx) << 0x18) |
                (     (Sx) << 0x10) |
                (     (Sx) << 0x08) |
                (xtime(Sx) ^ Sx   );
        Te0[i] = entry;
        Te1[i] = (Te0[i] >> 0x08) | (Te0[i] << 0x18);
        Te2[i] = (Te1[i] >> 0x08) | (Te1[i] << 0x18);
        Te3[i] = (Te2[i] >> 0x08) | (Te2[i] << 0x18);
        Te4[i] = (Sx << 0x18) |
                 (Sx << 0x10) |
                 (Sx << 0x08) |
                 (Sx        );
    }

    printf("== AES32 Encryption Table ==\n\n");

    puts("u32 Te0[256] = {");
    for (int i = 0; i < 256; i++) {
        printf("0x%08x, ", Te0[i]);
        if ((i % 4) == 3) printf("\n");
    }
    puts("}");


    puts("u32 Te1[256] = {");
    for (int i = 0; i < 256; i++) {
        printf("0x%08x, ", Te1[i]);
        if ((i % 4) == 3) printf("\n");
    }
    puts("}");


    puts("u32 Te2[256] = {");
    for (int i = 0; i < 256; i++) {
        printf("0x%08x, ", Te2[i]);
        if ((i % 4) == 3) printf("\n");
    }
    puts("}");


    puts("u32 Te3[256] = {");
    for (int i = 0; i < 256; i++) {
        printf("0x%08x, ", Te3[i]);
        if ((i % 4) == 3) printf("\n");
    }
    puts("}");


    puts("u32 Te4[256] = {");
    for (int i = 0; i < 256; i++) {
        printf("0x%08x, ", Te4[i]);
        if ((i % 4) == 3) printf("\n");
    }
    puts("}");

}

// e * y = 1110 * y = 8 * y + 4 * y + 2 * y
// 9 * y = 1001 * y = 8 * y + y
// d * y = 1101 * y = 8 * y + 4 * y + y
// b * y = 1011 * y = 8 * y + 2 * y + y
void AES32_Dec_Table_Generate() {
    u32 Td0[256], Td1[256], Td2[256], Td3[256], Td4[256];
    u8 Sx, Sx02, Sx04, Sx08, Sx0e, Sx09, Sx0d, Sx0b;
    u32 entry;

    for (int i = 0; i < 256; i++) {
        Sx = Inv_Sbox[i];
        Sx02 = xtime(Sx);
        Sx04 = xtime(Sx02);
        Sx08 = xtime(Sx04);
        Sx0e = Sx08 ^ Sx04 ^ Sx02;
        Sx09 = Sx08 ^ Sx02;
        Sx0d = Sx08 ^ Sx04 ^ Sx;
        Sx0b = Sx08 ^ Sx02 ^ Sx02;

        entry = (Sx0e << 0x18) |
                (Sx09 << 0x10) |
                (Sx0d << 0x08) |
                (Sx0b        );
        Td0[i] = entry;
        Td1[i] = (Td0[i] >> 0x08) | (Td0[i] << 0x18);
        Td2[i] = (Td1[i] >> 0x08) | (Td1[i] << 0x18);
        Td3[i] = (Td2[i] >> 0x08) | (Td2[i] << 0x18);
        Td4[i] = (Sx << 0x18) |
                 (Sx << 0x10) |
                 (Sx << 0x08) |
                 (Sx        );
    }

    printf("== AES32 Decryption Table ==\n\n");

    puts("u32 Td0[256] = {");
    for (int i = 0; i < 256; i++) {
        printf("0x%08x, ", Td0[i]);
        if ((i % 4) == 3) printf("\n");
    }
    puts("}");


    puts("u32 Td1[256] = {");
    for (int i = 0; i < 256; i++) {
        printf("0x%08x, ", Td1[i]);
        if ((i % 4) == 3) printf("\n");
    }
    puts("}");


    puts("u32 Td2[256] = {");
    for (int i = 0; i < 256; i++) {
        printf("0x%08x, ", Td2[i]);
        if ((i % 4) == 3) printf("\n");
    }
    puts("}");


    puts("u32 Td3[256] = {");
    for (int i = 0; i < 256; i++) {
        printf("0x%08x, ", Td3[i]);
        if ((i % 4) == 3) printf("\n");
    }
    puts("}");


    puts("u32 Td4[256] = {");
    for (int i = 0; i < 256; i++) {
        printf("0x%08x, ", Td4[i]);
        if ((i % 4) == 3) printf("\n");
    }
    puts("}");

}

void byte2state(u8 b[16], u32 st[4]) {
    st[0] = GETU32(b);
    st[1] = GETU32(b + 0x04);
    st[2] = GETU32(b + 0x08);
    st[3] = GETU32(b + 0x0C);
}

void state2byte(u32 st[4], u8 b[16]) {
    PUTU32(b       , st[0]);
    PUTU32(b + 0x04, st[1]);
    PUTU32(b + 0x08, st[2]);
    PUTU32(b + 0x0C, st[3]);
}

u32 RotWord(u32 w32) {
    return (w32 << 0x08) | (w32 >> 0x18);
}

u32 SubWord(u32 w32) {
    return (u32)Sbox[(w32 >> 0x18)] << 0x18 | 
           (u32)Sbox[(w32 >> 0x10) & 0xFF] << 0x10 | 
           (u32)Sbox[(w32 >> 0x08) & 0xFF] << 0x08 | 
           (u32)Sbox[(w32 & 0xFF)];
}

void AES32_Enc_KeySchedule(u8 k[16], u32 rk[11][4]) {
    u32 tmp;
    int rcon_cnt = 0x00;

    for (int i = 0; i < 4; i++) {
        rk[0][i] = GETU32(k + 4 * i);
    }
    
    for (int i = 1; i < 11; i++) {
        tmp = rk[i - 1][3];
        for (int k = 0; k < 4; k++) {
            if (k == 0) {
                tmp = SubWord(RotWord(tmp)) ^ Rcon[rcon_cnt];   // tmp = F(tmp)
                rcon_cnt++;
            }
            rk[i][k] = rk[i - 1][k] ^ tmp;
            tmp = rk[i][k];
        }
    }
}

void AES32_Dec_KeySchedule(u8 k[16], u32 rk[11][4]) {
    AES32_Enc_KeySchedule(k, rk);

    // Apply InvMixCol to 1-9 rounds

    // Te4[x] = [ S[x], S[x], S[x], S[x] ]
    // Tdi[] : InvMixCol(InvSub(Sub()))

    // How to simple it?
    for (int i = 1; i < 10; i++) {
        rk[i][0] = Td0[Te4[(rk[i][0] >> 0x18)       ] & 0xFF] ^
                   Td1[Te4[(rk[i][0] >> 0x10) & 0xFF] & 0xFF] ^
                   Td2[Te4[(rk[i][0] >> 0x08) & 0xFF] & 0xFF] ^
                   Td3[Te4[(rk[i][0]        ) & 0xFF] & 0xFF];
        
        rk[i][1] = Td0[Te4[(rk[i][1] >> 0x18)       ] & 0xFF] ^
                   Td1[Te4[(rk[i][1] >> 0x10) & 0xFF] & 0xFF] ^
                   Td2[Te4[(rk[i][1] >> 0x08) & 0xFF] & 0xFF] ^
                   Td3[Te4[(rk[i][1]        ) & 0xFF] & 0xFF];
        
        rk[i][2] = Td0[Te4[(rk[i][2] >> 0x18)       ] & 0xFF] ^
                   Td1[Te4[(rk[i][2] >> 0x10) & 0xFF] & 0xFF] ^
                   Td2[Te4[(rk[i][2] >> 0x08) & 0xFF] & 0xFF] ^
                   Td3[Te4[(rk[i][2]        ) & 0xFF] & 0xFF];
        
        rk[i][3] = Td0[Te4[(rk[i][3] >> 0x18)       ] & 0xFF] ^
                   Td1[Te4[(rk[i][3] >> 0x10) & 0xFF] & 0xFF] ^
                   Td2[Te4[(rk[i][3] >> 0x08) & 0xFF] & 0xFF] ^
                   Td3[Te4[(rk[i][3]        ) & 0xFF] & 0xFF];
        
    }
}

void AES8_KeySchedule(u8 k[16], u8 rk[11][16]) {
    u32 rk32[11][4];

    AES32_Enc_KeySchedule(k, rk32);
    for (int i = 0; i < 11; i++)
        state2byte(rk32[i], rk[i]);
}

void AES_32_EncRound(u32 st[4], u32 rk[4]) {
    u32 tmp[4];

    tmp[0] = Te0[(st[0] >> 0x18)       ] ^ 
             Te1[(st[1] >> 0x10) & 0xFF] ^
             Te2[(st[2] >> 0x08) & 0xFF] ^
             Te3[(st[3]        ) & 0xFF] ^ rk[0];
    
    tmp[1] = Te0[(st[1] >> 0x18)       ] ^
             Te1[(st[2] >> 0x10) & 0xFF] ^
             Te2[(st[3] >> 0x08) & 0xFF] ^
             Te3[(st[0]        ) & 0xFF] ^ rk[1];

    tmp[2] = Te0[(st[2] >> 0x18)       ] ^
             Te1[(st[3] >> 0x10) & 0xFF] ^
             Te2[(st[0] >> 0x08) & 0xFF] ^
             Te3[(st[1]        ) & 0xFF] ^ rk[2];

    tmp[3] = Te0[(st[3] >> 0x18)       ] ^
             Te1[(st[0] >> 0x10) & 0xFF] ^
             Te2[(st[1] >> 0x08) & 0xFF] ^
             Te3[(st[2]        ) & 0xFF] ^ rk[3];

    for (int k = 0; k < 4; k++)
        st[k] = tmp[4];
}

void AES_32_Encrypt(u8 pt[16], u32 rk[11][4], u8 ct[16]) {
    u32 st[4], tmp[4];

    byte2state(pt, st);

    for (int k = 0; k < 4; k++) st[k] ^= rk[0][k];
    for (int r = 1; r < 10; r++) AES_32_EncRound(st, rk[r]);

    tmp[0] = (Te4[(st[0] >> 0x18)       ] & 0xFF000000) ^
			 (Te4[(st[1] >> 0x10) & 0xFF] & 0x00FF0000) ^
			 (Te4[(st[2] >> 0x08) & 0xFF] & 0x0000FF00) ^
			 (Te4[st[3] 		  & 0xFF] & 0x000000FF) ^ rk[10][0];

	tmp[1] = (Te4[(st[1] >> 0x18)       ] & 0xFF000000) ^
			 (Te4[(st[2] >> 0x10) & 0xFF] & 0x00FF0000) ^
			 (Te4[(st[3] >> 0x08) & 0xFF] & 0x0000FF00) ^
			 (Te4[(st[0]        ) & 0xFF] & 0x000000FF) ^ rk[10][1];

	tmp[2] = (Te4[(st[2] >> 0x18)       ] & 0xFF000000) ^
			 (Te4[(st[3] >> 0x10) & 0xFF] & 0x00FF0000) ^
			 (Te4[(st[0] >> 0x08) & 0xFF] & 0x0000FF00) ^
			 (Te4[(st[1]        ) & 0xFF] & 0x000000FF) ^ rk[10][2];
	
	tmp[3] = (Te4[(st[3] >> 0x18)       ] & 0xFF000000) ^
			 (Te4[(st[0] >> 0x10) & 0xFF] & 0x00FF0000) ^
			 (Te4[(st[1] >> 0x08) & 0xFF] & 0x0000FF00) ^
			 (Te4[(st[2]        ) & 0xFF] & 0x000000FF) ^ rk[10][3];

    state2byte(tmp, ct);
}

void print_AES32_state(u8* state, const char* string) {
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

void TEST_AES32() {
    u8 pt[16] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                  0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };

    u8 key[16] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                   0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
    
    // u8 correct_ct[16] = { 0x69, 0xc4, 0xe0, 0xd8, 0x6a };

    u32 rk32[11][4];
    u8 ct[16];

    AES32_Enc_KeySchedule(key, rk32);
    AES_32_Encrypt(pt, rk32, ct);

    print_AES32_state(pt, "PT");
    print_AES32_state(ct, "CT");
}