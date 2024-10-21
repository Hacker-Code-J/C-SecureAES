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





