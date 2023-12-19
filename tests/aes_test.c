#include <stdio.h>
#include <stdlib.h>
#include <string.h> // For memcpy
#include <time.h>

#include "aes_key_expansion.h"
#include "aes.h"

double measure_time(void (*func)(const u8*, const u8*, u8*), u8* input, u8* key, u8* output) {
    srand((u32)time(NULL));
    // clock_t start, end;
    // double cpu_time_used;

    // start = clock();
    // func(input, key, output);
    // end = clock();

    // cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    // return cpu_time_used;

    struct timespec start, end;
    double cpu_time_used;
    const int num_runs = 1000; // Number of runs for averaging

    // Warm-up run (optional, but often a good idea)
    func(input, key, output);

    clock_gettime(1, &start);  // Start timing
    // clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < num_runs; i++) {
        func(input, key, output);
    }
    clock_gettime(1, &end); // End timing
    // clock_gettime(CLOCK_MONOTONIC, &end);

    // Calculate elapsed time in seconds
    cpu_time_used = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    
    return cpu_time_used / num_runs; // Average time per run
}


int main() {
    const char* inputString = "f34481ec3cc627bacd5dc3fb08f273e6";
    u8 input[16];
    stringToByteArray(inputString, input);
    // Print the plaintext
    printf("Plaintext: ");
    for (int i = 0; i < 16; i++) {
        printf("%02x", input[i]);
    }
    printf("\n");

    const char* keyString = "00000000000000000000000000000000";
    u8 key[AES_VERSION / 8];
    stringToByteArray(keyString, key);
    printf("Key: ");
    for (int i = 0; i < AES_VERSION / 8; i++) {
        printf("%02x", key[i]);
    }
    printf("\n");
    
    u8 output[16];

    // Call the AES_Encrypt function
    AES_Encrypt(input, key, output);

    // Print the ciphertext
    printf("Ciphertext: ");
    for (int i = 0; i < 16; i++) {
        printf("%02x", output[i]);
    }
    printf("\n\n");

    u8 decrypted[16] = { 0x00, };

    // Call the AES_Decrypt function
    AES_Decrypt(output, key, decrypted);

    printf("Decrypted-text: ");
    for (int i = 0; i < 16; i++) {
        printf("%02x", decrypted[i]);
    }
    printf("\n\n");

    double time_enc = measure_time(AES_Encrypt, input, key, output);
    double time_enc_opt = measure_time(AES_Encrypt_Opt, input, key, output);
    double time_enc_32 = measure_time(AES_Encrypt_32BIT, input, key, output);
    double time_dec = measure_time(AES_Decrypt, input, key, output);
    double time_dec_opt = measure_time(AES_Decrypt_Opt, input, key, output);
    double time_dec_32 = measure_time(AES_Decrypt_32BIT, input, key, output);

    printf("Time for AES_Encrypt: %.9f s\n", time_enc);
    printf("Time for AES_Encrypt_Opt: %.9f s\n", time_enc_opt);
    printf("Time for AES_Encrypt_32: %.9f s\n", time_enc_32);
    printf("Time for AES_Decrypt: %.9f s\n", time_dec);
    printf("Time for AES_Decrypt_Opt: %.9f s\n", time_dec_opt);
    printf("Time for AES_Decrypt_32: %.9f s\n", time_dec_32);


    return 0;
}
