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
    printf("(Complex::Add)\n");
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
    IncPrintCounter();
    if (imag >= 0)
        printf("(%.2f) + (%.2f) * i\n", real, imag);
    else
        printf("(%.2f) - (%.2f) * i\n", real, -imag); 
}

Complex Complex::Add(Complex z1, Complex z2) {
    Complex w;
    w.real = z1.real + z2.real;
    w.imag = z1.imag + z2.imag;
    return w;
}

Complex Complex::Mul(Complex z1, Complex z2) {
    Complex w;
    w.real = z1.real * z2.real - z1.imag * z2.imag;
    w.imag = z1.real * z2.imag + z2.real * z1.imag;
    return w;
}

Complex Complex::operator+(const Complex& z) {
    Complex w;
    w.real = z.real + real;
    w.imag = z.imag + imag;
    return w;
}

Complex Complex::operator*(const Complex& z) {
    Complex w;
    w.real = real * z.real - imag * z.imag;
    w.imag = real * z.imag + z.real * imag;
    return w;
}