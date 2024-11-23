#pragma once

#include <stdio.h>

class Complex {
// Member Type: public, private, protected 
private:
    // z = z.real + z.imag i
    double real;    // real part (member variable)
    double imag;    // imaginary part (member variable)

    // for complex3.c
    static int complex_counter;
    static int print_counter;
public: // Member function or method
    // Overloading
    // Complex() { real = 0.0; imag = 0.0; }
    // Complex(double x, double y) { real = x; imag = y; } // Constructor
    // Complex(double x) { real = x; imag = 0.0; }
    
    // Constructor
    Complex(double x = 0.0, double y = 0.0) {
        int cnt;
        real = x; imag = y;
        IncComplexCounter();
        cnt = GetComplexCounter();
        printf("(Constructor) [%d] Complex(x,y) = (%f, %f)\n", cnt, real, imag);
    }
    // Copy Constructor
    Complex(const Complex &z) { 
        int cnt;
        real = z.real; imag = z.imag;
        IncComplexCounter();
        cnt = GetComplexCounter();
        printf("(Copy Constructor) [%d] Complex(x,y) = (%f, %f)\n", cnt, real, imag);
    }
    // Destructor
    ~Complex() { 
        DecComplexCounter(); 
        printf("[%d] (Destructor)\n", GetComplexCounter());
    }

    void SetRe(double x) { real = x; }  // encapsulation
    void SetIm(double y) { imag = y; }
    double GetRe() { return real; }
    double GetIm() { return imag; }

    // Conjugate: z = x + yi -> x - yi
    Complex GetConjugate();
    void print();
    
    // Method
    Complex Add(Complex z);
    Complex Mul(Complex z);
    // Static Method (Overloading)
    static Complex Add(Complex z1, Complex z2);
    static Complex Mul(Complex z1, Complex z2);

    // Overloading Operations
    Complex operator+(const Complex& z);
    Complex operator*(const Complex& z);

    // Counter
    static void ResetComplexCounter(int cnt = 0) { complex_counter = cnt; }
    static void ResetPrintCounter(int cnt = 0) { print_counter = cnt; }
    static int GetComplexCounter() { return complex_counter; }
    static int GetPrintCounter() { return print_counter; }
    static void IncComplexCounter() { complex_counter++; }
    static void DecComplexCounter() { complex_counter--; }
    static void IncPrintCounter() { print_counter++; }
};