#include "aes.h"
#include "aes_utils.h"

int main() {
    const char* inputString = "f34481ec3cc627bacd5dc3fb08f273e6";
    u8 input[16];
    stringToByteArray(input, inputString);
    // Print the plaintext
    printf("Plaintext(08-bit): ");
    for (int i = 0; i < 16; i++) {
        printf("%02x", input[i]);
    }
    printf("\n");

    const char* keyString = "ffffffffffffffffffffffffffffffff";
    u8 key[16];
    stringToByteArray(key, keyString);
    printf("Key: ");
    for (int i = 0; i < 16; i++) {
        printf("%02x", key[i]);
    }
    printf("\n");
    
    u8 output[16];

    // Call the AES_Encrypt function
    AES_Encrypt(output, input, key, AES128);
    // Print the ciphertext
    printf("Ciphertext(08-bit): \n");
    for (int i = 0; i < 16; i++) {
        printf("%02x", output[i]);
    }
    printf("\n");

    return 0;
}