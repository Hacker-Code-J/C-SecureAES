#include "aes.h"

void KeySchedule(u32* rKey, const u8* uKey, u8 AES_VERSION) {
    /*
     * NK : Key Length (word)
    */
    u8 NK = (AES_VERSION == AES128) ? 4 : (AES_VERSION == AES192) ? 6 : 8;

    u32 temp;
	
	for (int i = 0; i < NK; i++) {
        rKey[i] = (u32)uKey[4*i] << 0x18 | 
                  (u32)uKey[4*i+1] << 0x10 | 
                  (u32)uKey[4*i+2] << 0x08 | 
                  (u32)uKey[4*i+3];
    }
    
    for (int i = NK; i < ((NK + 6) + 1) * 4; i++) {
        temp = rKey[i - 1];
        if (i % NK == 0) {
            temp = SUBWORD(ROTWORD(temp)) ^ rCon[i / NK - 1];
        } else if ((NK > 6) && (i % NK == 4)) {
            // Additional S-box transformation for AES-256
            temp = SUBWORD(temp);
        }
        rKey[i] = rKey[i - NK] ^ temp;
    }
}

void reverseKeySchedule(u8* uKey, const u32* rKey, u8 AES_VERSION) {
     /*
     * NK : Key Length (word)
     * NR : Number of Rounds
     * TSRK : Total Size of Round Keys (word)
     * NR = NK + 6;
     * TSRK = 4 * (NR + 1);
    */
    u8 NK, NR, TSRK;
    switch (AES_VERSION)
    {
    case AES192:
        NK = 6;
        NR = 12;
        TSRK = 52;
        break;
    case AES256:
        NK = 8;
        NR = 14;
        TSRK = 60;
        break;
    default:
        NK = 4;
        NR = 10;
        TSRK = 44;
        break;
    }
    
    u32 temp;
    u32 reversedKey[TSRK];

    // Copy the last round key as the starting point
    for (int i = 0; i < NK; i++) {
        reversedKey[(NR * NK) + i] = rKey[(NR * NK) + i];
    }

    // Reverse the key expansion process
    for (int i = (NR * NK) - 1; i >= 0; i--) {
        temp = reversedKey[i + 1];
        if (i % NK == 0) {
            temp = ROTWORD(temp);
            temp = SUBWORD(temp) ^ rCon[i / NK];
        } else if (NK > 6 && i % NK == 4) {
            temp = SUBWORD(temp);
        }
        reversedKey[i] = reversedKey[i + NK] ^ temp;
    }

    // Extract the user key
    for (int i = 0; i < NK; i++) {
        uKey[4 * i]     = (u8)(reversedKey[i] >> 24);
        uKey[4 * i + 1] = (u8)(reversedKey[i] >> 16);
        uKey[4 * i + 2] = (u8)(reversedKey[i] >> 8);
        uKey[4 * i + 3] = (u8)(reversedKey[i]);
    }
}