#include "aes_config.h"

#ifndef _AES_UTILS_H
#define _AES_UTILS_H

/* measure */

// Read Time-Stamp Counter
static inline u64 rdtsc() {
    u32 lo, hi;
    
    __asm__ __volatile__ (
      "rdtsc" : "=a" (lo), "=d" (hi)
    );
    
    return ((u64)hi << 32) | lo;
}

u64 measure_encryption_cycle(void (*func)(u8*, const u8*, const u8*, const u8), u8* dst, const u8* src, const u8* uKey, const u8 AES_VERSION);
double measure_encryption_time(void (*func)(u8*, const u8*, const u8*, const u8), u8* dst, const u8* src, const u8* uKey, const u8 AES_VERSION);

/* AES */

void stringToByteArray(u8* byteArray, const char* hexString);
void stringToWordArray(u32* wordArray, const char* hexString);

void byteToWord(u32* dst, const u8* src, size_t byteLen);
void wordToByte(u8* dst, const u32* src, size_t wordLen);

/* AESAVS */

void printProgressBar(int current, int total);

void parseHexLineToByte(u8* dst, const char* src, size_t byteLength);
void parseHexLineToWord(u32* dst, const char* src, size_t wordLength);

size_t wordLength(const char* hexString);
size_t byteLength(const char* hexString);

#endif /* _AES_UTILS_H */