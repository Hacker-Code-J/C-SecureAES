// g++ -g -Wall -Wextra main.cpp AES-ECB.cpp -o aes-ecb

#include "AES32.h"
#include <iostream>
#include <fstream>
using namespace std;

void padding(uint8_t in[], int in_len, uint8_t out[16]);
int pt_len(uint8_t padded[16]);
void AES_ENC_ECB(const char* filePT, uint8_t key[16], const char* fileCT);
void AES_DEC_ECB(const char* fileCT, uint8_t key[16], const char* filePT);
void FILE_ECB_TEST();

int main() {
    FILE_ECB_TEST();
    return 0;
}

void padding(uint8_t in[], int in_len, uint8_t out[16]) {
    uint8_t pad_header = 0x80;
    for (int i = 0; i < in_len; i++)
        out[i] = in[i];

    out[in_len] = pad_header;
    for (int i = in_len + 1; i < 16; i++)
        out[i] = 0x00;
}

int pt_len(uint8_t padded[16]) {
    int position80;
    position80 = 15;
    for (int i = 15; i > 0; i--) {
        if (padded[i] != 0x00) break;
        position80--;
    }

    if (padded[position80] != 0x80) {
        cerr << "Padding error: 0x80 not found." << endl;
        return -1;
    } else {
        return position80;
    }
}

void AES_ENC_ECB(const char* filePT, uint8_t key[16], const char* fileCT) {
    ifstream fin;
    ofstream fout;
    char ch;

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

    int num_blk, remain;
    num_blk = file_len / 16 + 1;
    remain = file_len - 16 * (num_blk - 1);

    cout << "file size (ciphertext) = " << num_blk * 16 << " bytes" << endl;

    u32 rk[11][4];
    
    AES32_Enc_KeySchedule(key, rk);
    
    u8 buffer[16];
    u8 ct[16];

    for (int i = 0; i < num_blk - 1; i++) {
        fin.read((char*)buffer, 16);
        AES32_Encrypt(buffer, rk, ct);
        fout.write((char*)ct, 16);
    }

    u8 last_in[16];
    u8 last_blk[16];
    for (int i = 0; i < remain; i++) {
        fin.read((char*)&ch, 1);
        last_in[i] = ch;
    }

    padding(last_in, remain, last_blk);
    AES32_Encrypt(last_blk, rk, ct);
    fout.write((char*)ct, 16);
    
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

    fout.open(filePT, ios::binary);
    if (fout.fail()) {
        cout << "Plaintext File Open Error!" << endl;
        return;
    }

    int file_len;
    fin.seekg(0, fin.end);
    file_len = fin.tellg();
    cout << "file size (ciphertext) = " << file_len << " bytes" << endl;
    fin.seekg(0, fin.beg);

    
    int num_blk;
    num_blk = file_len / 16;

    // cout << "file size (Ciphertext) = " << num_blk * 16 << " bytes" << endl;

    u32 rk[11][4];
    AES32_Dec_KeySchedule(key, rk);
    
    u8 buffer[16];
    u8 pt[16];

    for (int i = 0; i < num_blk - 1; i++) {
        fin.read((char*)buffer, 16);
        AES32_EqDecrypt(buffer, rk, pt);
        fout.write((char*)pt, 16);
    }

    fin.read((char*)buffer, 16);
    AES32_EqDecrypt(buffer, rk, pt);

    u8 last_pt_len = pt_len(pt);
    if (last_pt_len < 0) {
        cerr << "Padding error!" << endl;
        return;
    }

    fout.write((char*)pt, last_pt_len);

    cout << "file size (plaintext) = " << (num_blk - 1) * 16 + last_pt_len << " bytes" << endl;
    
    fin.close();
    fout.close();
}

void FILE_ECB_TEST() {
    const char* pPT = "PT.bin";
    const char* pCT = "CT.bin";
    const char* pDT = "DT.bin";

    u8 key[16];
    // u8 IV[16];
    for (int i = 0; i < 16; i++)
        key[i] = i;

    cout << "AES ECB Encrypt ..." << endl;
    AES_ENC_ECB(pPT, key, pCT);

    cout << "AES ECB Decrypt ..." << endl;
    AES_DEC_ECB(pCT, key, pDT);
}