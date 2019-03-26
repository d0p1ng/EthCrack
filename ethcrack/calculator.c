#include "calculator.h"
#include <stdio.h>

int d = 0;

void ethaddress_calculate(uint8_t *privatekey, uint8_t *address) {
	secp256k1_start();
	int keylen = 65;
	uint8_t pubkey[64];
	uint8_t fixed_pubkey[64];

	int ret = secp256k1_ecdsa_pubkey_create(pubkey, &keylen, privatekey, 0);
	
	for(int i = 1; i < 65; i++) {
		fixed_pubkey[i-1] = pubkey[i];
	}
	uint8_t out[32];
	
	SHA3_256(out, fixed_pubkey, 64);
	
	for(int i = 0; i < 32; i++) {
		address[i] = out[12+i];
	}
}