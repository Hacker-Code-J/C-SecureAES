/**
 * @file utils.c
 * @brief Implementation of utility functions.
 *
 * This source file contains the implementation of utility functions declared
 * in utils.h. It provides the functionality necessary for the various operations
 * needed in the AES encryption program, such as data conversion and formatting.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "utils.h"

void stringToByteArray(const char* hexString, u8* byteArray) {
    size_t length = strlen(hexString);
    for (size_t i = 0; i < length; i += 2) {
        sscanf(&hexString[i], "%2hhx", &byteArray[i / 2]);
    }
}

void stringToWordArray(const char* hexString, u32* wordArray) {
    size_t length = strlen(hexString);
    for (size_t i = 0; i < length; i += 8) {
        sscanf(&hexString[i], "%8x", &wordArray[i / 8]);
    }
}

void RANDOM_KEY_GENERATION(u8* key) {
    srand((u32)time(NULL));
    
    // Initialize pointer to the start of the key array
    u8* p = key;

    // Set the counter to 16 bytes
    int cnt = 0;

    // Loop until all 16 bytes are filled
    while (cnt < 16) {
        *p = rand() & 0xff; // Assign a random byte (0 to 255) to the current location pointed by p
        p++;                // Move to the next byte
        cnt++;              // Decrement the byte count
    }
}

// Base64 characters
static const char base64_chars[] = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// Function to find the index of a base64 character
static int base64_char_value(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;
    return -1;
}

// Function to encode a string into base64
char *base64_encode(const u8* input, int length) {
    int i = 0, j = 0;
    int enc_len = 4 * ((length + 2) / 3);
    char *encoded = malloc(enc_len + 1);

    for (i = 0; i < length; i += 3) {
        int val = (0xFF & (int)input[i]) << 16;
        val += (i + 1 < length) ? (0xFF & (int)input[i + 1]) << 8 : 0;
        val += (i + 2 < length) ? 0xFF & (int)input[i + 2] : 0;

        encoded[j++] = base64_chars[(val >> 18) & 0x3F];
        encoded[j++] = base64_chars[(val >> 12) & 0x3F];
        encoded[j++] = (i + 1 < length) ? base64_chars[(val >> 6) & 0x3F] : '=';
        encoded[j++] = (i + 2 < length) ? base64_chars[val & 0x3F] : '=';
    }

    encoded[enc_len] = '\0';
    return encoded;
}

// Function to convert a string to hexadecimal
char *string_to_hex(const char *input) {
    if (input == NULL) return NULL;

    size_t length = strlen(input);
    char *hex_output = malloc(2 * length + 1);
    char *hex_ptr = hex_output;

    for (size_t i = 0; i < length; ++i) {
        hex_ptr += sprintf(hex_ptr, "%02x", (unsigned char)input[i]);
    }

    hex_output[2 * length] = '\0';
    return hex_output;
}

// Function to decode a base64 encoded string
u8 *base64_decode(const char *input, int *out_length) {
    int length = strlen(input);
    int pad = (input[length - 1] == '=') + (input[length - 2] == '=');

    *out_length = 3 * length / 4 - pad;
    unsigned char *decoded = malloc(*out_length);
    if (!decoded) return NULL;

    for (int i = 0, j = 0; i < length; i += 4, j += 3) {
        int n = base64_char_value(input[i]) << 18;
        n += base64_char_value(input[i + 1]) << 12;
        n += base64_char_value(input[i + 2]) << 6;
        n += base64_char_value(input[i + 3]);

        decoded[j] = (n >> 16) & 0xFF;
        if (i + 2 < length && input[i + 2] != '=') decoded[j + 1] = (n >> 8) & 0xFF;
        if (i + 3 < length && input[i + 3] != '=') decoded[j + 2] = n & 0xFF;
    }

    return decoded;
}

// Function to convert a hexadecimal string to a byte array
u8 *hex_to_bytes(const char *hex, int *out_length) {
    if (hex == NULL) return NULL;

    size_t len = strlen(hex);
    if (len % 2 != 0) return NULL; // Hex string must be even length

    size_t final_len = len / 2;
    *out_length = final_len;
    unsigned char *bytes = malloc(final_len);
    if (!bytes) return NULL;

    for (size_t i = 0; i < final_len; ++i) {
        sscanf(hex + 2*i, "%2hhx", &bytes[i]);
    }

    return bytes;
}

// u8 MUL_GF256(u8 a, u8 b) {
// 	u8 res = 0;
// 	// Mask for detecting the MSB (0x80 = 0b10000000)
// 	u8 MSB_mask = 0x80;
// 	u8 MSB;
	
// 	/*
// 	 * The reduction polynomial
// 	 * (x^8 + x^4 + x^3 + x + 1) = 0b100011011
// 	 * for AES, represented in hexadecimal
// 	*/
// 	u8 modulo = 0x1B;
	
// 	for (int i = 0; i < 8; i++) {
// 		// LSB(b)=1
// 		if (b & 1) { // Addition in Binary Field
// 			res ^= a;
// 		}
		
// 		// Store the MSB of a
// 		MSB = a & MSB_mask;
		
// 		// Multiplying it by x effectively
// 		a <<= 1;
		
// 		// Reduce the result modulo the reduction polynomial
// 		if (MSB) {
// 			a ^= modulo;
// 		}
		
// 		// Moving to the next bit
// 		b >>= 1;
// 	}
	
// 	return res;
// }