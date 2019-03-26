#include "checker.h"

void init_string(struct string *s) {
  s->len = 0;
  s->ptr = malloc(s->len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "malloc() failed\n");
    exit(EXIT_FAILURE);
  }
  s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
  size_t new_len = s->len + size*nmemb;
  s->ptr = realloc(s->ptr, new_len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "realloc() failed\n");
    exit(EXIT_FAILURE);
  }
  memcpy(s->ptr+s->len, ptr, size*nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;

  return size*nmemb;
}

unsigned short check_balance(char* address, WORD console_state, HANDLE hConsole) {
	
	CURL *curl = curl_easy_init();
	struct curl_slist *headers = NULL;
	
	headers = curl_slist_append(headers, "content-type: application/json;");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_URL, CHECKBALANCE_URL);
	
	
	cJSON *payload = cJSON_CreateObject();
	cJSON *version = cJSON_CreateString("2.0");
	cJSON *request_id = cJSON_CreateNumber(1);
	cJSON *method = cJSON_CreateString("eth_getBalance");
	
	char fixed_address[42];
	memset(fixed_address, 0, 42);
	fixed_address[0] = '0';
	fixed_address[1] = 'x';
	strcat(fixed_address, address);
	
	cJSON *addr = cJSON_CreateString(fixed_address);
	cJSON *block = cJSON_CreateString("latest");
	
	cJSON *params = cJSON_CreateArray();
	
	cJSON_AddItemToObject(payload, "jsonrpc", version);
	cJSON_AddItemToObject(payload, "method", method);
	cJSON_AddItemToObject(payload, "id", request_id);
	
	cJSON_AddItemToArray(params, addr);
	cJSON_AddItemToArray(params, block);
	
	cJSON_AddItemToObject(payload, "params", params);
	
	char* stringed = cJSON_PrintUnformatted(payload);

	
	struct string s;
    init_string(&s);
	
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(stringed));
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, stringed);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
	
	CURLcode code = curl_easy_perform(curl);
	
	curl_easy_cleanup(curl);
	
	cJSON *responseText = cJSON_Parse(s.ptr);
	cJSON *result = cJSON_GetObjectItemCaseSensitive(responseText, "result");
	
	#ifdef VERBOSE
		printf("%s\n", s.ptr);
	#endif 
	
	if(!cJSON_IsString(result)) {

		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
	    printf("\n[!] ");
	    SetConsoleTextAttribute(hConsole, console_state);

		printf("Connection error. Check if you are running RPC. Unable to connect to %s\n", 
			CHECKBALANCE_URL);
		return FALSE;
	}
	
	char *balance = result->valuestring;

	int val = strcmp(balance, BAD_BALANCE);
	
	free(s.ptr);
	cJSON_Delete(responseText);

	return val != 0;
}