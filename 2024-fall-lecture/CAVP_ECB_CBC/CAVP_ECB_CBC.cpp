// g++ -g -Wall -Wextra CAVP_ECB_CBC.cpp HexByte.cpp AES32.cpp -o cavp-ecb-cbc
#include "HexByte.h"
#include "AES32.h"

/*
(Monte Carlo Test - ECB)
Key[0] = Key
PT[0] = PT
For i = 0 to 99
	Output Key[i]
	Output PT[0]

	For j = 0 to 999
		CT[j] = AES(Key[i], PT[j])
		PT[j+1] = CT[j]

	Output CT[j]
	Key[i+1] = Key[i] xor CT[j]
	PT[0] = CT[j]

KEY = 8d2e60365f17c7df1040d7501b4a7b5a
PLAINTEXT = 59b5088e6dadc3ad5f27a460872d5929
*/

void MCT_ECB();
void MCT_CBC();

int main() {
    // MCT_ECB();
    MCT_CBC();
    return 0;
}

void MCT_ECB() {
    const char* hex_key = "8d2e60365f17c7df1040d7501b4a7b5a";
	const char* hex_pt = "59b5088e6dadc3ad5f27a460872d5929";

    u8 key[16], pt[16], ct[16];
    u32 rk[11][4];

    Hex2Array(hex_key, 32, key);    // Key[0] = Key
    Hex2Array(hex_pt, 32, pt);       // PT[0] = PT

    cout << "MCT-ECB...(start)" << endl;

    for (int i = 0; i < 3; i++) {       // For i = 0 to 99
        print_b_array(key, 16, "KEY");  // Output Key[i]
        print_b_array(pt, 16, "PT");    // Output PT[0]

        AES32_Enc_KeySchedule(key, rk); 
        for (int j = 0; j < 1000; j++) {        // For j = 0 to 999
            AES32_Encrypt(pt, rk, ct);          // CT[j] = AES(Key[i], PT[j])
            copy_b_array(ct, 16, pt);           // PT[j+1] = CT[j]
        }

        print_b_array(ct, 16, "CT");    // Output CT[j]
        xor_b_array(key, 16, ct);       // Key[i+1] = Key[i] xor CT[j]
        copy_b_array(ct, 16, pt);       // PT[0] = CT[j]
        cout << endl;
    }
    cout << "MCT-ECB...(end)" << endl;
}

/*
(Monte Carlo Test - CBC)

Key[0] = Key
IV[0] = IV
PT[0] = PT

For i = 0 to 99
	Output Key[i]
	Output IV[i]
	Output PT[0]

	For j = 0 to 999
		If ( j=0 )
			CT[j] = AES(Key[i], IV[i] xor PT[j])
			PT[j+1] = IV[i]
		Else
			CT[j] = AES(Key[i], PT[j] xor CT[j-1])
			PT[j+1] = CT[j-1]

	Output CT[j]

	Key[i+1] = Key[i] xor CT[j]
	IV[i+1] = CT[j]
	PT[0] = CT[j-1]

KEY = 9dc2c84a37850c11699818605f47958c
IV = 256953b2feab2a04ae0180d8335bbed6
PLAINTEXT = 2e586692e647f5028ec6fa47a55a2aab
*/
void MCT_CBC() {
    const char* hex_key = "9dc2c84a37850c11699818605f47958c";
	const char* hex_iv = "256953b2feab2a04ae0180d8335bbed6";
    const char* hex_pt = "2e586692e647f5028ec6fa47a55a2aab";

    u8 key[16], pt[16], ct[16], prev_ct[16], iv[16];
    u32 rk[11][4];

    Hex2Array(hex_key, 32, key);    // Key[0] = Key
    Hex2Array(hex_iv, 32, iv);      // IV[0] = IV
    Hex2Array(hex_pt, 32, pt);      // PT[0] = PT

    cout << "MCT-CBC...(start)" << endl;

    for (int i = 0; i < 3; i++) {
        print_b_array(key, 16, "KEY");  // Output Key[i]
        print_b_array(iv, 16, "IV");    // Output IV[i]
        print_b_array(pt, 16, "PT");    // Output PT[0]

        AES32_Enc_KeySchedule(key, rk);
        for (int j = 0; j < 1000; j++) {
            if (j == 0) {
                xor_b_array(pt, 16, iv);
                AES32_Encrypt(pt, rk, ct);
                copy_b_array(iv, 16, pt);
            } else {
                copy_b_array(ct, 16, prev_ct);
                xor_b_array(pt, 16, prev_ct);
                AES32_Encrypt(pt, rk, ct);
                copy_b_array(prev_ct, 16, pt);
            }
        }
        print_b_array(ct, 16, "CT");    // Output CT[j]
        xor_b_array(key, 16, ct);       // Key[i+1] = Key[i] xor CT[j]
        copy_b_array(ct, 16, iv);       // IV[i+1] = CT[j]
        cout << endl;
    }
    cout << "MCT-CBC...(end)" << endl;
}