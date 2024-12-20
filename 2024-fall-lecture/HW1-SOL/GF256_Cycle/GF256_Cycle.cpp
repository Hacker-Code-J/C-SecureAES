﻿// HW1.cpp 
//

#include <stdio.h>
//#include "GF2.h"
#include "GF256.h"

// [5-(e)]
byte GF_inv_using_cyclic(byte x) {
    // [5-(d)]의 출력으로 만든 테이블을 사용한다.    
    byte LogTable[256] = {
     0x00, 0x00, 0x8c, 0x80, 0x19, 0x01, 0x0d, 0x63, 0xa5, 0xe3, 0x8d, 0x34, 0x99, 0x77, 0xef, 0x81,
     0x32, 0x02, 0x70, 0x07, 0x1a, 0xc6, 0xc0, 0xf7, 0x26, 0xb8, 0x04, 0x64, 0x7c, 0xb4, 0x0e, 0xe0,
     0xbe, 0x61, 0x8e, 0xda, 0xfc, 0xdc, 0x93, 0x35, 0xa6, 0x72, 0x53, 0x39, 0x4d, 0xe4, 0x84, 0x3c,
     0xb2, 0x97, 0x45, 0x82, 0x90, 0x87, 0xf0, 0x12, 0x09, 0x78, 0x41, 0xa2, 0x9a, 0xc9, 0x6d, 0x47,
     0x4b, 0xc7, 0xed, 0xde, 0x1b, 0x68, 0x67, 0x4a, 0x89, 0x2e, 0x69, 0xf8, 0x20, 0x23, 0xc1, 0x1c,
     0x33, 0xee, 0xfe, 0x18, 0xdf, 0x03, 0xc5, 0x31, 0xd9, 0x92, 0x71, 0x4c, 0x11, 0x44, 0xc8, 0x08,
     0x3f, 0x37, 0x24, 0xe1, 0xd1, 0x5b, 0x0f, 0x21, 0x1d, 0xb5, 0x14, 0x2a, 0x7d, 0xc2, 0x9e, 0x5d,
     0x95, 0xbc, 0x05, 0x8a, 0xcd, 0xcf, 0x2f, 0x65, 0x27, 0x6a, 0x56, 0xf2, 0xf9, 0xb9, 0xd3, 0xab,
     0xd7, 0x2c, 0x54, 0x28, 0x7a, 0x75, 0x6b, 0x3a, 0xa7, 0x57, 0xf4, 0xea, 0xf3, 0x73, 0xd6, 0x74,
     0x16, 0xeb, 0xba, 0x3d, 0xf5, 0x0b, 0x85, 0xfa, 0xac, 0xe5, 0xaf, 0x58, 0x4e, 0xd4, 0xa8, 0x50,
     0xbf, 0x06, 0x7b, 0xb7, 0x8b, 0x62, 0xa4, 0x76, 0x6c, 0xa1, 0x8f, 0x96, 0x52, 0x3b, 0xbd, 0xdb,
     0x66, 0xdd, 0x1f, 0x2d, 0xfd, 0x30, 0xd8, 0x43, 0x9d, 0x29, 0xd0, 0x36, 0x55, 0xaa, 0x94, 0xce,
     0xcb, 0x59, 0xc3, 0x48, 0xb0, 0x5f, 0x6e, 0x7e, 0x5e, 0xca, 0xe7, 0xe6, 0x9b, 0x9f, 0xad, 0xe8,
     0xa9, 0x9c, 0x42, 0x1e, 0xa0, 0x51, 0xb6, 0xa3, 0x0a, 0x15, 0x4f, 0xae, 0x2b, 0x79, 0xe9, 0xd5,
     0x22, 0x88, 0x49, 0xec, 0x91, 0x10, 0x17, 0xc4, 0x5a, 0x3e, 0x5c, 0x13, 0xbb, 0xcc, 0xf1, 0xd2,
     0xb3, 0x25, 0xf6, 0x6f, 0xe2, 0x98, 0x7f, 0x0c, 0x86, 0xb1, 0x46, 0x40, 0x60, 0xfb, 0x38, 0x83, };
    byte ExpTable[256] = {
     0x01, 0x05, 0x11, 0x55, 0x1a, 0x72, 0xa1, 0x13, 0x5f, 0x38, 0xd8, 0x95, 0xf7, 0x06, 0x1e, 0x66,
     0xe5, 0x5c, 0x37, 0xeb, 0x6a, 0xd9, 0x90, 0xe6, 0x53, 0x04, 0x14, 0x44, 0x4f, 0x68, 0xd3, 0xb2,
     0x4c, 0x67, 0xe0, 0x4d, 0x62, 0xf1, 0x18, 0x78, 0x83, 0xb9, 0x6b, 0xdc, 0x81, 0xb3, 0x49, 0x76,
     0xb5, 0x57, 0x10, 0x50, 0x0b, 0x27, 0xbb, 0x61, 0xfe, 0x2b, 0x87, 0xad, 0x2f, 0x93, 0xe9, 0x60,
     0xfb, 0x3a, 0xd2, 0xb7, 0x5d, 0x32, 0xfa, 0x3f, 0xc3, 0xe2, 0x47, 0x40, 0x5b, 0x2c, 0x9c, 0xda,
     0x9f, 0xd5, 0xac, 0x2a, 0x82, 0xbc, 0x7a, 0x89, 0x9b, 0xc1, 0xe8, 0x65, 0xea, 0x6f, 0xc8, 0xc5,
     0xfc, 0x21, 0xa5, 0x07, 0x1b, 0x77, 0xb0, 0x46, 0x45, 0x4a, 0x79, 0x86, 0xa8, 0x3e, 0xc6, 0xf3,
     0x12, 0x5a, 0x29, 0x8d, 0x8f, 0x85, 0xa7, 0x0d, 0x39, 0xdd, 0x84, 0xa2, 0x1c, 0x6c, 0xc7, 0xf6,
     0x03, 0x0f, 0x33, 0xff, 0x2e, 0x96, 0xf8, 0x35, 0xe1, 0x48, 0x73, 0xa4, 0x02, 0x0a, 0x22, 0xaa,
     0x34, 0xe4, 0x59, 0x26, 0xbe, 0x70, 0xab, 0x31, 0xf5, 0x0c, 0x3c, 0xcc, 0xd1, 0xb8, 0x6e, 0xcd,
     0xd4, 0xa9, 0x3b, 0xd7, 0xa6, 0x08, 0x28, 0x88, 0x9e, 0xd0, 0xbd, 0x7f, 0x98, 0xce, 0xdb, 0x9a,
     0xc4, 0xf9, 0x30, 0xf0, 0x1d, 0x69, 0xd6, 0xa3, 0x19, 0x7d, 0x92, 0xec, 0x71, 0xae, 0x20, 0xa0,
     0x16, 0x4e, 0x6d, 0xc2, 0xe7, 0x56, 0x15, 0x41, 0x5e, 0x3d, 0xc9, 0xc0, 0xed, 0x74, 0xbf, 0x75,
     0xba, 0x64, 0xef, 0x7e, 0x9d, 0xdf, 0x8e, 0x80, 0xb6, 0x58, 0x23, 0xaf, 0x25, 0xb1, 0x43, 0x54,
     0x1f, 0x63, 0xf4, 0x09, 0x2d, 0x99, 0xcb, 0xca, 0xcf, 0xde, 0x8b, 0x91, 0xe3, 0x42, 0x51, 0x0e,
     0x36, 0xee, 0x7b, 0x8c, 0x8a, 0x94, 0xf2, 0x17, 0x4b, 0x7c, 0x97, 0xfd, 0x24, 0xb4, 0x52, 0x01, };

    byte ax, result;

    ax = LogTable[x];
    //같은 결과의 코드: result = ExpTable[(ax * 254) % 255];
    result = ExpTable[255 - ax];

    return result;
}

//3-(e) GF_mul_cyclic() 함수 만들기
byte GF_mul_cycle(byte x, byte y) {
    // [5-(d)]의 출력으로 만든 테이블을 사용한다.    
    byte LogTable[256] = {
     0x00, 0x00, 0x8c, 0x80, 0x19, 0x01, 0x0d, 0x63, 0xa5, 0xe3, 0x8d, 0x34, 0x99, 0x77, 0xef, 0x81,
     0x32, 0x02, 0x70, 0x07, 0x1a, 0xc6, 0xc0, 0xf7, 0x26, 0xb8, 0x04, 0x64, 0x7c, 0xb4, 0x0e, 0xe0,
     0xbe, 0x61, 0x8e, 0xda, 0xfc, 0xdc, 0x93, 0x35, 0xa6, 0x72, 0x53, 0x39, 0x4d, 0xe4, 0x84, 0x3c,
     0xb2, 0x97, 0x45, 0x82, 0x90, 0x87, 0xf0, 0x12, 0x09, 0x78, 0x41, 0xa2, 0x9a, 0xc9, 0x6d, 0x47,
     0x4b, 0xc7, 0xed, 0xde, 0x1b, 0x68, 0x67, 0x4a, 0x89, 0x2e, 0x69, 0xf8, 0x20, 0x23, 0xc1, 0x1c,
     0x33, 0xee, 0xfe, 0x18, 0xdf, 0x03, 0xc5, 0x31, 0xd9, 0x92, 0x71, 0x4c, 0x11, 0x44, 0xc8, 0x08,
     0x3f, 0x37, 0x24, 0xe1, 0xd1, 0x5b, 0x0f, 0x21, 0x1d, 0xb5, 0x14, 0x2a, 0x7d, 0xc2, 0x9e, 0x5d,
     0x95, 0xbc, 0x05, 0x8a, 0xcd, 0xcf, 0x2f, 0x65, 0x27, 0x6a, 0x56, 0xf2, 0xf9, 0xb9, 0xd3, 0xab,
     0xd7, 0x2c, 0x54, 0x28, 0x7a, 0x75, 0x6b, 0x3a, 0xa7, 0x57, 0xf4, 0xea, 0xf3, 0x73, 0xd6, 0x74,
     0x16, 0xeb, 0xba, 0x3d, 0xf5, 0x0b, 0x85, 0xfa, 0xac, 0xe5, 0xaf, 0x58, 0x4e, 0xd4, 0xa8, 0x50,
     0xbf, 0x06, 0x7b, 0xb7, 0x8b, 0x62, 0xa4, 0x76, 0x6c, 0xa1, 0x8f, 0x96, 0x52, 0x3b, 0xbd, 0xdb,
     0x66, 0xdd, 0x1f, 0x2d, 0xfd, 0x30, 0xd8, 0x43, 0x9d, 0x29, 0xd0, 0x36, 0x55, 0xaa, 0x94, 0xce,
     0xcb, 0x59, 0xc3, 0x48, 0xb0, 0x5f, 0x6e, 0x7e, 0x5e, 0xca, 0xe7, 0xe6, 0x9b, 0x9f, 0xad, 0xe8,
     0xa9, 0x9c, 0x42, 0x1e, 0xa0, 0x51, 0xb6, 0xa3, 0x0a, 0x15, 0x4f, 0xae, 0x2b, 0x79, 0xe9, 0xd5,
     0x22, 0x88, 0x49, 0xec, 0x91, 0x10, 0x17, 0xc4, 0x5a, 0x3e, 0x5c, 0x13, 0xbb, 0xcc, 0xf1, 0xd2,
     0xb3, 0x25, 0xf6, 0x6f, 0xe2, 0x98, 0x7f, 0x0c, 0x86, 0xb1, 0x46, 0x40, 0x60, 0xfb, 0x38, 0x83, };
    byte ExpTable[256] = {
     0x01, 0x05, 0x11, 0x55, 0x1a, 0x72, 0xa1, 0x13, 0x5f, 0x38, 0xd8, 0x95, 0xf7, 0x06, 0x1e, 0x66,
     0xe5, 0x5c, 0x37, 0xeb, 0x6a, 0xd9, 0x90, 0xe6, 0x53, 0x04, 0x14, 0x44, 0x4f, 0x68, 0xd3, 0xb2,
     0x4c, 0x67, 0xe0, 0x4d, 0x62, 0xf1, 0x18, 0x78, 0x83, 0xb9, 0x6b, 0xdc, 0x81, 0xb3, 0x49, 0x76,
     0xb5, 0x57, 0x10, 0x50, 0x0b, 0x27, 0xbb, 0x61, 0xfe, 0x2b, 0x87, 0xad, 0x2f, 0x93, 0xe9, 0x60,
     0xfb, 0x3a, 0xd2, 0xb7, 0x5d, 0x32, 0xfa, 0x3f, 0xc3, 0xe2, 0x47, 0x40, 0x5b, 0x2c, 0x9c, 0xda,
     0x9f, 0xd5, 0xac, 0x2a, 0x82, 0xbc, 0x7a, 0x89, 0x9b, 0xc1, 0xe8, 0x65, 0xea, 0x6f, 0xc8, 0xc5,
     0xfc, 0x21, 0xa5, 0x07, 0x1b, 0x77, 0xb0, 0x46, 0x45, 0x4a, 0x79, 0x86, 0xa8, 0x3e, 0xc6, 0xf3,
     0x12, 0x5a, 0x29, 0x8d, 0x8f, 0x85, 0xa7, 0x0d, 0x39, 0xdd, 0x84, 0xa2, 0x1c, 0x6c, 0xc7, 0xf6,
     0x03, 0x0f, 0x33, 0xff, 0x2e, 0x96, 0xf8, 0x35, 0xe1, 0x48, 0x73, 0xa4, 0x02, 0x0a, 0x22, 0xaa,
     0x34, 0xe4, 0x59, 0x26, 0xbe, 0x70, 0xab, 0x31, 0xf5, 0x0c, 0x3c, 0xcc, 0xd1, 0xb8, 0x6e, 0xcd,
     0xd4, 0xa9, 0x3b, 0xd7, 0xa6, 0x08, 0x28, 0x88, 0x9e, 0xd0, 0xbd, 0x7f, 0x98, 0xce, 0xdb, 0x9a,
     0xc4, 0xf9, 0x30, 0xf0, 0x1d, 0x69, 0xd6, 0xa3, 0x19, 0x7d, 0x92, 0xec, 0x71, 0xae, 0x20, 0xa0,
     0x16, 0x4e, 0x6d, 0xc2, 0xe7, 0x56, 0x15, 0x41, 0x5e, 0x3d, 0xc9, 0xc0, 0xed, 0x74, 0xbf, 0x75,
     0xba, 0x64, 0xef, 0x7e, 0x9d, 0xdf, 0x8e, 0x80, 0xb6, 0x58, 0x23, 0xaf, 0x25, 0xb1, 0x43, 0x54,
     0x1f, 0x63, 0xf4, 0x09, 0x2d, 0x99, 0xcb, 0xca, 0xcf, 0xde, 0x8b, 0x91, 0xe3, 0x42, 0x51, 0x0e,
     0x36, 0xee, 0x7b, 0x8c, 0x8a, 0x94, 0xf2, 0x17, 0x4b, 0x7c, 0x97, 0xfd, 0x24, 0xb4, 0x52, 0x01, };

    byte kx, ky, kz, z;

    kx = LogTable[x];
    ky = LogTable[y];

    kz = (kx + ky) % 255;

    z = ExpTable[kz];

    return z;
}

void HW1_Prob5() {

    printf("=== [HW1 - Problem 5] ===\n");
    printf("=== [5-(a)]\n");
    int odr, num_gen;
    byte xk;
    num_gen = 0;

    for (int x = 1; x <= 255; x++) {
        odr = 0;
        xk = x;
        for (int k = 1; k <= 255; k++) {
            if (xk == 1) {
                printf("order(%d)= %d\n", x, k);
                if (k == 255) {
                    num_gen++;
                }
                break;
            }
            xk = GF256_mul(xk, x);
        }
    }
    printf("The number of generator is %d.\n", num_gen);

    //5-(b)
    printf("\n=== [5-(b)]\n");
    int gen1 = 0x03;
    int gen2 = 0x05; // 다른 생성자를 선택한다.
    byte gen1_pow, gen2_pow;

    gen1_pow = 0x01;
    gen2_pow = 0x01;
    for (int i = 0; i <= 255; i++) {
        printf("a^%d | 0x%02x | 0x%02x \n", i, gen1_pow, gen2_pow);
        gen1_pow = GF256_mul(gen1_pow, gen1);
        gen2_pow = GF256_mul(gen2_pow, gen2);
    }
    //5-(c)
    printf("\n=== [5-(c)]\n");
    byte x = 0xa1;
    byte y = 0x63;
    byte akx, aky, akz;
    int kx, ky, kz;
    int generator;

    generator = gen1;
    printf("Using generator1 = 0x%02x\n", generator);
    akx = generator;
    aky = generator;
    for (int i = 1; i <= 255; i++) {
        if (akx == x) {
            kx = i;
            break;
        }
        akx = GF256_mul(akx, generator);
    }
    for (int i = 1; i <= 255; i++) {
        if (aky == y) {
            ky = i;
            break;
        }
        aky = GF256_mul(aky, generator);
    }
    kz = (kx + ky) % 255;
    akz = 1;
    for (int i = 1; i <= kz; i++) {
        akz = GF256_mul(akz, generator);
    }
    printf(" x = 0x%02x = 0x%02x ^ %d \n", x, generator, kx);
    printf(" y = 0x%02x = 0x%02x ^ %d \n", y, generator, ky);
    printf(" z = x*y = 0x%02x = 0x%02x ^ %d \n", akz, generator, kz);
    //확인용! printf(" x*y = 0x%02x * 0x%02x = 0x%02x\n", x, y, GF_mul(x, y));

    generator = gen2;
    printf("Using generator2 = 0x%02x\n", generator);
    akx = generator;
    aky = generator;
    for (int i = 1; i <= 255; i++) {
        if (akx == x) {
            kx = i;
            break;
        }
        akx = GF256_mul(akx, generator);
    }
    for (int i = 1; i <= 255; i++) {
        if (aky == y) {
            ky = i;
            break;
        }
        aky = GF256_mul(aky, generator);
    }
    kz = (kx + ky) % 255;
    akz = 1;
    for (int i = 1; i <= kz; i++) {
        akz = GF256_mul(akz, generator);
    }
    printf(" x = 0x%02x = 0x%02x ^ %d \n", x, generator, kx);
    printf(" y = 0x%02x = 0x%02x ^ %d \n", y, generator, ky);
    printf(" z = x*y = 0x%02x = 0x%02x ^ %d \n", akz, generator, kz);
    //확인용! printf(" x*y = 0x%02x * 0x%02x = 0x%02x\n", x, y, GF_mul(x, y));

    //5-(d)
    printf("\n=== [5-(d)]\n");
    byte base_x = gen2;
    byte pow_x = 1;
    byte LogTable[256];
    LogTable[0] = 0; //0은 예외
    for (int i = 1; i <= 255; i++) { // 실제로 i<=254까지만 해도 된다 (a^255=1)
        pow_x = GF256_mul(base_x, pow_x);
        LogTable[pow_x] = i;
    }
    LogTable[1] = 0; // a^0 = a^(255 mod (2^8-1)) = 1

    //LogTable의 역함수 테이블 만들기
    byte ExpTable[256];
    for (int i = 0; i < 256; i++) {
        ExpTable[LogTable[i]] = i;
    }
    ExpTable[255] = 0x01;  // a^255 = 1 이므로 (LogTable[]에는 255가 없다)
    printf("byte LogTable[256] = {");
    for (int i = 0; i < 256; i++) {
        if ((i % 16) == 0) printf("\n");
        printf(" 0x%02x,", LogTable[i]);
    }
    printf("};\n");
    printf("byte ExpTable[256] = {");
    for (int i = 0; i < 256; i++) {
        if ((i % 16) == 0) printf("\n");
        printf(" 0x%02x,", ExpTable[i]);
    }
    printf("};\n");

    //5-(e)
    printf("\n=== [5-(e)]\n");
    x = 0xa1;
    y = 0x63;
    byte z;
    z = GF_mul_cycle(x, y);
    printf(" x = 0x%02x = 0x%02x ^ %d \n", x, generator, kx);
    printf(" y = 0x%02x = 0x%02x ^ %d \n", y, generator, ky);
    printf(" z = %02x = x*y = 0x%02x = 0x%02x ^ %d \n", z, akz, generator, kz);

}



int main()
{
    HW1_Prob5();
}
