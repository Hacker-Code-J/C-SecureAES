#include "aes_config.h"

#ifndef _AES_UTILS_H
#define _AES_UTILS_H

void stringToByteArray(u8* byteArray, const char* hexString);
void stringToWordArray(u32* wordArray, const char* hexString);

#endif /* _AES_UTILS_H */