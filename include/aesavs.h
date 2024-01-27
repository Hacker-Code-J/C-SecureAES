#include "aes.h"
#include "aes_utils.h"

#ifndef _AESAVS_H
#define _AESAVS_H

#define MAX_LINE_LENGTH 1034
#define MAX_TXT_SIZE 1028

typedef struct {
    u8* key;   // Pointer for arbitrary length key
    u8* pt;    // Pointer for arbitrary length plaintext
    u8* ct;    // Pointer for arbitrary length ciphertext
} Data;         // 24-byte (8 * 3)

void freeData(Data* pData);

void printHexToFile(FILE* fp, const u8* data, const size_t byteLength);

bool readData(FILE* fp, Data* pData);
bool compareData128(const Data* pData1, const Data* pData2);

/* AESAVS-VARTXT-KAT */
void create_AES_VARTXT_KAT_ReqFile(const char* pTxtFileName, const char* pReqFileName);
void create_AES_VARTXT_KAT_FaxFile(const char* pTxtFileName, const char* pFaxFileName);
void create_AES_VARTXT_RspFile(const char* pReqFileName, const char* pRspFileName);
void MOVS_AES_VARTXT_TEST(void);

#endif /* _AESAVS_H */