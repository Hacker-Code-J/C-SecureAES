u8 xtime(u8 f) {
	return (f & 0x80) ? (f << 1) ^ 0x1b : f << 1;
}
