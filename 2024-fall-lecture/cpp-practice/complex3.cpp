// g++ -g -Wall -Wextra complex3.cpp complex.cpp -o complex3
#include <stdio.h>
#include "complex.h"

void test_static_var() {
    printf("print_counter = %d\n", Complex::GetPrintCounter());

    Complex z1, z2, z3;

    z1.SetRe(1.0); z1.SetIm(1.0);
    z2.SetRe(2.0); z2.SetIm(2.0);
    z3.SetRe(3.0); z3.SetIm(3.0);

    Complex z4;
    z4 = z1.Add(z2);

    z1.print();
    z2.print();
    z3.print();
    z4.print();

    printf("print_counter = %d\n", Complex::GetPrintCounter());
}

int Complex::complex_counter = 0;
int Complex::print_counter = 0;

int main() {
    test_static_var();
    return 0;
}
