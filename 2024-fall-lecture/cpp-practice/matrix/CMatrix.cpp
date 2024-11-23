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
double CMatrix::GetValue(int i, int j) const {
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

// CSQMatrix ================================================

CSQMatrix::CSQMatrix(CMatrix& cm) {
    if (cm.GetRow() != cm.GetCol()) {
        fprintf(stderr, "Cannot be copided to Square Matrix!\n");
        return;
    }

    row = cm.GetRow();
    col = cm.GetCol();
    M = new double[row * col];
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            M[i * col + j] = cm.GetValue(i, j);
        }
    }
}
CSQMatrix& CSQMatrix::operator=(const CSQMatrix& sq) {
    if (this != &sq) {
        if (M != nullptr) delete[] M;
        row = sq.row;
        col = sq.col;
        M = new double[row * col];
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                M[i * col + j] = sq.M[i * col + j];
            }
        }
    }
    return *this;
}
CSQMatrix& CSQMatrix::operator=(const CMatrix& cm) {
    if (this != &cm) {
        if (!cm.IsSquare()) {
            fprintf(stderr, "Cannot be copied to Square Matrix!\n");
            return *this;
        }
        if (M != nullptr) delete[] M;
        row = cm.GetRow();
        col = cm.GetCol();
        M = new double[row * col];
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                SetValue(i, j, cm.GetValue(i, j));
            }
        }
    }
    return *this;
}


double CSQMatrix::Trace() {
    double res;

    res = 0;
    for (int i = 0; i < row; i++) {
        res += GetValue(i, i);
    }

    return res;
}

double CSQMatrix::Det() {
    double det;
    int sign, c_pos;

    if (row == 1) return GetValue(0,0);

    CSQMatrix Adj(row - 1); // adjoint matrix
    det = 0.0;
    sign = -1;

    for (int c = 0; c < col; c++) {
        sign *= -1;
        for (int i = 1; i < row; i++) {
            c_pos = 0;
            for (int j = 0; j < col; j++) {
                if (j != c) {
                    Adj.SetValue(i - 1, c_pos, GetValue(i, j));
                    c_pos++;
                }
            }
        }
        det += sign * GetValue(0, c) * Adj.Det();
    }

    return det;
}