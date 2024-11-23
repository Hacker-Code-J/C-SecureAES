#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

#define NEARLY_ZERO 1e-10

class CMatrix {
private:
    int row;
    int col;
    double* M;
public:
    // Constructor
    CMatrix() { 
        row = 1; col = 1;
        M = new double[1];
        M[0] = 0.0; 
    };
    CMatrix(int r, int c) {
        row = r; col = c;
        M = new double[row * col]; // heap
    }
    CMatrix(CMatrix& cm);

    // Destructor
    ~CMatrix() { delete[] M; }; // delete[] is operator

    void SetValue(int i, int j, double value);
    double GetValue(int i, int j);

    void ResizeMat(int new_row, int new_col);
    int GetRow() { return row; };
    int GetCol() { return col; };

    void PrintMat(const char* pMsg = nullptr);

    CMatrix operator+(CMatrix& cm);
    CMatrix operator*(CMatrix& cm);
    CMatrix& operator=(const CMatrix& cm);
};