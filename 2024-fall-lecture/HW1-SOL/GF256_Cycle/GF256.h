#pragma once

typedef unsigned char byte; // byte�� GF(2^8) ����ü�� ���Ҹ� �����ϴ� ������ ����

byte GF256_add(byte a, byte b);
void GF256_print_bin(byte x);
void GF256_print_poly(byte a);
byte GF256_xtime(byte a);
byte GF256_mul(byte f, byte g);
byte GF256_inv(byte x);

void Get_AES_Sbox(byte S[256]);
void Get_AES_Inv_Sbox(byte IS[256]);