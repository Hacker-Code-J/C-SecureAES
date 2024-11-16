// GF256.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>  // C언어 헤더 파일

//바이트 저장용 데이터 형 정의
// byte B; // B = b7b6...b1b0 = b7x^7 + b6x^6 + ... + b1x + b0
typedef unsigned char byte; // byte는 GF(2^8) 유한체의 원소를 저장하는 데이터 유형

//유한체 GF(2^8)의 원소 a,b를 더하기
// a = a7a6...a1a0 = a7x^7 + a6x^6 + ... + a1x + a0
// b = b7b6...b1b0 = b7x^7 + b6x^6 + ... + b1x + b0
// c = a + b = (a7+b7)x^7 + (a6+b6)x^6 + ... + (a0+b0)
// c = (a7^b7)(a6^b6)...(a0^b0) = (a7a6...a1a0)^(b7b6...b1b0)
// 계수들의 덧셈(예: a7+b7)은 GF(2)의 덧셈 (계수는 0 또는 1)
// 계수들의 덧셈은 xor와 동일하다. 예: a7+b7 = a7 xor b7 = a7 ^ b7
byte GF256_add(byte a, byte b) {
    return a ^ b;
}

// GF(2^8)의 원소를 비트열로 출력하기
// x = x7x6...x0 ==> [x7, x6, ... , x0]
void GF256_print_bin(byte x) {
    byte x_bin[8]; //각 비트를 저장할 배열
    for (int i = 0; i < 8; i++) {
        // x를 오른쪽으로 7,6,5,...,0번 쉬프트
        // x7,  x7x6, x7x6x5, .... , x7x6...x0
        x_bin[i] = (x >> (7 - i)) & 0x01; 
    }
    for (int i = 0; i < 8; i++) {
        printf("%d", x_bin[i]);
    }
    printf("\n");
}

// GF(2^8)의 원소를 다항식으로 출력하기
// a = [a7a6...a0] => a7x^7 + ... + a0
void GF256_print_poly(byte a) {
    byte a_bin[8]; //각 비트를 저장할 배열 [a7, a6, ... , a0]
    for (int i = 0; i < 8; i++) { 
        // a를 오른쪽으로 7,6,5,...,0번 쉬프트
        a_bin[i] = (a >> (7 - i)) & 0x01;
    }
    int coef, expo;

    for (int i = 0; i < 7; i++) {  // x^7항부터 x항까지
        coef = a_bin[i]; // a7, a6, ... , a1
        expo = 7 - i; // 7, 6, .... , 1
        if (coef == 1) {
            printf("x^%1d + ", expo);
        }
    }
    //상수항
    coef = a_bin[7];
    if (coef == 1) {
        printf("1\n");
    }
    else {
        printf("0\n");
    }
}

// 다항식에 x를 곱하기
// a = a7a6...a1a0 = a7x^7 + a6x^6 + ... + a1x + a0
// xtime(a) = a6x^7 + a5x^6 + ... + a0x (a7 = 0)
// xtime(a) = a6x^7 + a5x^6 + ... + a0x + [x^4+x^3+x+1] (a7 = 1) 
// [x^4+x^3+x+1] = [0001 1011] = [0x1b]
byte GF256_xtime(byte a) {
    int msb; // a7을 저장(7차항의 계수)
    byte result; // 계산결과를 저장

    msb = (a >> 7) & 0x01; // a7
    if (msb == 0) { // a7=0
        result = a << 1; //[a6a5...a0 0]
    }
    else { // a7=1
        result = (a << 1) ^ 0x1b; // [0x1b] = [x^4+x^3+x+1] = [x^8]
    }
    return result;
}

byte GF256_xtime_simple(byte a) {
    return (((a >> 7) & 0x01) == 0) ? a << 1 : (a << 1) ^ 0x1b;
}

// GF(2^8)의 (다항식) 곱셈
// f(x) = a7x^7 + a6x^6 + ... + a1x + a0
// g(x)
// h(x) = g(x)f(x) = g(x)*a7x^7 + g(x)*a6x^6 + ... + g(x)*a1x + g(x)*a0
// 단계별 계산:
//      h(x) <= g(x)*a7
//      h(x) <= x*g(x)*a7 + g(x)*a6
//      h(x) <= x*(x*g(x)*a7 + g(x)*a6) + g(x)*a5
//            ...
//      h(x) <= x*(   ...   + g(x)*a1) + g(x)*a0
//  계수 ai에 g(x)를 곱한 결과는 g(x) 또는 0 뿐이다!
byte GF256_mul(byte f, byte g) {
    byte h; // 곱셈결과 저장
    int coef; // 계수
    
    h = 0x00; // 초깃값 h(x) = 0
    for (int i = 7; i >= 0; i--) { // a7, a6, a5, ... ,a1, a0 순서로 반복
        coef = (f >> i) & 0x01; // a7, a6, a5, ... ,a1, a0
        //h = GF256_xtime(h); 
        h = GF256_xtime_simple(h);
        if (coef == 1) {  // 계수 ai가 1인 경우만 g(x)를 h(x)에 더한다
            h = GF256_add(h, g);
        }
    }
    return h;
}

//=====================//
// Sbox 만들기  y = S[x] = A x^(-1) + b 

//GF(2^8)의 원소 x의 곱셈의 역원 x^(-1) = x^254
// x in GF(2^8)에 대하여 ord(x)는 x^d = 1인 최소의 자연수 d를 의미한다. (x는 0이 아님)
// 모든 원소 x(x는 0이 아님)의 ord(x)는 1부터 255 사이의 값이다.
// 또한, ord(x)는 255의 약수이다.  d = ord(x)이면, d*k = 255 인 자연수 k가 존재한다.
// x^255 = x^(d*k) = (x^d)^k = 1^k = 1.
// 따라서, x^254 = x^(-1)
// x^254 = x^(1111 1110)
//       = x^(1000 0000) * x^(0100 0000) * x^(0010 0000) *... * x^(0000 0010)
byte GF256_inv(byte x) {  // x^254 = x^(1111 1110)
    byte x_inv, term;
    term = x; 

    //x=0이 입력되면, 역원은 존재하지 않지만, 0을 리턴한다.
    if (x == 0) return 0x00;

    x_inv = 1; //곱하기에 대한 초기값
    for (int i = 0; i < 7; i++) {
        term = GF256_mul(term, term); // x^2, x^4, x^8, ... , x^128  (128 = 2^7)
        x_inv = GF256_mul(x_inv, term); // (1), x^2, x^2*x^4, (x^2*x^4)*x^8, ...
    }
    return x_inv;
}


//Affine 변환 w --> Aw + b
byte AES_Affine(byte w) {
    const byte A[8][8] = { // variable => stack, const => literal
        {1, 0, 0, 0, 1, 1, 1, 1},
        {1, 1, 0, 0, 0, 1, 1, 1},
        {1, 1, 1, 0, 0, 0, 1, 1},
        {1, 1, 1, 1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 1, 1, 1, 1, 1}
    };
    //순서에 주의! b_vec[8] = { b0, b1, b2, ... , b7}
    byte b_vec[8] = { 1, 1, 0, 0, 0, 1, 1, 0 };

    // 바이트 w = (msb) w7 w6 w5 w4 w3 w2 w1 w0 (lsb)
    byte w_vec[8]; // w_vec[8] = {w0, w1, ... , w7} (순서!)
    byte y_vec[8]; // y_vec[8] = {y0, y1, ... , y7} (순서!)

    // w --> w_vec[]
    for (int i = 0; i < 8; i++) {
        w_vec[i] = (w >> i) & 0x01;
    }
    // w --> y = Aw + b = b + Aw
    // y[i] = A[i][0]*w[0] + A[i][1]*w[1] + ... + A[i][7]*w[7]
    for (int i = 0; i < 8; i++) {
        //y_vec[i] = 0x00; 
        y_vec[i] = b_vec[i];
        for (int j = 0; j < 8; j++) {
            y_vec[i] ^= A[i][j] * w_vec[j]; // GF(2) 연산
        }
    }

    // y_vec[] --> y
    byte y;
    y = 0;
    for (int i = 0; i < 8; i++) {
        y ^= (y_vec[i] << i);
    }
    return y;
}

// AES의 Sbox 만들기
void Get_AES_Sbox(byte S[256]) {
    byte w;

    for (int x = 0; x < 256; x++) { // S[0], S[1], ... , S[255]
        w = GF256_inv(x); //  w = x^(-1)
        S[x] = AES_Affine(w); // y = Aw + b
    }
}

// GF(2^8)의 원소의 위수(order) 구하기
// x가 0이 아닌 GF(2^8)의 원소이면, 유한한 위수를 갖는다.
// x, x^2, x^3, ... , x^d=1  ==> d = ord(x)
// H = {x, x^2, x^3, ... , x^d=1}는 
// 곱셈군 G = {1, 2, ... , 255}의 순환부분군(cyclic subgroup)이 된다.
// 부분군 H의 크기는 G의 크기를 나눈다. ( |H| | |G| ) - Lagrange의 정리
int GF256_ord(byte x) {
    byte temp;
    int ord;

    if (x ==  0) return -1; // 0은 곱셈군의 원소가 아님. 의미없음.

    temp = x;
    ord = 1;
    while (temp != 0x01) {  // x, x^2, x^3, ...
        temp = GF256_mul(temp, x);
        ord++;
    }
    return ord;
}

// Sbox의 역함수 ISbox 구하기 
void Get_AES_Inv_Sbox(byte IS[256]) {
    byte Sbox[256]; // Sbox

    Get_AES_Sbox(Sbox);

    // y = S[x] <==> x = IS[y] = IS[ S[x] ]
    // IS[] 채우기 (1)
    // IS[0] = , IS[1] = , IS[2] = , ... IS[255] = 
    // IS[] 채우기 (2)
    // IS[ S[0] ] = 0, IS[ S[1] ] = 1, ... , IS[ S[255] ] = 255
    for (int x = 0; x < 256; x++) { 
        IS[Sbox[x]] = x;         
    }
}

// ISbox를 직접 만들기
// x --> y = S[x] = A*x^(-1) + b = Aw + b
// x = IS[y] 
// Aw = y+b (+b와 -b는 GF(2)에서 동일함)
// w = A^(-1)(y+b) <-----  A^(-1)는 A의 역행렬
// x = w^(-1)  <------- w^(-1)는 GF(2^8)에서 w의 곱셈의 역원

byte AES_Inv_Affine(byte y) {  // w = A^(-1)(y+b)
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
    //순서에 주의! b_vec[8] = { b0, b1, b2, ... , b7}
    byte b_vec[8] = { 1, 1, 0, 0, 0, 1, 1, 0 };

    // 바이트 w = (msb) w7 w6 w5 w4 w3 w2 w1 w0 (lsb)
    byte w_vec[8]; // w_vec[8] = {w0, w1, ... , w7} (순서!)
    byte y_vec[8]; // y_vec[8] = {y0, y1, ... , y7} (순서!)
    byte yb_vec[8]; // yb = y+b

    // y --> y_vec[]
    for (int i = 0; i < 8; i++) {
        y_vec[i] = (y >> i) & 0x01;
    }
    // yb_vec = y_vec + b_vec
    for (int i = 0; i < 8; i++) {
        yb_vec[i] = y_vec[i] ^ b_vec[i];
    }

    // y --> w = A^(-1) (y+b) 
    for (int i = 0; i < 8; i++) {
        w_vec[i] = 0x00; 
        for (int j = 0; j < 8; j++) {
            w_vec[i] ^= IA[i][j] * yb_vec[j]; // GF(2) 연산
        }
    }

    // w_vec[] --> w
    byte w;
    w = 0;
    for (int i = 0; i < 8; i++) {
        w ^= (w_vec[i] << i);
    }
    return w;
}

// AES의 ISbox 만들기 (Inv_Affine 함수를 이용한 버전)
void Get_AES_Inv_Sbox_from_Affine(byte IS[256]) {
    byte w;

    for (int y = 0; y < 256; y++) { // IS[0], IS[1], ... , IS[255]
        w = AES_Inv_Affine(y); // w = A^(-1) (y+b)
        IS[y] = GF256_inv(w); //  x = IS[y] = w^(-1)
    }
}

//=====================//

void test_GF256() {
    byte a, b, c;
    a = 0x57;
    b = 0x83;
    c = GF256_mul(a, b); // 0xc1 = 0x57*0x83

    printf("a = %2d = 0x%02x = ", a, a);
    GF256_print_bin(a);
    GF256_print_poly(a);
    printf("b = %2d = 0x%02x = ", b, b);
    GF256_print_bin(b);
    GF256_print_poly(b);
    printf("c = %2d = 0x%02x = ", c, c);
    GF256_print_bin(c);
    GF256_print_poly(c);

    byte d, e;
    d = GF256_inv(a);
    printf("d = %2d = 0x%02x = ", d, d);
    GF256_print_bin(d);
    GF256_print_poly(d);

    e = GF256_mul(a, d);
    printf("e = %2d = 0x%02x = ", e, e);
    GF256_print_bin(e);
    GF256_print_poly(e);
}

void test_Sbox() {
    byte Sbox[256];

    Get_AES_Sbox(Sbox);

    printf("Sbox = \n");
    for (int i = 0; i < 256; i++) {
        printf(" %02x", Sbox[i]);
        if ((i % 16) == 15) printf("\n");
    }
    printf("\n");

    byte ISbox[256];
    Get_AES_Inv_Sbox(ISbox);

    printf("Inv_Sbox = \n");
    for (int i = 0; i < 256; i++) {
        printf(" %02x", ISbox[i]);
        if ((i % 16) == 15) printf("\n");
    }
    printf("\n");

    Get_AES_Inv_Sbox_from_Affine(ISbox);
    printf("Inv_Sbox = \n");
    for (int i = 0; i < 256; i++) {
        printf(" %02x", ISbox[i]);
        if ((i % 16) == 15) printf("\n");
    }
    printf("\n");


    for (int i = 0; i < 256; i++) {
        if (ISbox[Sbox[i]] != i) {
            printf("Sbox-ISbox error!\n");
            return; //오류로 종료!
        }
    }
    printf("Sbox_ISbox test OK!\n");
}

void test_order() {
    int ord;
    int count_full_order;
    
    count_full_order = 0;

    for (int i = 0; i < 256; i++) {
        ord = GF256_ord(i);
        printf("ord(%02x) = %d\n", i, ord);
        if (ord == 255) count_full_order++;
    }
    printf("The number of x s.t. ord(x)=255 is %d", count_full_order);
}


