#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t u8;     // byte type
typedef uint32_t u32;

#define MATRIX_MAX_SIZE 4

// a * x
u8 GF256_xtime(u8 a) {
    return ((a & 0x80) == 0) ? (a << 1) : (a << 1) ^ 0x1b;
}

// Multiplication on GF(2^8)
u8 GF256_mul(u8 f, u8 g) {
    u8 h = 0x00;
    u8 coef;

    for (int i = 7; i >= 0; i--) {
        coef = (f >> i) & 0x01; // a7, a6, ..., a1, a0
        h = GF256_xtime(h);
        if (coef == 1) h ^= g;
    }
    return h;
}

// Multiplicative Inverse on GF(2^8)
u8 GF256_inv(u8 x) {
    if (!x) return 0x00;
    
    u8 x_inv = 0x01;
    u8 temp = x;
    for (int i = 0; i < 7; i++) {
        temp = GF256_mul(temp, temp); // x^2, x^4, ..., x^128 (128 = 2^7)
        x_inv = GF256_mul(x_inv, temp); // 1, x^2, x^2 * x^4, ..., x^2 * ...* x^{128}
    }

    return x_inv;
}

// x in GF(2^8) - {0} => ord(x) is finite
// H_x = {x, x^2, .., x^{ord(x)}=1} is a cyclic subgroup of G = {1, 2, ..., 255}
// |H| divides |G| by Lagrange's Thm
int GF256_ord(u8 x) {
    u8 temp;
    int ord;

    if (!x) return -1;

    temp = x;
    ord = 0x01;
    while (temp != 0x01) {
        temp = GF256_mul(temp, x);
        ord++;
    }

    return ord;
}

void Problem5_a() {
    int ord;
    int cnt_full_ord = 0x00;

    for (int i = 1; i < 256; i++) {
        ord = GF256_ord(i);
        printf("ord(%03d) = %03d | ", i, ord);
        if (!(i & 7)) puts("");
        if (ord == 255) cnt_full_ord++;
    } puts("");

    printf("The number of x s.t. ord(x)=255 is %d\n", cnt_full_ord);
}

void Problem5_b() {
    u8 a = 0x03; // 003
    u8 b = 0xff; // 255

    // printf("α");

    u8 temp = a;
    int i = 0;

    printf(" 0x01 |");
    while (a != 0x01) {
        printf(" 0x%02x |", a);
        if (i % 8 == 0) puts("");
        a = GF256_mul(a, temp); i++;
    } printf(" 0x%02x |\n", a);

    temp = b;
    i = 0;

    printf("\n\n 0x01 |");
    while (b != 0x01) {
        printf(" 0x%02x |", b);
        if (i % 8 == 0) puts("");
        b = GF256_mul(b, temp); i++;
    } printf(" 0x%02x |\n", b);

}

void Problem5_c() {

}

#define GF_SIZE 256
u8 ExpTable[GF_SIZE];
u8 LogTable[GF_SIZE];

void GF256_Table_Init(u8 generator) {
    u8 x = 0x01;  // Start with a^0 = 1
    for (int i = 0; i < GF_SIZE - 1; i++) {
        ExpTable[i] = x;  // ExpTable(k) = a^k
        LogTable[x] = i;  // LogTable(a^k) = k

        x = GF256_mul(x, generator);
    }

    ExpTable[GF_SIZE - 1] = ExpTable[0];  // ExpTable(255) = a^0 = 1
}

void PrintTable(const char *tableName, u8 table[], int size) {
    int numCols = 16;  // Number of columns to print per row

    printf("\n%s:\n", tableName);
    printf("Index | ");
    for (int col = 0; col < numCols; col++) {
        printf("%02X  ", col);  // Column headers in hex
    }
    printf("\n");
    printf("------+");
    for (int col = 0; col < numCols; col++) {
        printf("----");
    }
    printf("\n");

    for (int row = 0; row < size / numCols; row++) {
        printf("  %02X  | ", row * numCols);  // Row label in hex

        for (int col = 0; col < numCols; col++) {
            printf("%02X  ", table[row * numCols + col]);  // Print the value in hex
        }
        printf("\n");
    }
}

void Problem5_d() {
    u8 a = 0x03; // 003
    GF256_Table_Init(a);
    PrintTable("ExpTable", ExpTable, GF_SIZE);
    PrintTable("LogTable", LogTable, GF_SIZE);
}

u8 GF256_mul_exp_log(u8 x, u8 y) {
    if (x == 0 || y == 0) return 0;

    // Logarithmic addition: log(x) + log(y) mod 255
    int log_x = LogTable[x];
    int log_y = LogTable[y];
    int log_sum = (log_x + log_y) % (GF_SIZE - 1);

    // Return the exponentiation of the sum of logarithms
    return ExpTable[log_sum];
}

void Problem5_e() {
    GF256_Table_Init(0x03);

    u8 a = 0xa1;
    u8 b = 0x63;

    printf("0xa1 * 0x63 = %02x (with GF_MUL)\n", GF256_mul(a, b));
    printf("0xa1 * 0x63 = %02x (with Table )\n", GF256_mul_exp_log(a, b));
}

int main(int argc, char* argv[]) {
    // Problem5_a();
    // Problem5_b();
    // Problem5_c();
    Problem5_d();
    Problem5_e();

    return 0;
}
