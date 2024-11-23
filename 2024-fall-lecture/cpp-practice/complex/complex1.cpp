#include <stdio.h>

// typedef struct {
//     double real;
//     double imag;
// } complex;

// int main() {
//     complex z;
//     z.real = 1.0;
//     z.imag = 2.0;
//     return 0;
// }

class Complex {
// Member Type: public, private, protected 
private:
    // z = z.real + z.imag i
    double real;    // real part (member variable)
    double imag;    // imaginary part (member variable)

public: // Member function or method
    void SetRe(double x) { real = x; }  // encapsulation
    void SetIm(double y) { imag = y; }
    double GetRe() { return real; }
    double GetIm() { return imag; }

    // Conjugate: z = x + yi -> x - yi
    Complex GetConjugate();
    void print();
    Complex Add(Complex z);
    Complex Mul(Complex z);
};

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

int main() {
    Complex z;

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



    return 0;
}