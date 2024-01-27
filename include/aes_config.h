#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef _AES_CONFIG_H
#define _AES_CONFIG_H

typedef int8_t i8;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint32_t u32;
typedef uint64_t u64;

#define AES128 (u8)128
#define AES192 (u8)192
#define AES256 (u8)256

#endif /* _AES_CONFIG_H */