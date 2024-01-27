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
void byteToWord(u32* output, const u8* input, size_t input_byteLen) {
    for (size_t i = 0; i < input_byteLen / 4; ++i) {
        output[i] = (u32)input[4 * i + 0] << 0x18 |
                    (u32)input[4 * i + 1] << 0x10 |
                    (u32)input[4 * i + 2] << 0x08 |
                    (u32)input[4 * i + 3] << 0x00;
    }
}

// Function to convert u32 array to u8 array
void wordToByte(u8* output, const u32* input, size_t input_wordLen) {
    for (size_t i = 0; i < input_wordLen; ++i) {
        output[4 * i + 0] = (u8)(input[i] >> 0x18);
        output[4 * i + 1] = (u8)(input[i] >> 0x10);
        output[4 * i + 2] = (u8)(input[i] >> 0x08);
        output[4 * i + 3] = (u8)(input[i] >> 0x00);
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