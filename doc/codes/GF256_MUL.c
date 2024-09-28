u8 GF256_MUL(u8 f, u8 g) {
	u8 h = 0;
	for (i8 = 0; i < 8; i++) {
		h ^= (g & 1) * f;  
		g >>= 1;	// g = g / 2.
		
		u8 msb = f & 0x80;
		f <<= 1;	// f = f * 2.
		// Conditional XOR without branching.
		f ^= (0 - ((f & 0x80) >> 7)) & 0x1b; // (f & 0x80) is the msb
	}
	
	return h;
}
