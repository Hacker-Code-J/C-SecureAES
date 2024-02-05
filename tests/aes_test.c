#include "aes_utils.h"
#include "aes.h"
#include "aes32.h"

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

    printf("Ciphertext(32-bit): \n");
    for (int i = 0; i < 16; i++) {
        printf("%02x", output2[i]);
    }
    printf("\n");
}

