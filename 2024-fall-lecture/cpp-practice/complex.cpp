#include "complex.h"

Complex Complex::Mul(Complex z) {
    // (x1, y1) * (x2, y2) = (x1x2-y1y2, x1y2+x2y1)
    Complex w;

    w.real = real * z.real - imag * z.imag;
    w.imag = real * z.imag + z.real * imag;

    return w;
}

Complex Complex::Add(Complex z) {
    Complex w;
    w.real = z.real + real;
    // w.imag = z.imag + imag;
    w.SetIm(z.GetIm() + GetIm());

    return w;
}

Complex Complex::GetConjugate() {
    Complex z_bar;
    z_bar.real = real;
    z_bar.imag = -imag;
    return z_bar;
}

void Complex::print() { // member function or method
    if (imag >= 0)
        printf("(%.2f) + (%.2f) * i\n", real, imag);
    else
        printf("(%.2f) - (%.2f) * i\n", real, -imag); 
}