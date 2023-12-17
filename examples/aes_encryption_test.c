#include <stdio.h>
#include <string.h> // For memcpy
#include <time.h>

#include "aes_key_expansion.h"
#include "aes.h"

double measure_time(void (*func)(const u8*, const u8*, u8*), u8* input, u8* key, u8* output) {
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    func(input, key, output);
    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    return cpu_time_used;
}


int main() {
    const char* inputString = "00112233445566778899aabbccddeeff";
    u8 input[16];
    stringToByteArray(inputString, input);
    // Print the plaintext
    printf("Plaintext: ");
    for (int i = 0; i < 16; i++) {
        printf("%02x ", input[i]);
    }
    printf("\n");

    const char* keyString = "000102030405060708090a0b0c0d0e0f";
    u8 key[16];
    stringToByteArray(keyString, key);
    printf("Key: ");
    for (int i = 0; i < 16; i++) {
        printf("%02x ", key[i]);
    }
    printf("\n");

    // Define input plaintext and cipher key in hexadecimal format
    // u8 input[16] = {
    //     0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d,
    //     0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34
    // };
    // u8 key[16] = {
    //     0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
    //     0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
    // };
    
    u8 output[16];

    // Call the AES128_Encrypt function
    AES128_Encrypt(input, key, output);

    // Print the ciphertext
    printf("Ciphertext: ");
    for (int i = 0; i < 16; i++) {
        printf("%02x ", output[i]);
    }
    printf("\n");

    double time_normal = measure_time(AES128_Encrypt, input, key, output);
    double time_optimized = measure_time(AES128_Encrypt_Opt, input, key, output);

    printf("Time for AES128_Encrypt: %.3f s\n", time_normal);
    printf("Time for AES128_Encrypt_Opt: %.3f s\n", time_optimized);


    return 0;
}
