#include "aes_config.h"

static inline u8 xtime(u8 a) {
    return ((a & 0x80) == 0) ? a << 1 : (a << 1) ^ 0x1b;
}

// Te0[x] = [ 2*S(x), S(x), S(x), 3*S(x) ]
// Te1[x] = [ 3*S(x), 2*S(x), S(x), S(x) ]
// Te2[x] = [ 2*S(x), S(x), S(x), 3*S(x) ]
// Te3[x] = [ S(x), S(x), 3*S(x), 2*S(x) ]
// Te4[x] = [ S(x), S(x), S(x), S(x) ]
void AES32_Enc_Table_Generate();

// Td0[x] = [ e*S(x), 9*(x), d*S(x), b*S(x) ]
// Td1[x] = [ 9*(x), d*S(x), b*S(x), e*S(x) ]
// Td2[x] = [ d*S(x), b*S(x), e*S(x), 9*(x) ]
// Td2[x] = [ b*S(x), e*S(x), 9*(x), d*S(x) ]
void AES32_Dec_Table_Generate();