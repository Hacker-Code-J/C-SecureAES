u8 b[16] = { 0x00, 0x01, 0x02, 0x03,
             0x04, 0x05, 0x06, 0x07,
             0x08, 0x09, 0x0A, 0x0B,
             0x0C, 0x0D, 0x0E, 0x0F};

u32 x[4] = { GETU32(b       ), 
             GETU32(b + 0x04),
             GETU32(b + 0x08),
             GETU32(b + 0x0C)};