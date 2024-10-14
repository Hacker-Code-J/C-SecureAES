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

typedef struct {
    u8 M[MATRIX_MAX_SIZE][2 * MATRIX_MAX_SIZE];
    int row;
    int col;
} Matrix;

void Matrix_print(Matrix A) {
    for (int i = 0; i < A.row; i++) {
        printf("[ ");
        for (int j = 0; j < A.col; j++)
                printf("%02x ", A.M[i][j]);
        printf("]\n");
    } puts("");
}

void Matrix_Init(Matrix& mat, int row, int col) {
    mat.row = row;
    mat.col = col;
}

Matrix Matrix_Add(Matrix A, Matrix B) {
    Matrix res;
    Matrix_Init(res, 1, 1);
    
    if ((A.col != B.col) || (A.row != B.row)) {
        printf("(Matrix Add) Matrix Size Error!!!\n");
        return res;
    }
    res.row = A.row;
    res.col = A.col;
    for (int i = 0; i < res.row; i++)
        for (int j = 0; j < res.col; j++)
            res.M[i][j] = A.M[i][j] ^ B.M[i][j];

    return res;
}

// C = A * B, [m, k] * [k, n] = [m, n]
Matrix Matrix_Mul(Matrix A, Matrix B) {
    Matrix AB;
    Matrix_Init(AB, 1, 1);

    if (A.col != B.row) {
        printf("(Matrix Mul) Matrix Size Error!!!\n");
        return AB;
    }

    AB.row = A.row;
    AB.col = B.col;

    for (int i = 0; i < AB.row; i++) {
        for (int j = 0; j < AB.col; j++) {
            AB.M[i][j] = 0x00;
            for (int k = 0; k < A.col; k++)
                AB.M[i][j] ^= GF256_mul(A.M[i][k], B.M[k][j]);
        }
    }

    return AB;
}

Matrix Matrix_Scalar(Matrix A, u8 k) {
    Matrix kA;
    Matrix_Init(kA, 1, 1);

    kA.row = A.row;
    kA.col = A.col;

    for (int i = 0; i < kA.row; i++)
        for (int j = 0; j < kA.col; j++)
            kA.M[i][j] = GF256_mul(k, A.M[i][j]);
    
    return kA;
}

void Mat_Exchange_Row(Matrix& A, int row1, int row2) {
    for (int j = 0; j < A.col; j++) {
        u8 temp = A.M[row1][j];
        A.M[row1][j] = A.M[row2][j];
        A.M[row2][j] = temp;
    }
}

void Mat_Scalar_Row(Matrix& A, u8 k, int row) {
    for (int j = 0; j < A.col; j++) {
        A.M[row][j] = GF256_mul(A.M[row][j], k);
    }
}

void Mat_Row_Add(Matrix& A, u8 k, int row_src, int row_dst) {
    for (int j = 0; j < A.col; j++)
        A.M[row_dst][j] ^= GF256_mul(k, A.M[row_src][j]);
}

Matrix Matrix_Inverse(Matrix A) {
    Matrix InvA;
    Matrix_Init(InvA, A.row, A.col);

    if (A.row != A.col) {
        printf("(Matrix Inverse) Non-Square Matrix Error!!!\n");
        return InvA;
    }

    // AA = [A|I]
    Matrix AA;
    Matrix_Init(AA, A.row, 2 * A.col);

    for (int i = 0; i < A.row; i++) {
        for (int j = 0; j < A.col; j++) {
            AA.M[i][j] = A.M[i][j];
            AA.M[i][j + A.col] = (i == j) ? 0x01 : 0x00;
        }
    }

    puts("[A|I]:");
    Matrix_print(AA);

    // R-REF (Reduced Row Echelon Form) [A|I] ==> [I|A^{-1}]
    int pivot_row;
    for (int j = 0; j < A.col; j++) {
        pivot_row = -1; // Initialization

        for (int i = j; i < A.row; i++) {
            if (AA.M[i][j] != 0x00) {
                pivot_row = i;
                break;
            } 
        }

        if (pivot_row == -1) {
            printf("(Matrix Inverse) Not Invertible!!!\n");
            // return InvA;
        }

        if (pivot_row != j) {
            Mat_Exchange_Row(AA, j, pivot_row);
        }

        // Multiplicative Inverse on GF(2)
        Mat_Scalar_Row(AA, GF256_inv(AA.M[j][j]), j);

        for (int i = 0; i < A.row; i++) {
            // Additive Inverse AA.M[i][j]
            if (i != j) Mat_Row_Add(AA, AA.M[i][j], j, i);
        }

        printf("step %d:\n", j+1);
        Matrix_print(AA);
    }

    for (int i = 0; i < InvA.row; i++)
        for (int j = 0; j < InvA.col; j++)
            InvA.M[i][j] = AA.M[i][j + A.col];

    return InvA;
}

void GF2_InvMat_Test() {
    Matrix mat, invMat;
    Matrix_Init(mat, 4, 4);
    Matrix_Init(invMat, 4, 4);

    const int M[16] = {
        0x02, 0x03, 0x01, 0x01,
        0x01, 0x02, 0x03, 0x01,
        0x01, 0x01, 0x02, 0x03,
        0x03, 0x01, 0x01, 0x02
    };
        
    for (int i = 0; i < mat.row; i++)
        for(int j = 0; j < mat.col; j++)
            mat.M[i][j] = M[4 * i + j];

    puts("Matrix A:");
    Matrix_print(mat);

    invMat = Matrix_Inverse(mat);
    puts("Matrix A^{-1}:");
    Matrix_print(invMat);
}

int main(int argc, char* argv[]) {
    GF2_InvMat_Test();
    
    return 0;
}
