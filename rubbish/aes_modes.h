#ifndef _AES_MODES_H
#define _AES_MODES_H

#include <stddef.h>

#include "utils.h"

void pkcs7_pad(u8* block, size_t block_len, size_t input_len);

#endif // _AES_MODES_H