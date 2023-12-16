/**
 * @file aes_key_expansion.h
 * @brief This file contains the declarations for AES key expansion functions.
 */

#ifndef _AES_KEY_EXPANSION_H
#define _AES_KEY_EXPANSION_H

#include "utils.h"

/**
 * @brief Round constants used in the AES key expansion algorithm.
 *
 * These constants are used in the key expansion process of the AES algorithm. 
 * They are derived from the powers of x (x is 0x02) in the field GF(2^8).
 */
static const u32 rCon[10] = {
    0x01000000, 0x02000000, 0x04000000, 0x08000000,
    0x10000000, 0x20000000, 0x40000000, 0x80000000,
    0x1b000000, 0x36000000
};

/**
 * @brief Rotates the word by one byte.
 *
 * This function is a part of the AES key expansion process. It rotates
 * the 4 bytes in a word to the left, effectively moving byte 1 to the end.
 *
 * @param word The 32-bit word to be rotated.
 * @return The rotated 32-bit word.
 */
u32 RotWord(u32 word);

/**
 * @brief Applies S-box substitution on all bytes of the word.
 *
 * This function is used in the AES key expansion process. Each byte of the word
 * is substituted with its corresponding value in the S-box.
 *
 * @param word The 32-bit word to be substituted.
 * @return The 32-bit word after S-box substitution.
 */
u32 SubWord(u32 word);

/**
 * @brief Expands the AES key into the round keys.
 *
 * This function takes the initial key for AES and expands it into
 * several round keys as required by the AES algorithm. It uses the
 * helper functions RotWord and SubWord in the process.
 *
 * @param ukey The initial key for AES.
 * @param rKey The expanded round keys.
 */
void KeyExpansion(const u8* ukey, u32* rKey);

#endif // _AES_KEY_EXPANSION_H
