#include "aes_utils.h"

/* AES */

void stringToByteArray(u8* byteArray, const char* hexString) {
    size_t length = strlen(hexString);
    for (size_t i = 0; i < length; i += 2) {
        sscanf(&hexString[i], "%2hhx", &byteArray[i / 2]);
    }
}
void stringToWordArray(u32* wordArray, const char* hexString) {
    size_t length = strlen(hexString);
    for (size_t i = 0; i < length; i += 8) {
        sscanf(&hexString[i], "%8x", &wordArray[i / 8]);
    }
}

// Function to convert u8 array to u32 array
// b7|b6|b5|b4|b3|b2|b1|b0 <=> byte[8] = { b0, b1, b2, b3, b4, b5, b6, b7 }
// -> word[0] = b0 | b1 | b2 | b3 = w0
// -> word[1] = b4 | b5 | b6 | b7 = w1
void byteToWord(u32* dst, const u8* src, size_t byteLen) {
    for (size_t i = 0; i < byteLen / 4; i++) {
        dst[i] = (u32)src[4 * i    ] << 0x18 |
                 (u32)src[4 * i + 1] << 0x10 |
                 (u32)src[4 * i + 2] << 0x08 |
                 (u32)src[4 * i + 3];
    }
}

// Function to convert u32 array to u8 array
// w1|w0 <=> word[2] = { w0, w1 }
// -> byte[0] = (w0 >> 0x18) & 0xFF = b0
// -> byte[1] = (w0 >> 0x10) & 0xFF = b1
// -> byte[2] = (w0 >> 0x08) & 0xFF = b2
// -> byte[3] = (w0 >> 0x00) & 0xFF = b3
void wordToByte(u8* dst, const u32* src, size_t wordLen) {
    for (size_t i = 0; i < wordLen; i++) {
        dst[4 * i + 0] = (u8)((src[i] >> 0x18) & 0xFF);
        dst[4 * i + 1] = (u8)((src[i] >> 0x10) & 0xFF);
        dst[4 * i + 2] = (u8)((src[i] >> 0x08) & 0xFF);
        dst[4 * i + 3] = (u8)((src[i]        ) & 0xFF);
    }
}

/* AESAVS */

void printProgressBar(int current, int total) {
    int width = 50; // Width of the progress bar
    float progress = (float)current / total;
    int pos = width * progress;

    // ANSI Escape Codes for colors
    const char* GREEN = "\x1b[32m";
    const char* YELLOW = "\x1b[33m";
    const char* RED = "\x1b[31m";
    const char* RESET = "\x1b[0m";

    printf("\r[");
    for (int i = 0; i < width; ++i) {
        if (i < pos) printf("%s=", GREEN); // White for completed part
        else if (i == pos) printf("%s>", YELLOW); // Yellow for current position
        else printf("%s ", RED); // Red for remaining part
    }
    printf("%s] %d%% (%d/%d)", RESET, (int)(progress * 100.0), current, total);
}

void parseHexLineToByte(u8* dst, const char* src, size_t byteLength) {
    for (size_t i = 0; i < byteLength; i++) {
        u8 value;
        // Ensure not to read beyond the line's end
        if (sscanf(src + i * 2, "%2hhx", &value) != 1) {
            // Handle parsing error, such as setting a default value or logging an error
            dst[i] = 0; // Example: set to zero if parsing fails
        } else {
            dst[i] = value;
        }
    }
}

void parseHexLineToWord(u32* dst, const char* src, size_t wordLength) {
    for (size_t i = 0; i < wordLength; i++) {
        u32 value;
        // Ensure not to read beyond the line's end
        if (sscanf(src + i * 8, "%8x", &value) != 1) {
            // Handle parsing error, such as setting a default value or logging an error
            dst[i] = 0; // Example: set to zero if parsing fails
        } else {
            dst[i] = value;
        }
    }
}

size_t wordLength(const char* hexString) {
    size_t hexLength = strlen(hexString);

    size_t wordLength = hexLength / 8; // 0x00: 1-byte

    // If the hex string length is not a multiple of 8, add an extra element
    // if (hexLength % 8 != 0) {
    //     u32Length++;
    // }

    return wordLength;
}

size_t byteLength(const char* hexString) {
    size_t hexLength = strlen(hexString);

    size_t byteLength = hexLength / 2; // 0x00: 1-byte

    return byteLength;
}