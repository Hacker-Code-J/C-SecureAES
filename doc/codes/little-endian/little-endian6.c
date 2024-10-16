#define PUTU32(ct, st) { (ct)[0] = (u8)((st) >> 0x18); \
                         (ct)[1] = (u8)((st) >> 0x10); \
                         (ct)[2] = (u8)((st) >> 0x08); \
                         (ct)[3] = (u8)((st)        ); }

u32 x[4] = { 0x00010203, 0x04050607, 0x08090A0B, 0x0C0D0E0F };
u8 y[16];

PUTU32(y       , x[0]);
PUTU32(y + 0x04, x[1]);
PUTU32(y + 0x08, x[2]);
PUTU32(y + 0x0C, x[3]);
