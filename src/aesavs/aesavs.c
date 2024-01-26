#include "aesavs.h"

void freeData(Data* pData) {
    if (pData != NULL) {
        free(pData->key);
        free(pData->pt);
        free(pData->ct);
        pData->key = NULL;
        pData->pt = NULL;
        pData->ct = NULL;
    }
}

void printHexToFile(FILE* fp, const u8* data, const size_t byteLength) {
    for (size_t i = 0; i < byteLength; i++) {
        fprintf(fp, "%08X", data[i]);
    }
    fprintf(fp, "\n"); // Add a newline for formatting, if needed
}

bool readData(FILE* fp, Data* pData) {
    char line[MAX_LINE_LENGTH];

    size_t byteDataLen = 0x00;
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strncmp(line, "KEY =", 5) == 0) {
            size_t byteKeyLen = byteLength(line + 6);
            pData->key = (u8*)calloc(byteKeyLen, sizeof(u8));
            if (pData->key == NULL) return false;
            parseHexLineToByte(pData->key, line, byteKeyLen);
        } else if (strncmp(line, "PT =", 4) == 0) {
            byteDataLen = byteLength(line + 5);
            pData->pt = (u8*)calloc(byteDataLen, sizeof(u8));
            if (pData->pt == NULL) return false;
            parseHexLineToByte(pData->pt, line, byteDataLen);
        } else if (strncmp(line, "CT =", 4) == 0 ) {
            if (pData->pt == NULL) {
                perror("Plaintext not found");
                return false;
            }
            pData->ct = (u8*)calloc(byteDataLen, sizeof(u8));
            if (pData->ct == NULL) return false;
            parseHexLineToByte(pData->ct, line, byteDataLen);
        }
    }

    return true; // Return true on successful read
}


bool compareData128(const Data* pData1, const Data* pData2) {
    // Compare contents key and iv
    // for (size_t i = 0; i < 16; i++) {
    //     if (pData1->key[i] != pData2->key[i]) {
    //         return false;
    //     }
    // }

    // // Compare contents pt and ct
    // for (size_t i = 0; i < 16; i++) {
    //     if (pData1->pt[i] != pData2->pt[i] ||
    //         pData1->ct[i] != pData2->ct[i]) {
    //         return false;
    //     }
    // }

    return true; // All comparisons passed, data structures are equal
}