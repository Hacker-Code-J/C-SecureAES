// g++ -g -Wall -Wextra AES-CBC.cpp AES32.cpp -o aes-cbc

#include "AES32.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
using namespace std;

void AES_ENC_CBC(const char* filePT, uint8_t key[16], const char* fileCT);
void AES_DEC_CBC(const char* fileCT, uint8_t key[16], const char* filePT);
void FILE_CBC_TEST();

int main() {
    FILE_CBC_TEST();
    return 0;
}

void AES_ENC_CBC(const char* filePT, uint8_t key[16], const char* fileCT) {
    ifstream fin;
    ofstream fout;

    fin.open(filePT, ios::binary);
    if (fin.fail()) {
        cerr << "Plaintext File Open Error!" << endl;
        return;
    }

    int file_len;
    fin.seekg(0, fin.end);
    file_len = fin.tellg();
    cout << "file size (plaintext) = " << file_len << " bytes" << endl;
    fin.seekg(0, fin.beg);

    fout.open(fileCT, ios::binary);
    if (fout.fail()) {
        cout << "Ciphertext File Open Error!" << endl;
        return;
    }

    u32 rk[11][4];
    
    AES32_Enc_KeySchedule(key, rk);
    
    u8 buffer[16];
    u8 ct[16];

    // First IV
    u8 currentIV[16] = {
        0xaa, 0xd1, 0x58, 0x3c, 0xd9, 0x13, 0x65, 0xe3,
        0xbb, 0x2f, 0x0c, 0x34, 0x30, 0xd0, 0x65, 0xbb
    };

    int num_blk = file_len / 16;
    cout << "file size (ciphertext) = " << num_blk * 16 << " bytes" << endl;

    for (int i = 0; i < num_blk; i++) {
        fin.read((char*)buffer, 16);

        // XOR with IV or previous ciphertext block
        for (int i = 0; i < 16; i++)
            buffer[i] ^= currentIV[i]; 

        // Encrypt the Block
        AES32_Encrypt(buffer, rk, ct);
        fout.write((char*)ct, 16);
        
        // Update IV (current CT block become next IV)
        memcpy(currentIV, ct, 16 * sizeof(u8));
    }
    
    fin.close();
    fout.close();
}

void AES_DEC_CBC(const char* fileCT, uint8_t key[16], const char* filePT) {
    ifstream fin;
    ofstream fout;

    fin.open(fileCT, ios::binary);
    if (fin.fail()) {
        cerr << "Ciphertext File Open Error!" << endl;
        return;
    }

    int file_len;
    fin.seekg(0, fin.end);
    file_len = fin.tellg();
    cout << "file size (ciphertext) = " << file_len << " bytes" << endl;
    fin.seekg(0, fin.beg);

    fout.open(filePT, ios::binary);
    if (fout.fail()) {
        cout << "Plaintext File Open Error!" << endl;
        return;
    }
    
    u32 rk[11][4];
    AES32_Dec_KeySchedule(key, rk);
    
    u8 buffer[16];
    u8 pt[16];

    // First IV
    u8 currentIV[16] = {
        0xaa, 0xd1, 0x58, 0x3c, 0xd9, 0x13, 0x65, 0xe3,
        0xbb, 0x2f, 0x0c, 0x34, 0x30, 0xd0, 0x65, 0xbb
    };

    int num_blk = file_len / 16;
    cout << "file size (plaintext) = " << num_blk * 16 << " bytes" << endl;
    for (int i = 0; i < num_blk; i++) {
        fin.read((char*)buffer, 16);

        // Decrypt the Block
        AES32_EqDecrypt(buffer, rk, pt);

        // XOR with IV or previous ciphertext block
        for (int i = 0; i < 16; i++)
            pt[i] ^= currentIV[i]; 
        fout.write((char*)pt, 16);

        // Update IV (current CT block become next IV)
        memcpy(currentIV, buffer, 16 * sizeof(u8));
    }

    fin.close();
    fout.close();
}

void FILE_CBC_TEST() {
    const char* pPT = "PT-CBC.bin";
    const char* pCT = "CT-CBC.bin";
    const char* pDT = "DT-CBC.bin";

    u8 key[16] = {
        0x07, 0x00, 0xd6, 0x03, 0xa1, 0xc5, 0x14, 0xe4,
        0x6b, 0x61, 0x91, 0xba, 0x43, 0x0a, 0x3a, 0x0c
    };
    
    cout << "AES CBC Encrypt ..." << endl;
    AES_ENC_CBC(pPT, key, pCT);

    // cout << "AES CBC Decrypt ..." << endl;
    AES_DEC_CBC(pCT, key, pDT);
}