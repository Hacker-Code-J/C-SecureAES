#include "aes.h"
#include "aes_utils.h"
#include "aesavs.h"

// #include "aes_table_sample.h"
#include "aes_8bit_sample.h"
#include "aes_32bit_sample.h"

int main() {
    // TEST_AES8();
    TEST_AES32();
    // AES32_Enc_Table_Generate();
    // AES32_Dec_Table_Generate();
    
    // AES128_Opt_Comp();
    // AES128_Test();
    // MOVS_AES_VARTXT_TEST();
    // GF_MUL_Test();

    /* ============================= */

    // test_MixCol();
    // test_Mat_Mul();
    // test_MDS_power();
    // test_AES_round();
    
    return 0;
}