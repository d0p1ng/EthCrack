#include <curl.h>
#include "../cjson/cJSON.h"
#include "datatypes.h"
#include "../config.h"

#define CHECKBALANCE_URL "http://127.0.0.1:1122/"
#define BAD_BALANCE "0x0"

unsigned short check_balance(char* address, WORD console_state, HANDLE hConsole);