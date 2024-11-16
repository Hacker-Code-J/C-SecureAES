#pragma once

#include <stdio.h>

class Complex {
// Member Type: public, private, protected 
private:
    // z = z.real + z.imag i
    double real;    // real part (member variable)
    double imag;    // imaginary part (member variable)

public: // Member function or method
    // Overloading
    Complex() { real = 0.0; imag = 0.0; }
    Complex(double x, double y) { real = x; imag = y; } // Constructor
    Complex(double x) { real = x; imag = 0.0; }
    
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