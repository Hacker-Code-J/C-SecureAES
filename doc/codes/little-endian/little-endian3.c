u8 b[4] = { 0x00, 0x01, 0x02, 0x03 };
u32* px = (u32*)b;
u32 x, y;

x = *px;
y = ((u32)b[0] << 0x18) ^
    ((u32)b[1] << 0x10) ^
    ((u32)b[2] << 0x08) ^
    ((u32)b[3]        );