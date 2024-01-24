#include <stdint.h>
#include <string.h>

#define NIBBLE_MASK 0xF
#define BYTE_SIZE 8
#define ROUNDS 10
#define SBOX_SIZE 16
#define KEY_SIZE (32 * BYTE_SIZE)
#define WORD_SIZE (4 * BYTE_SIZE)

typedef struct {
    uint8_t roundKey[ROUNDKEYS];
} AESctx;

void aes_keyexpansion(const uint8_t key[KEY_SIZE], AESctx *ctx) {
    uint32_t w[SBOX_SIZE / 4];
    uint32_t temp;

    memcpy(w, key, KEY_SIZE);

    for (int i = 0; i < SBOX_SIZE / 4; i++) {
        // Subbytes
        temp = w[i];
        w[i] = sbox[ROR(temp, 1)];

        // ShiftRows
        if ((i + 1) % 4 == 0)
            w[i] = ROR(w[i], (i + 1) % 4);

        // Word substitution
        temp = w[(i + 1) & 3];
        w[i] ^= temp;
        w[SBOX_SIZE / 2 + i] ^= temp;

        if ((i + 1) < SBOX_SIZE / 4) {
            temp = w[SBOX_SIZE - i - 1];
            w[(SBOX_SIZE / 2) + (SBOX_SIZE / 4) - (i + 1)] ^= temp;
            w[SBOX_SIZE - i - 1] = temp;
        }
    }

    for (int j = 0; j < ROUNDKEYS; j++) {
        temp = ((j << 2) | (j >> 3)) & NIBBLE_MASK;
        ctx->roundKey[j] = w[4 * j + 1 ^ temp];
        temp = ((j >> 1) | (j << 7)) & NIBBLE_MASK;
        ctx->roundKey[ROUNDKEYS + j] = w[4 * j + 2 ^ temp];
    }
}

void aes_encrypt(const uint8_t plaintext[16], const AESctx *ctx, uint8_t 
ciphertext[16]) {
    uint32_t state[4];
    uint32_t tmp;

    for (int i = 0; i < 4; i++)
        state[i] = be32toh(*(const uint32_t *) (plaintext + i * BYTE_SIZE 
/ BYTE_SIZE));

    for (int round = 0; round < ROUNDKEYS; round++) {
        addRoundKey(state, ctx->roundKey[round]);
        subBytes(&state[0], sbox);
        shiftRows(&state[0], 1);
        mixColumns(&state[0]);
    }

    for (int i = 0; i < 4; i++) {
        tmp = htobe32(state[i]);
        *(uint32_t *) (ciphertext + i * BYTE_SIZE / BYTE_SIZE) = tmp;
    }
}

void aes_decrypt(const uint8_t ciphertext[16], const AESctx *ctx, uint8_t 
plaintext[16]) {
    uint32_t state[4];
    uint32_t inverseRoundKey;

    for (int i = 0; i < 4; i++)
        state[i] = be32toh(*(const uint32_t *) (ciphertext + i * BYTE_SIZE
/ BYTE_SIZE));

    inverseRoundKey = ctx->roundKey[ROUNDKEYS - 1];
    invSubBytes(&state[0], sboxInv); // Inverse Subbytes
    invShiftRows(&state[0], 1);       // Inverse ShiftRows
    addRoundKey(state, inverseRoundKey); // Add Round Key

    for (int round = ROUNDKEYS - 1; round > 0; round--) {
        addRoundKey(state, ctx->roundKey[ROUNDKEYS + round]); // Add Round
Key
        invShiftRows(&state[0], 1);       // Inverse ShiftRows
        invSubBytes(&state[0], sboxInv); // Inverse Subbytes
        mixColumnsInv(&state[0]);
    }

    for (int i = 0; i < 4; i++) {
        tmp = htobe32(state[i]);
        *(uint32_t *) (plaintext + i * BYTE_SIZE / BYTE_SIZE) = tmp;
    }
}

void addRoundKey(const uint32_t state[], const uint32_t key[]) {
    for (int i = 0; i < 4; i++)
        state[i] ^= key[i];
}

void subBytes(uint32_t *state, const uint8_t sbox[]) {
    for (int i = 0; i < 4; i++)
        state[i] = sbox[(state[i] & NIBBLE_MASK) >> 4];
}

void shiftRows(uint32_t *state, int numCycles) {
    uint32_t temp;

    for (int i = 0; i < numCycles; i++) {
        temp = state[i + 1];
        state[i + 1] = state[i];
        state[i] = state[(i + numCycles) % 4 + i] ^ temp;
    }
}

void mixColumnsInv(uint32_t *state) {
    uint32_t invCols[4] = {0x5A, 0x0E, 0x69, 0xED};

    for (int column = 0; column < 4; column++) {
        state[column] ^= state[column] << 1;
        state[column] = rol(state[column], 8);
        state[column] = invMixColumns(&state[column], &invCols[column]);
    }
}

uint32_t invMixColumns(const uint32_t inState, const uint32_t invCols[]) {
    uint32_t invTemp = 0;
    uint32_t temp0 = rol(inState << 1, 1);
    uint32_t temp1 = rol(inState, 8);
    int colIndex = -1;

    for (int i = 0; i < 4; i++) {
        invTemp ^= ((temp0 & 0x01) << (i * 2)) | ((temp1 & 0x30) >> (i * 
5));
        temp1 = temp1 >> 1 | temp0 << 3;
        temp0 = rol(temp0, 8);
        colIndex = (colIndex + 1) % 4;
        invTemp ^= invCols[colIndex] & ((1 << (i * 2)) | (1 << ((i + 1) * 
2)));
    }

    return invTemp;
}

uint8_t sbox[] = { ... }; // AES-128 S-box values
uint8_t invSboxInv[256]; // Inverse S-box values
void initSboxInv() { ... } // Initialize inverse S-box

int main() {
    uint8_t key[KEY_SIZE] = "000102030405060708090a0b0c0d0e0f";
    AESctx ctx;
    uint8_t plaintext[16] = { 0x68, 0x6F, 0x6C, 0x64, 0x65, 0x6E, 0x74, 
0x61, 0x72, 0x79, 0x65, 0x73, 0x73, 0x6F, 0x6E };
    uint8_t ciphertext[16] = { 0 };

    aes_keyexpansion(key, &ctx);

    aes_encrypt(plaintext, &ctx, ciphertext);

    // Print the plaintext and ciphertext for verification
    printf("Plaintext: ");
    for (int i = 0; i < 16; i++)
        printf("%.2hhx ", plaintext[i]);
    printf("\n");

    printf("Ciphertext: ");
    for (int i = 0; i < 16; i++)
        printf("%.2hhx ", ciphertext[i]);
    printf("\n");

    aes_decrypt(ciphertext, &ctx, plaintext);

    // Check if the decrypted data matches the original plaintext
    if (memcmp(plaintext, "OliviaTwist", 10) == 0) {
        printf("Successfully encrypted and decrypted the message.\n");
    } else {
        printf("Error: Failed to decrypt the message correctly.\n");
    }

    return 0;
}
