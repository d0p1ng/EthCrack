#include "generator.h"

uint8_t random_byte() {

	

	/*uint8_t byte;
	
	FILE *urandom = fopen("/dev/urandom", "rb");
	fread(&byte, 1, 1, urandom);
	fclose(urandom);
	
	return byte;*/
}

void generate_privatekey(uint8_t *key) {
	memset(key, 0, KEYLEN);
	
	/*for(int i = 0; i < KEYLEN; i++) {
		key[i] = random_byte();
	}*/

	HCRYPTPROV hCryptProv;
	BYTE pKey[32];

	if(CryptAcquireContextA(
		&hCryptProv,
		NULL,
		NULL,
		PROV_RSA_FULL,
		CRYPT_VERIFYCONTEXT)) {


		if(CryptGenRandom(
			hCryptProv,
			32,
			pKey)) {

			memcpy(key, pKey, 32);
		}

		CryptReleaseContext(hCryptProv, 0);

	}
}