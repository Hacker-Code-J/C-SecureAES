u8 GF256_mul(u8 f, u8 g) {
	u8 h = 0x00; // h = f * g
    u8 coef;
	
    for (i8 i = 7; i >= 0; i--) {
        coef = (f >> i) & 0x01; // Get the coefficient f_i
        h = GF256_xtime(h);     // Multiply h by x
        if (coef == 1) h ^= g;  // If f_i = 1, add g(x) to h(x)
    }

	return h;
}
