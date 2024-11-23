// g++ -g -Wall -Wextra main.cpp CMatrix.cpp -o CMatrix  
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

void test_sqmatrix1() {
    CSQMatrix A, B;
    A.ResizeMat(2, 2);
    B.ResizeMat(2, 2);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            A.SetValue(i, j, 1. + i + j);
            B.SetValue(i, j, 1. + i - j);
        }
    }
    CMatrix C;
    C = A * B;

    A.PrintMat("A");
    printf("Trace of A = %f\n", A.Trace());
    printf("Determinant of A = %f\n\n", A.Det());

    B.PrintMat("B");
    printf("Trace of B = %f\n", B.Trace());
    printf("Determinant of B = %f\n\n", B.Det());

    C.PrintMat("C");
    // printf("Trace of C = %f\n", C.Trace()); // impossible
    // printf("Determinant of C = %f\n\n", C.Det()); // impossible
}

void test_sqmatrix2() {
    CSQMatrix sqA(3);
    CSQMatrix sqM1, sqM2, sqM3, sqM4;

    for (int i = 0; i < sqA.GetRow(); i++) {
        for (int j = 0; j < sqA.GetCol(); j++) {
            if (i > j) sqA.SetValue(i, j, 0.0);
            else sqA.SetValue(i, j, 1.0);
        }
    }

    sqA.PrintMat("sqA");
    printf("Trace of sqA = %f\n", sqA.Trace());
    printf("Determinant of sqA = %f\n\n", sqA.Det());

    sqM1 = sqA + sqA;
    sqM1.PrintMat("sqM1 = sqA + sqA ");

    sqM2 = sqA * sqA;
    sqM2.PrintMat("sqM2 = sqA * sqA ");

    sqM3 = sqM2 * sqA;
    sqM3.PrintMat("sqM3 = sqM2 * sqA ");

    sqM4 = sqA * sqA * sqA;
    sqM4.PrintMat("sqM4 = sqA * sqA * sqA ");
}

int main() {
    // test_matrix1();
    // test_sqmatrix1();
    test_sqmatrix2();

    return 0;
}

/* Result of test_matrix1()
A=
[   1.00   2.00]
[   2.00   3.00]

B=
[   1.00   0.00]
[   2.00   1.00]

C=
[   2.00   2.00]
[   4.00   4.00]

D=
[   5.00   2.00]
[   8.00   3.00]
*/

/* Result of test_sqmatrix1()
A=
[   1.00   2.00]
[   2.00   3.00]

Trace of A = 4.000000
Determinant of A = -1.000000

B=
[   1.00   0.00]
[   2.00   1.00]

Trace of B = 2.000000
Determinant of B = 1.000000

C=
[   5.00   2.00]
[   8.00   3.00]
*/

/* test_sqmatrix2()
sqA=
[   1.00   1.00   1.00]
[   0.00   1.00   1.00]
[   0.00   0.00   1.00]

Trace of sqA = 3.000000
Determinant of sqA = 1.000000

sqM1 = sqA + sqA =
[   2.00   2.00   2.00]
[   0.00   2.00   2.00]
[   0.00   0.00   2.00]

sqM2 = sqA * sqA =
[   1.00   2.00   3.00]
[   0.00   1.00   2.00]
[   0.00   0.00   1.00]

sqM3 = sqM2 * sqA =
[   1.00   3.00   6.00]
[   0.00   1.00   3.00]
[   0.00   0.00   1.00]

sqM4 = sqA * sqA * sqA =
[   1.00   3.00   6.00]
[   0.00   1.00   3.00]
[   0.00   0.00   1.00]
*/