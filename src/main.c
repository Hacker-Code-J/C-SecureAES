#include "aes.h"
#include "aes_utils.h"
#include "aesavs.h"

#include "aes_8bit_sample.h"

#define GETU32(pt) ((u32)(pt)[0] << 0x18) ^ \
                   ((u32)(pt)[1] << 0x10) ^ \
                   ((u32)(pt)[2] << 0x08) ^ \
                   ((u32)(pt)[3]        )

#define PUTU32(ct, st) { (ct)[0] = (u8)((st) >> 0x18); \
                         (ct)[1] = (u8)((st) >> 0x10); \
                         (ct)[2] = (u8)((st) >> 0x08); \
                         (ct)[3] = (u8)((st)        ); }

int main() {
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