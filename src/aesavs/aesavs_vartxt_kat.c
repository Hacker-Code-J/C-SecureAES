#include "aesavs.h"

void create_AES_VARTXT_KAT_ReqFile(const char* pTxtFileName, const char* pReqFileName) {
    FILE *pTxtFile, *pReqFile;
    char* pLine;
    size_t bufsize = MAX_LINE_LENGTH;
    int isFirstKey = 1; // Flag to check if it's the first KEY line

    pTxtFile = fopen(pTxtFileName, "r"); // AES128(VARTXT)KAT.txt
    if (pTxtFile == NULL) {
        perror("Error opening input file");
        return;
    }

    // Open the .req file for writing
    pReqFile = fopen(pReqFileName, "w"); // AES128(VARTXT)KAT.req
    if (pReqFile == NULL) {
        perror("Error opening .req file");
        fclose(pTxtFile);
        return;
    }
    
    // Allocate initial buffer
    pLine = (char*)calloc(bufsize, sizeof(char));
    if(pLine == NULL) {
        perror("Unable to allocate memory");
        fclose(pTxtFile);
        fclose(pReqFile);
        return;
    }

    // Read the source file line by line
    while (fgets(pLine, bufsize, pTxtFile)) {
        if (strncmp(pLine, "KEY =", 5) == 0) {
            // If not the first KEY, add a newline before writing the line
            if (!isFirstKey) fputc('\n', pReqFile);
            isFirstKey = 0;
            fputs(pLine, pReqFile);
        } else if (strncmp(pLine, "PT =", 4) == 0) {
            fputs(pLine, pReqFile);
        }
    }

    free(pLine);
    fclose(pTxtFile);
    fclose(pReqFile);

    printf("AES128(VARTXT)KAT.req file has been successfully created in 'AES_KAT/VarTxt_KAT' folder.\n");
}

void create_AES_VARTXT_KAT_FaxFile(const char* pTxtFileName, const char* pFaxFileName) {
    FILE *pTxtFile, *pFaxFile;
    char* pLine;
    size_t bufsize = MAX_LINE_LENGTH;
    int isFirstKey = 1; // Flag to check if it's the first KEY line

    pTxtFile = fopen(pTxtFileName, "r"); // AES128(VARTXT)KAT.txt
    if (pTxtFile == NULL) {
        perror("Error opening input file");
        return;
    }

    // Open the .fax file for writing
    pFaxFile = fopen(pFaxFileName, "w"); // AES128(VARTXT)KAT.fax
    if (pFaxFile == NULL) {
        perror("Error opening .fax file");
        fclose(pTxtFile);
        return;
    }
    
    // Allocate initial buffer
    pLine = (char*)calloc(bufsize, sizeof(char));
    if(pLine == NULL) {
        perror("Unable to allocate memory");
        fclose(pTxtFile);
        fclose(pFaxFile);
        return;
    }

    // Read the source file line by line
    while (fgets(pLine, bufsize, pTxtFile)) {
        if (strncmp(pLine, "KEY =", 5) == 0) {
            // If not the first KEY, add a newline before writing the line
            if (!isFirstKey) fputc('\n', pFaxFile);
            isFirstKey = 0;
            fputs(pLine, pFaxFile);
        } else if (strncmp(pLine, "PT =", 4) == 0 ||
                   strncmp(pLine, "CT =", 4) == 0) {
            fputs(pLine, pFaxFile);
        }
    }

    free(pLine);
    fclose(pTxtFile);
    fclose(pFaxFile);

    printf("AES128(VARTXT)KAT.fax file has been successfully created in 'AES_KAT/VarTxt_KAT' folder.\n");
}

void create_AES_VARTXT_RspFile(const char* pReqFileName, const char* pRspFileName) {
    FILE *pReqFile, *pRspFile;
    char* pLine;
    size_t bufsize = MAX_LINE_LENGTH;
    int isFirstKey = 1;
    
    Data* pData = (Data*)malloc(sizeof(Data));
    if (pData == NULL) {
        perror("Unable to allocate memory");
        return;
    }
    memset(pData, 0, sizeof(Data));

    pReqFile = fopen(pReqFileName, "r"); // AES128(VARTXT)KAT.req
    if (pReqFile == NULL) {
        perror("Error opening .req file");
        return;
    }

    // Open the .rsp file for writing
    pRspFile = fopen(pRspFileName, "w"); // AES128(VARTXT)KAT.rsp
    if (pRspFile == NULL) {
        perror("Error opening .rsp file");
        fclose(pReqFile);
        return;
    }
    
    // Allocate initial buffer
    pLine = (char*)calloc(bufsize, sizeof(char));
    if(pLine == NULL) {
        perror("Unable to allocate memory");
        fclose(pReqFile);
        fclose(pRspFile);
        return;
    }

    // Read the source file line by line
    size_t byteDataLen = 0x00;
    while (fgets(pLine, bufsize, pReqFile)) {
        if (strncmp(pLine, "KEY =", 5) == 0) {
            if (!isFirstKey) fputc('\n', pRspFile);
            isFirstKey = 0;
            size_t byteKeyLen = byteLength(pLine + 6);
            pData->key = (u8*)malloc(byteKeyLen * sizeof(u8));
            if (pData->key == NULL) {
                perror("Unable to allocate memory for KEY");
                break;
            }
            parseHexLineToByte(pData->key, pLine + 6, byteKeyLen);
            fputs(pLine, pRspFile);
        } else if (strncmp(pLine, "PT =", 4) == 0) {
            byteDataLen = byteLength(pLine + 5);
            pData->pt = (u8*)malloc(byteDataLen * sizeof(u8));
            if (pData->pt == NULL) {
                perror("Unable to allocate memory for PT");
                freeData(pData);
                break;
            }
            parseHexLineToByte(pData->pt, pLine + 5, byteDataLen);
            fputs(pLine, pRspFile);
        } else {
            pData->ct = (u8*)malloc(byteDataLen * sizeof(u8));
            if (pData->ct == NULL) {
                perror("Unable to allocate memory for CT");
                freeData(pData);
                break;
            }

            fprintf(pRspFile, "CT = ");
            AES_Encrypt(pData->ct, pData->pt, pData->key, AES128);
            for (size_t i = 0; i < byteDataLen; i++) {
                fprintf(pRspFile, "%02X", pData->ct[i]);
            }
            fprintf(pRspFile, "\n");
            freeData(pData);
            memset(pData, 0, sizeof(Data));
        }
    }
    pData->ct = (u8*)malloc(byteDataLen * sizeof(u8));
    if (pData->ct == NULL) {
        perror("Unable to allocate memory for CT");
        freeData(pData);
        return;
    }

    fprintf(pRspFile, "CT = ");
    AES_Encrypt(pData->ct, pData->pt, pData->key, AES128);
    for (size_t i = 0; i < byteDataLen; i++) {
        fprintf(pRspFile, "%02X", pData->ct[i]);
    }
    fprintf(pRspFile, "\n");
    freeData(pData);
    
    free(pLine);
    fclose(pReqFile);
    fclose(pRspFile);

    printf("AES128(VARTXT)KAT.rsp file has been successfully created in 'AES_KAT/VarTxt_KAT' folder.\n");
}

void MOVS_AES_VARTXT_TEST(void) {
    const char* folderPath = "../AES_KAT/VarTxt_KAT/";
    char txtFileName[50];
    char reqFileName[50];
    char faxFileName[50];
    char rspFileName[50];
    
    // Construct full paths for input and output files
    snprintf(txtFileName, sizeof(txtFileName), "%s%s", folderPath, "AES128(VARTXT)KAT.txt");
    snprintf(reqFileName, sizeof(reqFileName), "%s%s", folderPath, "AES128(VARTXT)KAT.req");
    snprintf(faxFileName, sizeof(faxFileName), "%s%s", folderPath, "AES128(VARTXT)KAT.fax");
    snprintf(rspFileName, sizeof(rspFileName), "%s%s", folderPath, "AES128(VARTXT)KAT.rsp");
    
    create_AES_VARTXT_KAT_ReqFile(txtFileName, reqFileName);
    create_AES_VARTXT_KAT_FaxFile(txtFileName, faxFileName);
    create_AES_VARTXT_RspFile(reqFileName, rspFileName);

    // printf("\nAES128-VARTXT-KAT:\n");

    // FILE* file1 = fopen(faxFileName, "r");
    // FILE* file2 = fopen(rspFileName, "r");

    // if (!file1 || !file2) {
    //     perror("Error opening files");
    //     return;
    // }

    // Data* pData1 = (Data*)malloc(sizeof(Data));
    // Data* pData2 = (Data*)malloc(sizeof(Data));
    // if (pData1 == NULL || pData2 == NULL) {
    //     perror("Unable to allocate memory");
    //     return;
    // }
    // int result = 1; // Default to pass
    // int idx = 1;
    // int totalTests = 128; // Assuming a total of 275 tests
    // int passedTests = 0;
    // while (idx <= totalTests) {
    //     // sReset the structures for the next iteration
    //     memset(pData1, 0, sizeof(Data));
    //     memset(pData2, 0, sizeof(Data));
    //     if (!readData(file1, pData1) || !readData(file2, pData2)) {
    //         result = 0; // Indicate failure if read fails
    //         break;
    //     }

    //     if (!compareData128(pData1, pData2)) {
    //         result = 0; // Fail
    //         printf("\nFAIL\n");
    //         break;
    //     }

    //     // Free the dynamically allocated memory
    //     freeData(pData1);
    //     freeData(pData2);

    //     passedTests++;
    //     printProgressBar(idx++, totalTests);
    // }

    // printf("\n\nTesting Summary:\n");
    // printf("Passed: %d/%d\n", passedTests, totalTests);
    // if (result) {
    //     printf("Perfect PASS !!!\n\n");
    // } else {
    //     printf("Some tests FAILED.\n\n");
    // }

    // fclose(file1);
    // fclose(file2);
}