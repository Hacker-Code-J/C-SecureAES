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
