#pragma once

#include <iostream>
#include <stdint.h>

typedef uint8_t u8;
using namespace std;


bool isHex(char ch);
u8 Hex2Digit(char ch);
u8 Hex2Byte(const char h[2]);
void Hex2Array(const char hex_str[], int hex_len, u8 barr[]);
void print_b_array(u8 b_arr[], int len, const char* pStr = nullptr);
void copy_b_array(u8 src[], int len, u8 dest[]);
void xor_b_array(u8 data[], int len, u8 xor_arr[]);

void hex_test();
