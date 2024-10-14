#include <stdio.h>
#include <stddef.h>

typedef __int8_t i8;
typedef __int32_t i32;
typedef __uint8_t u8;
typedef __uint32_t u32;

u32 gcd(u32 a, u32 b) {
    u32 r;

    while (b != 0) {
        r = a % b;
        a = b;
        b = r;
    }

    return a;
}

u32 rec_gcd(u32 a, u32 b) {
    return (b == 0) ? a : rec_gcd(b, a % b);
}

u32 eea(u32 a, u32 b, u32* x, u32* y) {
    u32 an, bn;
    u32 ua, va, ub, vb;
    u32 new_an, new_bn;
    u32 n_ua, n_va, n_ub, n_vb;
    u32 q;

    an = a; bn = b;
    ua = 1; va = 0;
    ub = 0; vb = 1;

    while (bn != 0) {
        q = an / bn;
        new_an = bn;
        new_bn = an - bn * q;

        n_ua = ub; n_va = vb;
        n_ub = ua - ub * q; n_vb = va - vb * q;
        
        an = new_an; bn = new_bn;
        ua = n_ua; va = n_va; ub = n_ub; vb = n_vb;
    }

    *x = ua; *y = va;
    printf("[debug] x = %d, y = %d, an = %d\n", *x, *y, an);
    return an;
}


// Multiply two elements in GF(2^8) modulo the irreducible polynomial 0x11B
u32 gf_mul(u32 a, u32 b) {
    u32 result = 0;
    while (b > 0) {
        if (b & 1) {
            result ^= a;
        }
        a <<= 1;
        if (a & 0x100) {
            a ^= 0x11B; // Reduce by the irreducible polynomial if overflow
        }
        b >>= 1;
    }
    return result;
}

// Divide two elements in GF(2^8) (find quotient a/b in GF(2^8))
u32 gf_div(u32 a, u32 b) {
    u32 quotient = 0;
    int shift = 0;

    // Shift the divisor left until it lines up with the highest bit of the dividend
    while (b <= a) {
        b <<= 1;
        shift++;
    }

    // Perform the division
    while (shift >= 0) {
        if (a >= b) {
            a ^= b;
            quotient |= (1 << shift);
        }
        b >>= 1;
        shift--;
    }
    return quotient;
}

// Extended Euclidean Algorithm for GF(2^8)
u32 gf256_eea(u32 a, u32 b, u32* x, u32* y) {
    u32 an, bn;
    u32 ua, va, ub, vb;
    u32 new_an, new_bn;
    u32 n_ua, n_va, n_ub, n_vb;
    u32 q;

    an = a; bn = b;
    ua = 1; va = 0;
    ub = 0; vb = 1;

    while (bn != 0) {
        // Use GF(2^8) division to compute q (quotient)
        q = gf_div(an, bn);
        
        // Update new an and bn
        new_an = bn;
        new_bn = an ^ gf_mul(bn, q);  // an - bn * q in GF(2^8)
        
        // Update coefficients u and v
        n_ua = ub; n_va = vb;
        n_ub = ua ^ gf_mul(ub, q);  // ua - ub * q in GF(2^8)
        n_vb = va ^ gf_mul(vb, q);  // va - vb * q in GF(2^8)
        
        // Update values for the next iteration
        an = new_an; bn = new_bn;
        ua = n_ua; va = n_va; ub = n_ub; vb = n_vb;
    }

    *x = ua; *y = va;
    printf("[debug] x = %d, y = %d, an = %d\n", *x, *y, an);
    return an;
}

int main() {
    u32 a, b;
    a = 0xf1;
    b = 0x11b;

    // printf("a = %02x, b = %02x, gcd(%02x,%02x)=%02x\n", a, b, a, b, gcd(a,b));
    // printf("a = %02x, b = %02x, gcd(%02x,%02x)=%02x\n", a, b, a, b, rec_gcd(a,b));

    u32 p, q;
    p = 1; q = 1;
    u32 g = gf256_eea(a, b, &p, &q);
    printf("gcd(%03x,%03x) = %03x = %03x*%03x + %03x*%03x\n", a, b, g, p, a, q, b);

    return 0;
}