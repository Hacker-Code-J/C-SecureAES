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