#include "aes_utils.h"
#include "aes.h"
#include "aes32.h"

u8 mul1(u8 a, u8 b) {
    u8 res = 0;
    for (int i = 0; i < 8; i++) {
        res ^= (b & 1) * a;  
        b >>= 1;             // Prepare for the next iteration (b = b / 2).

        u8 MSB = a & 0x80;
        a <<= 1;             // a = a * 2.
        a ^= (0 - (MSB >> 7)) & 0x1B;  // Conditional XOR without branching.
    }

    return res;
}

u8 mul2_sub(u8 f) {
    return ((f & 0x80) == 0) ? f << 1 : (f << 1) ^ 0x1b;
}

u8 mul2(u8 f, u8 g) {
    u8 h = 0x00; // h = f * g
    u8 coef;
	
    for (i8 i = 7; i >= 0; i--) {
        coef = (f >> i) & 0x01; // Get the coefficient f_i
        h = mul2_sub(h);     // Multiply h by x
        if (coef == 1) h ^= g;  // If f_i = 1, add g(x) to h(x)
    }

	return h;
}

void GF_MUL_Test() {
    for (u8 i = 0; i < 100; i++) {
        // printf("%lu\n", measure_mul_cycle(mul1, 0xFF, 0xFF));
        // printf("%lu\n", measure_mul_cycle(mul2, 0xFF, 0xFF));
        printf("%.3f\n", measure_mul_time(mul1, 0xFF, 0xFF)*1000000);
        printf("%.3f\n", measure_mul_time(mul2, 0xFF, 0xFF)*1000000);
    }
    // printf("%d %d", mul1(0xFF, 0xFF), mul2(0xFF, 0xFF));
}

void AES128_Test() {
    const char* inputString = "e0000000000000000000000000000000";
    u8 input[16];
    stringToByteArray(input, inputString);
    // Print the plaintext
    printf("Plaintext(08-bit): ");
    for (int i = 0; i < 16; i++) {
        printf("%02x", input[i]);
    }
    printf("\n");

    const char* keyString = "00000000000000000000000000000000";
    u8 key[16];
    stringToByteArray(key, keyString);
    printf("Key: ");
    for (int i = 0; i < 16; i++) {
        printf("%02x", key[i]);
    }
    printf("\n");
    
    u8 output[16];
    u8 output2[16];

    // Call the AES_Encrypt function
    AES_Encrypt(output, input, key, AES128);
    AES32_Encrypt(output2, input, key, AES128);
    
    // Print the ciphertext
    printf("Ciphertext(08-bit): \n");
    for (int i = 0; i < 16; i++) {
        printf("%02x", output[i]);
    }
    printf("\n");
    printf("08-BIT-AES-CYCLE: %lu cycles\n", measure_encryption_cycle(AES_Encrypt, output, input, key, AES128));
    printf("08-BIT-AES--TIME: %.3f µs\n", measure_encryption_time(AES_Encrypt, output, input, key, AES128)*1000000);

    printf("Ciphertext(32-bit): \n");
    for (int i = 0; i < 16; i++) {
        printf("%02x", output2[i]);
    }
    printf("\n");
    printf("32-BIT-AES-CYCLE: %lu cycles\n", measure_encryption_cycle(AES32_Encrypt, output, input, key, AES128));
    printf("32-BIT-AES--TIME: %.3f µs\n", measure_encryption_time(AES32_Encrypt, output, input, key, AES128)*1000000);
}

void AES128_Opt_Comp() {
    const char* inputString = "e0000000000000000000000000000000";
    u8 input[16];
    stringToByteArray(input, inputString);

    const char* keyString = "00000000000000000000000000000000";
    u8 key[16];
    stringToByteArray(key, keyString);
    
    u8 output[16];

    for (u8 i = 0; i < 100; i++) {
        printf("%.3f\n", measure_encryption_time(AES_Encrypt, output, input, key, AES128)*1000000);
        printf("%.3f\n", measure_encryption_time(AES32_Encrypt, output, input, key, AES128)*1000000);
        // printf("%lu\n", measure_encryption_cycle(AES_Encrypt, output, input, key, AES128));
        // printf("%lu\n", measure_encryption_cycle(AES32_Encrypt, output, input, key, AES128));
    }
}

