#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include <Wincrypt.h>

#define KEYLEN 32

uint8_t random_byte();

void generate_privatekey(uint8_t *key);