#include <stdio.h>
#include "complex.h"

void complex_test2();
void complex_test3();

int main() {
    // complex_test2();
    complex_test3();
    return 0;
}

void complex_test2() {
    Complex z;

    z.print();

    z.SetRe(1.0);
    z.SetIm(1.0);
    z.print();

    Complex z2;
    z2 = z.GetConjugate();
    z2.print();

    Complex w; // Instantiation; w is instance of Complex class.
    w.SetRe(1.0);
    w.SetIm(-1.0);
    w.print();

    Complex w2;
    w2 = w.GetConjugate();
    w2.print();

    Complex zw; // zw = z + w
    zw = z.Add(w);
    zw.print();

    Complex wz; // zw = z + w
    wz = w.Mul(z);
    wz.print();
}

void complex_test3() {
    Complex z1(1.0, 1.0);
    Complex z2(2.0, 2.0);
    Complex z3(3.0);

    z1.print();
    z2.print();
    z3.print();
}
