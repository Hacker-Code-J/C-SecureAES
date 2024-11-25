// GF2_Matrix.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>

#define MATRIX_MAX_SIZE 20
typedef unsigned char byte;

typedef struct {  // GF(2) 행렬을 나타내는 구조체
    byte M[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE];  // 0 또는 1만 저장한다.
    int row;
    int col;
} GF2_Matrix; 

void GF2_Matrix_print(GF2_Matrix A) {
    for (int i = 0; i < A.row; i++) {  //각 행에 대하여
        printf("[");
        for (int j = 0; j < A.col; j++) {
                printf(" %1d", A.M[i][j]); // 0 또는 1만 출력
        }
        printf("]\n");
    }
    printf("\n");
}

// 행렬 변수의 초기화
GF2_Matrix GF2_Matrix_Init() {
    GF2_Matrix A;
    A.row = 1; A.col = 1;
    A.M[0][0] = (byte)0; 

    return A;
}

// C = A+B (행렬의 덧셈)
GF2_Matrix GF2_Matrix_Add(GF2_Matrix A, GF2_Matrix B) {
    GF2_Matrix result;

    result = GF2_Matrix_Init();
    // 행렬의 규격 확인
    if ((A.col != B.col) || (A.row != B.row)) {
        printf("(GF2_Matrix_Add) Matrix size error\n");
        return result;
    }
    result.row = A.row;
    result.col = A.col;
    for (int i = 0; i < result.row; i++)
        for (int j = 0; j < result.col; j++)
            //result.M[i][j] = (A.M[i][j] + B.M[i][j]) % 2;  // 2로 나눈 나머지
            result.M[i][j] = A.M[i][j] ^ B.M[i][j];  // xor
    return result;
}

// C = A*B (행렬 곱)  [m,k]*[k,n] = [m,n] GF(2)의 곱 (0*0, 0*1, 1*0, 1*1)
GF2_Matrix GF2_Matrix_Mul(GF2_Matrix A, GF2_Matrix B) {
    GF2_Matrix AB;
    AB = GF2_Matrix_Init();

    //행렬의 규격 확인 
    if (A.col != B.row) {
        printf("(GF2_Matrix_Mul) Matrix size error\n");
        return AB;
    }

    AB.row = A.row;
    AB.col = B.col;
    for (int i = 0; i < AB.row; i++) {
        for (int j = 0; j < AB.col; j++) {
            AB.M[i][j] = 0.0;
            for (int k = 0; k < A.col; k++) {
                AB.M[i][j] += A.M[i][k] * B.M[k][j];
            }
        }
    }
    return AB;
}

//두 행을 바꾸기
void GF2_Mat_Exchange_Row(GF2_Matrix& A, int row1, int row2) {
    byte temp; // temp는 0 또는 1
    for (int j = 0; j < A.col; j++) {
        //A[row1][j] <--> A[row2][j]
        temp = A.M[row1][j];
        A.M[row1][j] = A.M[row2][j];
        A.M[row2][j] = temp;
    }
}

//한 행에 상수배 하기
void GF2_Mat_Scalar_Mul_Row(GF2_Matrix& A, byte k, int row) {
    // k(상수)는 0 또는 1
    for (int j = 0; j < A.col; j++)
        //A.M[row][j] = (k == 1) ? A.M[row][j] : 0;
        A.M[row][j] *= k;
}

//한 행의 상수배를 다른 행에 더하기
void GF2_Mat_Row_Add(GF2_Matrix& A, byte k, int row_src, int row_target) {
    for (int j = 0; j < A.col; j++)
        A.M[row_target][j] ^= k * A.M[row_src][j];  // xor 
}

//역행렬 구하기
GF2_Matrix GF2_Matrix_Inverse(GF2_Matrix A) {
    GF2_Matrix InvA;
    InvA = GF2_Matrix_Init();
    if (A.row != A.col) { // 정사각행렬인지 확인
        printf("(GF2_Matrix_Inverse) Non-Sqaure Matrix error\n");
        return InvA;
    }

    // AA = [A|I] -------> [I|A^(-1)]
    GF2_Matrix AA;
    AA.row = A.row;
    AA.col = 2 * A.col;
    for (int i = 0; i < A.row; i++) {
        for (int j = 0; j < A.col; j++) {
            AA.M[i][j] = A.M[i][j];
            AA.M[i][j + A.col] = (i == j) ? 1 : 0;
        }
    }

    //GF2_Matrix_print(AA);

    // R-REF(Reduced Row Echelon Form) [A|I] ==> [I|A^(-1)]
    int pivot_row; // 각 열을 계산하는 단계에서 '1'이 있는 위치의 행
    for (int j = 0; j < A.col; j++) { // A의 각 열에 대하여...
        // pivot: AA[0][0], AA[1][1], AA[2][2], ... 
        pivot_row = -1; // 초깃값
        for (int i = j; i < A.row; i++) { // AA[j][j], AA[j+1][j], ...
            if (AA.M[i][j] != 0) {
                pivot_row = i;
                break;  // for-loop 밖으로
            }
        }
        if (pivot_row == -1) { // 해당 열의 모든 원소가 0이면,
            printf("(GF2_Matrix_Inverse) Not Invertible\n");
            return InvA;
        }
        if (pivot_row != j) {
           GF2_Mat_Exchange_Row(AA, j, pivot_row);
        }
        //GF2에서는 필요하지 않음: Mat_Scalar_Mul_Row(AA, 1. / AA.M[j][j], j);

        for (int i = 0; i < A.row; i++) {
            if (i != j) { // A[j][j] 이 포함되지 않은 행에 대해서만...
                GF2_Mat_Row_Add(AA, AA.M[i][j], j, i);
            }
        }
        //GF2_Matrix_print(AA);
    }

    InvA.row = A.row;
    InvA.col = A.col;
    for (int i = 0; i < InvA.row; i++)
        for (int j = 0; j < InvA.col; j++)
            InvA.M[i][j] = AA.M[i][j + A.col];

    return InvA;
}

// A --> k*A  (k=0,1)
GF2_Matrix GF2_Matrix_scalar(GF2_Matrix A, byte k) {
    GF2_Matrix kA;
    kA = GF2_Matrix_Init();

    kA.row = A.row;
    kA.col = A.col;

    for (int i = 0; i < kA.row; i++)
        for (int j = 0; j < kA.col; j++)
            kA.M[i][j] = k * A.M[i][j];
    return kA;
}

void test_GF2_matrix() {
    const byte A[8][8] = { 
    {1, 0, 0, 0, 1, 1, 1, 1},
    {1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1},
    {1, 1, 1, 1, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 1, 1, 1, 1, 1}
    };
    
    const byte IA[8][8] = {  // A^(-1)을 미리 준비한다. 
    {0, 0, 1, 0, 0, 1, 0, 1},
    {1, 0, 0, 1, 0, 0, 1, 0},
    {0, 1, 0, 0, 1, 0, 0, 1},
    {1, 0, 1, 0, 0, 1, 0, 0},
    {0, 1, 0, 1, 0, 0, 1, 0},
    {0, 0, 1, 0, 1, 0, 0, 1},
    {1, 0, 0, 1, 0, 1, 0, 0},
    {0, 1, 0, 0, 1, 0, 1, 0}
    };

    GF2_Matrix GF2_A;
    GF2_A.row = 8; GF2_A.col = 8;
    for (int i = 0; i < GF2_A.row; i++)
        for (int j = 0; j < GF2_A.col; j++)
            GF2_A.M[i][j] = A[i][j];

    GF2_Matrix GF2_IA;
    GF2_IA.row = 8; GF2_IA.col = 8;
    for (int i = 0; i < GF2_IA.row; i++)
        for (int j = 0; j < GF2_IA.col; j++)
            GF2_IA.M[i][j] = IA[i][j];

    printf("A =\n");
    GF2_Matrix_print(GF2_A);
    printf("invA =\n");
    GF2_Matrix_print(GF2_IA);
    
    printf("=== Inverse of A\n");
    GF2_Matrix InvA;
    InvA = GF2_Matrix_Inverse(GF2_A);
    GF2_Matrix_print(InvA);
}



int main()
{
    test_GF2_matrix();
}
