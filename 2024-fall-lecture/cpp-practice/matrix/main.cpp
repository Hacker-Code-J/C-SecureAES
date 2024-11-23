// g++ -g -Wall -Wextra main.cpp CMatrix.cpp -o a.out  
#include "CMatrix.h"

void test_matrix1() {
    CMatrix A;
    A.ResizeMat(2, 2);
    CMatrix B(2, 2);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            A.SetValue(i, j, 1. + i + j);
            B.SetValue(i, j, 1. + i - j);
        }
    }

    A.PrintMat("A");
    B.PrintMat("B");
    CMatrix C;
    C = A + B;
    C.PrintMat("C");
    CMatrix D = A * B;
    D.PrintMat("D");
}

int main() {
    test_matrix1();

    return 0;
}