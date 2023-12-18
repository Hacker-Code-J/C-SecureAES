/**
 * @file utils.h
 * @brief Utility functions for the AES encryption program.
 *
 * This header file declares various utility functions used across
 * the AES encryption program. These include functions for data manipulation,
 * formatting, and other helper functions that streamline the main AES processes.
 */

#ifndef _UTILS_H
#define _UTILS_H

#include "config.h"


void stringToByteArray(const char* hexString, u8* byteArray);

void RANDOM_KEY_GENERATION(u8 *key);

// /**
//  * Performs multiplication in the Galois Field (2^8), which is used in the MixColumns step of AES.
//  * This function multiplies two bytes in GF(2^8) and returns the result.
//  * 
//  * Galois Field multiplication is different from regular integer multiplication. 
//  * It's a bitwise operation under the constraints of a finite field defined by a reduction polynomial.
//  * In AES, the reduction polynomial is x^8 + x^4 + x^3 + x + 1, represented as 0x1B.
//  * 
//  * @param a The first byte to multiply.
//  * @param b The second byte to multiply.
//  * @return The result of the multiplication in GF(2^8).
//  */
// u8 MUL_GF256(u8 a, u8 b);

#endif // _UTILS_H