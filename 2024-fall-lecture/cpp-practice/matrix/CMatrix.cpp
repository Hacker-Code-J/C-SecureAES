// g++ -g -Wall -Wextra CMatrix.cpp -o CMatrix

#include "CMatrix.h"

CMatrix::CMatrix(CMatrix& cm) {
    row = cm.row;
    col = cm.col;
    // M = cm.M;   // New memory is not allocated.
    M = new double[row * col];
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++)
            M[i * col + j] = cm.M[i * col + j];
    }
}

// A.M[i][j] = value ===> A.SetValue(i, j, value)
void CMatrix::SetValue(int i, int j, double value) {
    M[i * col + j] = value;
}
// x = A.M[i][j] ===> x = A.GetValue(i, j)
double CMatrix::GetValue(int i, int j) {
    return M[i * col + j];
}

void CMatrix::ResizeMat(int new_row, int new_col) {
    if (M != nullptr) delete[] M;

    row = new_row;
    col = new_col;
    M = new double[row * col];
}

void CMatrix::PrintMat(const char* pMsg) {
    if (pMsg != nullptr) printf("%s=\n", pMsg);

    for (int i = 0; i < row; i++) {
        printf("[");
        for (int j = 0; j < col; j++)
            printf("%7.2f", GetValue(i,j));
        puts("]");
    }
    puts("");
}


CMatrix CMatrix::operator+(CMatrix& cm) {
    CMatrix res;
    double value;

    if ((col != cm.col) || (row != cm.row)) {
        fprintf(stderr, "(Matrix_Add) Matrix size error!\n");
        return res;
    }

    res.ResizeMat(row, col);
    for (int i = 0; i < res.row; i++) {
        for (int j = 0; j < res.col; j++) {
            value = GetValue(i, j) + cm.GetValue(i, j);
            res.SetValue(i, j, value);
        }
    }

    return res;
}
CMatrix CMatrix::operator*(CMatrix& cm) {
    CMatrix res;
    double value;

    if (col != cm.row) {
        fprintf(stderr, "(Matrix_Mul) Matrix size error!\n");
        return res;
    }

    res.ResizeMat(row, cm.col);

    for (int i = 0; i < res.row; i++) {
        for (int j = 0; j < res.col; j++) {
            value = 0.0;
            for (int k = 0; k < col; k++) {
                value += GetValue(i, k) * cm.GetValue(k, j);
            }
            res.SetValue(i, j, value);
        }
    }

    return res;
}

CMatrix& CMatrix::operator=(const CMatrix& cm) {
    if (this != &cm) {
        row = cm.row;
        col = cm.col;
        
        if (M != nullptr) delete[] M;
        M = new double[row * col];
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                M[i * col + j] = cm.M[i * col + j];
            }
        }
    }

    return *this;
}