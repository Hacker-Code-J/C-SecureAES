#include "aes.h"

void KeyScheduleTest(void) {
    // u8 uKey[AES_KEY_SIZE] = { 0x00, };
    // RANDOM_KEY_GENERATION(uKey);
    u8 uKey[16] = { 
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
    };
    for (int i = 0; i < 16; i++) {
        printf("%02x:", uKey[i]);
    } printf("\n\n");

    u32 rKeys[44];
    KeySchedule(rKeys, uKey, AES128);
    for (int i = 0; i < 44; i++) {
        printf("RK[%02d]: %08x\n", i, rKeys[i]);
    }
}