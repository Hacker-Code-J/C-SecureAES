#include <stdio.h>
#include <stdlib.h>
#include <string.h> // For memcpy
#include <time.h>

#include <openssl/bio.h>
#include <openssl/evp.h>

#include "aes_key_expansion.h"
#include "aes.h"
#include "aes_modes.h"

// // Function to convert a string to base64
// char *base64_encode(const char *input, int length) {
//     BIO *b64, *bmem;
//     char *buff;
//     long size;

//     b64 = BIO_new(BIO_f_base64());
//     BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL); // To not use newlines to flush buffer
//     bmem = BIO_new(BIO_s_mem());
//     b64 = BIO_push(b64, bmem);
//     BIO_write(b64, input, length);
//     BIO_flush(b64);
//     size = BIO_get_mem_data(bmem, &buff);

//     char *output = (char *)malloc(size + 1);
//     memcpy(output, buff, size);
//     output[size] = 0;

//     BIO_free_all(b64);

//     return output;
// }

// // Function to convert base64 string to hexadecimal
// void base64_to_hex(char *base64, char *hex) {
//     for (int i = 0; base64[i] != '\0'; i++) {
//         sprintf(hex + (i * 2), "%02x", (unsigned char)base64[i]);
//     }
// }


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
    // u8 data[16]; // 128-bit block
    // const char *input = "Heldfgfdgdgdf";
    // size_t input_len = strlen(input);

    // if (input_len > sizeof(data)) {
    //     fprintf(stderr, "Input too long for the block size.\n");
    //     return 1;
    // }

    // memcpy(data, input, input_len); // Copy input to data array
    // pkcs7_pad(data, sizeof(data), input_len); // Apply PKCS#7 padding

    // // Print padded data in hex
    // for (size_t i = 0; i < sizeof(data); ++i) {
    //     printf("%02x ", data[i]);
    // }
    // printf("\n");
    
    // char *text = "Hello World";
    // char *base64_encoded = base64_encode((const u8*)text, strlen(text));
    // char *hex_encoded = string_to_hex(base64_encoded);

    // printf("Base64 Encoded: %s\n", base64_encoded);
    // printf("Hex Encoded: %s\n", hex_encoded);

    // // Convert Hex to bytes (Base64 encoded string)
    // int base64_length;
    // unsigned char *base64_bytes = hex_to_bytes(hex_encoded, &base64_length);

    // // Convert bytes to Base64 string
    // char *base64_str = malloc(base64_length + 1);
    // memcpy(base64_str, base64_bytes, base64_length);
    // base64_str[base64_length] = '\0';

    // // Decode from Base64
    // int decoded_length;
    // unsigned char *decoded = base64_decode(base64_str, &decoded_length);
    // printf("Decoded from Hex: %.*s\n\n", decoded_length, decoded);

    // free(base64_encoded);
    // free(hex_encoded);
    // free(base64_bytes);
    // free(base64_str);
    // free(decoded);
    
    // char *text = "Hello World";
    // char *base64_encoded = base64_encode(text, strlen(text));
    // char hex_encoded[2 * strlen(base64_encoded) + 1];
    
    // base64_to_hex(base64_encoded, hex_encoded);

    // printf("Base64 Encoded: %s\n", base64_encoded);
    // printf("Hex Encoded: %s\n", hex_encoded);

    // free(base64_encoded);

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
    double time_enc_32 = measure_time(AES_Encrypt_Precomp, input, key, output);
    double time_dec = measure_time(AES_Decrypt, input, key, output);
    double time_dec_32 = measure_time(AES_Decrypt_32BIT, input, key, output);

    printf("Time for AES_Encrypt: %.9f s\n", time_enc);
    printf("Time for AES_Encrypt_Precomp: %.9f s\n", time_enc_32);
    printf("Time for AES_Decrypt: %.9f s\n", time_dec);
    printf("Time for AES_Decrypt_32: %.9f s\n", time_dec_32);

    return 0;
}
