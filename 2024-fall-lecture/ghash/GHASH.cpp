// g++ -g -Wall -Wextra GHASH.cpp HexByte.cpp -o ghash
#include "GHASH.h"

void GF128_xtime(u8 p[16]);
void GF128_mul(u8 p[16], u8 q[16]);
void GHASH_ver0(u8 msg[], int msg_blks, u8 H[16], u8 tag[16]);
void GHASH_test();

void Make_GHASH_const_R0R1(u8 R0[256], u8 R1[256]);
void Make_GHASH_H_table(u8 H[16], u8 HT[256][16]);

int main() {
    // GHASH_test();

    u8 R0[256], R1[256];
    Make_GHASH_const_R0R1(R0, R1);

    return 0;
}

// GF(2^128)
// u8 p[16] = p0 p1 ... p127
// p(x) = p0 + p1*x + p127*x^127
// m(x) = 1 + x + x^2 + x^7 + x^127 (primitive polynomial)
// x*p(x) = p0*x + ... p126*x^127 + p127*(1+x+x^2+x^7)
// = [0 p0 ... p6] [p7 ... p14] ... [p119 ... p126] xor p127*[1110 0001]
// p(x) <- x*p(x)
void GF128_xtime(u8 p[16]) {
    u8 msb;
    msb = (u8) (p[15] & 0x01); // p127
    for (int i = 15; i > 0; i--) {
        p[i] = (p[i] >> 1) | ((p[i-1] & 0x01) << 7);
    }
    p[0] >>= 1;
    if (msb != 0) {     // p127 = 1
        p[0] ^= 0xe1;
    }
}

// p(x) <- p(x)*q(x)
void GF128_mul(u8 p[16], u8 q[16]) {
    u8 z[16] = { 0, };  // buffer
    u8 qi;  // q0, q1, ..., q127
    for (int i = 0; i < 16; i++) {      // u8 unit
        for (int j = 0 ; j < 8; j++) {  // bit unit
            qi = q[i] & (1 << (7-j));   // 0 or 1
            if (qi) {
                for (int k = 0; k < 16; k++) z[k] ^= p[k];
            }
            GF128_xtime(p); // p(x), p(x)*x, ..., p(x)*x^127
        }
    }
    for (int i = 0; i < 16; i++) p[i] = z[i];
}

void GHASH_ver0(u8 msg[],
                int msg_blks,
                u8 H[16],
                u8 tag[16]) {
    u8 x[16];
    u8 out[16] = { 0, };

    for (int i = 0; i < msg_blks; i++) {
        for (int j = 0; j < 16; j++) x[j] = msg[i * 16 + j];
        xor_b_array(out, 16, x);    // out <- x ^ out
        GF128_mul(out, H);          // out <- out * H
    }
    for (int i = 0; i < 16; i++) tag[i] = out[i];
}

void GHASH_test() {
    u8 H[16];
    u8 msg[16] = {0,};
    u8 tag[16];

    for (int i = 0; i < 16; i++) H[i] = i;  // test H(x)
    for (int i = 0; i < 16; i++) msg[i] = 0;  // test H(x)
    msg[1] = 0x01;

    GHASH_ver0(msg, 1, H, tag);

    // GHASH Tag1 = 152ebc020406080a0c0e10121416181a
    print_b_array(tag, 16, "Tag");
}

// x^127 = 1 + x + x^2 + x^7
// (a0 ... a7) * (x^127)
void Make_GHASH_const_R0R1(u8 R0[256], u8 R1[256]) {
    u8 a[8];    // a0 a1 ... a7
    for (int i = 0; i < 256; i++) {
        R0[i] = 0;
        R1[i] = 0;
    }

    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 8; j++) {
            a[j] = (i >> (7-j)) & 0x01;
        }
        R0[i] = a[0] << 7;
		R0[i] ^= (a[0] ^ a[1]) << 6;
		R0[i] ^= (a[0] ^ a[1] ^ a[2]) << 5;
		R0[i] ^= (a[1] ^ a[2] ^ a[3]) << 4;
		R0[i] ^= (a[2] ^ a[3] ^ a[4]) << 3;
		R0[i] ^= (a[3] ^ a[4] ^ a[5]) << 2;
		R0[i] ^= (a[4] ^ a[5] ^ a[6]) << 1;
		R0[i] ^= a[5] ^ a[6] ^ a[7] ^ a[0];

		R1[i] = (a[7] ^ a[6] ^ a[1]) << 7;
		R1[i] ^= (a[7] ^ a[2]) << 6;
		R1[i] ^= a[3] << 5;
		R1[i] ^= a[4] << 4;
		R1[i] ^= a[5] << 3;
		R1[i] ^= a[6] << 2;
		R1[i] ^= a[7] << 1;
    }

    puts("u8 R0[256] = {");
    for (int i = 0; i < 256; i++) {
        if (i != 255)  printf("0x%02x, ", R0[i]);
        else printf("0x%02x", R0[i]);
        if ((i%16) == 15) puts("");
    }
    puts("};");

    puts("u8 R1[256] = {");
    for (int i = 0; i < 256; i++) {
        if (i != 255)  printf("0x%02x, ", R1[i]);
        else printf("0x%02x", R1[i]);
        if ((i%16) == 15) puts("");
    }
    puts("};");
}

// H(x)*q(x) = H(x)*(q0+q1*x+....+q7*x^7) --> HT[q(x)][]
// = q0*H(x) + q1*H(x)*x + ... + q7*H(x)*x^7 
void Make_GHASH_H_table(u8 H[16], u8 HT[256][16]) {
	u8 Z[16]; // buffer
	u8 H_mul[16]; // H(x), H(x)*x, H(x)*x^2, ... , H(x)*x^7
	u8 qj; // q_j: q0, q1, ... , q7

	for (int i = 0; i < 256; i++) { // 0x00 - 0xFF
		// H(x)*[0x00 - 0xFF] ==> HT[i][0]...HT[i][16]
		for (int j = 0; j < 16; j++) { // Initialize buffer
			Z[j] = 0x00;
			H_mul[j] = H[j];  // Initialize as H(x)
		}
		for (int j = 0; j < 8; j++) { // q_j: q0, q1, ... , q7
			// [i 다항식] =  [q0 q1 q2 ... q7] (8-bit)
			qj = (u8)((i >> (7 - j)) & 0x01);
			if (qj == 1) {
				xor_b_array(Z, 16, H_mul); // Z <- Z + qj*H(x)*x^j
			}
			GF128_xtime(H_mul); // Hmul <-- H_mul * x 
		}
		copy_b_array(Z, 16, HT[i]); // Z[] --> HT[i][]
	}
}