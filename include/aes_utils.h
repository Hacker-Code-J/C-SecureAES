#include "aes_config.h"

#ifndef _AES_UTILS_H
#define _AES_UTILS_H

/* AES */

void stringToByteArray(u8* byteArray, const char* hexString);
void stringToWordArray(u32* wordArray, const char* hexString);

/* AESAVS */

void printProgressBar(int current, int total);

void parseHexLineToByte(u8* dst, const char* src, size_t byteLength);
void parseHexLineToWord(u32* dst, const char* src, size_t wordLength);

size_t wordLength(const char* hexString);
size_t byteLength(const char* hexString);

#endif /* _AES_UTILS_H */