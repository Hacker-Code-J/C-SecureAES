// g++ -g -Wall -Wextra padding.cpp -o padding

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
using namespace std;

// typedef unsigned char byte;
// enum class byte : unsigned char;

void padding(uint8_t in[], int in_len, uint8_t out[16]);
void padding_test();

int main() {
    padding_test();
}

void padding(uint8_t in[], int in_len, uint8_t out[16]) {
    uint8_t pad_header = 0x80;
    for (int i = 0; i < in_len; i++)
        out[i] = in[i];

    out[in_len] = pad_header;
    for (int i = in_len + 1; i < 16; i++)
        out[i] = 0x00;
}

void padding_test() {
    ifstream fin;
    ofstream fout;
    char ch;

    fin.open("before-pad.bin", ios::binary);
    if (fin.fail()) {
        cout << "File Open Error!" << endl;
        return;
    }

    int file_len;
    fin.seekg(0, fin.end);
    file_len = fin.tellg();
    cout << "file size (before padding) = " << file_len << " bytes" << endl;
    fin.seekg(0, fin.beg);

    fout.open("after_pad.bin", ios::binary);
    if (fout.fail()) {
        cout << "File Open Error!" << endl;
        return;
    }

    int num_blk, remain;
    num_blk = file_len / 16 + 1;
    remain = file_len - 16 * (num_blk - 1);

    cout << "file size (after padding) " << num_blk * 16 << " bytes" << endl;

    uint8_t buffer[16];
    for (int i = 0; i < num_blk - 1; i++) {
        fin.read((char*)buffer, 16);
        fout.write((char*)buffer, 16);
    }

    uint8_t last_in[16], last_blk[16];
    for (int i = 0; i < remain; i++) {
        fin.read((char*)&ch, 1);
        last_in[i] = ch;
    }

    padding(last_in, remain, last_blk);
    fout.write((char*)last_blk, 16);

    fin.close();
    fout.close();
}