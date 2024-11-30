// g++ -g -Wall -Wextra AES-ECB.cpp AES32.cpp -o aes-ecb

#include "AES32.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
using namespace std;

void AES_ENC_ECB(const char* filePT, uint8_t key[16], const char* fileCT);
void AES_DEC_ECB(const char* fileCT, uint8_t key[16], const char* filePT);
void FILE_ECB_TEST();

int main() {
    FILE_ECB_TEST();
    return 0;
}

void AES_ENC_ECB(const char* filePT, uint8_t key[16], const char* fileCT) {
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

    int num_blk = file_len / 16;
    cout << "file size (ciphertext) = " << num_blk * 16 << " bytes" << endl;

    for (int i = 0; i < num_blk; i++) {
        fin.read((char*)buffer, 16);
        AES32_Encrypt(buffer, rk, ct);
        fout.write((char*)ct, 16);
    }
    
    fin.close();
    fout.close();
}

void AES_DEC_ECB(const char* fileCT, uint8_t key[16], const char* filePT) {
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

    int num_blk = file_len / 16;
    cout << "file size (plaintext) = " << num_blk * 16 << " bytes" << endl;
    for (int i = 0; i < num_blk; i++) {
        fin.read((char*)buffer, 16);
        AES32_EqDecrypt(buffer, rk, pt);
        fout.write((char*)pt, 16);
    }

    fin.close();
    fout.close();
}

void FILE_ECB_TEST() {
    const char* pPT = "PT-ECB.bin";
    const char* pCT = "CT-ECB.bin";
    const char* pDT = "DT-ECB.bin";

    u8 key[16] = {
        0x77, 0x23, 0xd8, 0x7d, 0x77, 0x3a, 0x8b, 0xbf,
        0xe1, 0xae, 0x5b, 0x08, 0x12, 0x35, 0xb5, 0x66
    };
    

    cout << "AES ECB Encrypt ..." << endl;
    AES_ENC_ECB(pPT, key, pCT);

    cout << "AES ECB Decrypt ..." << endl;
    AES_DEC_ECB(pCT, key, pDT);
}