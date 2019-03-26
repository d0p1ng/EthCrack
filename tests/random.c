#include "../utils/generator.h"

#include <stdlib.h>

int main () {
	
	uint8_t *key = (uint8_t *)malloc(32);
	generate_privatekey(key);
	
	
	
	return 0;
}