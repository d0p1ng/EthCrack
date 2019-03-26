#include "ethcrack.h"

CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
WORD saved_attributes;

void random_worker() {
	while(TRUE) {

		uint8_t priv[32];
		memset(priv, 0, 32);

		generate_privatekey(priv);

		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		//memcpy(priv, pk, 32);

		uint8_t ethaddress[20];
		ethaddress_calculate(priv, ethaddress);
		
		char hex[40];
		memset(hex, 0, 40);
		
		for(int i = 0; i < 20; i++) {
			char tmp[2];
			sprintf(tmp, "%02x", ethaddress[i]);
			strcat(hex, tmp);
		}

		if(strcmp(hex, "3f17f1962b36e491b30a40b2405849e597ba5fb5") == 0) {
			return;
		}
		
		#ifdef VERBOSE

			SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		    printf("\n[*] ");
		    SetConsoleTextAttribute(hConsole, saved_attributes);

			printf("Trying ");
			
			for(int i = 0; i < 32; i++) {
				printf("%02x", priv[i]);
			}
			
			printf(":0x%s\n", hex);
		#endif
		
		if(check_balance(hex, saved_attributes, hConsole) == TRUE) {

			printf("\n\n!!!! KEY WITH BALANCE FOUND !!!!\n\n");
			
			printf("\nPrivate Key: ");
			for(int i = 0; i < 32; i++) {
				printf("%02x", priv[i]);
			}
			printf("\n");
			
			printf("\nEth address: 0x");
			printf("%s\n", hex);
			
			exit(0);
		}

		//system("cls");
	}
	
}

int main (int argc, char** argv) {
	
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;

    printf(" #######               #####                              \n");
    printf(" #       ##### #    # #     # #####    ##    ####  #    # \n");
    printf(" #         #   #    # #       #    #  #  #  #    # #   #  \n");
    printf(" #####     #   ###### #       #    # #    # #      ####   \n");
    printf(" #         #   #    # #       #####  ###### #      #  #   \n");
    printf(" #         #   #    # #     # #   #  #    # #    # #   #  \n");
    printf(" #######   #   #    #  #####  #    # #    #  ####  #    # \n");
    printf("\n");

    printf("\t[ Developed by nrize ]\n");
    printf("\t[ Version %s ]", VERSION);

    if(argc < 2) {
    	printf("\n\nUsage: %s <threads>\n", argv[0]);
    	return 1;
    }

    int thread_count = atoi(argv[1]);

    if(thread_count == 0) {
    	printf("\n\nInvalid thread count!\n\n");
    	return 2;
    }

    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
    printf("\n[*] ");
    SetConsoleTextAttribute(hConsole, saved_attributes);

	printf("Wallet mining will be started in %i seconds ...\n", SLEEP_TIME);
	sleep(SLEEP_TIME);
	
	#ifdef ENDLESS

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
	    printf("\n[*] ");
	    SetConsoleTextAttribute(hConsole, saved_attributes);

		printf("Initializing %i threads ...\n", thread_count);
		pthread_t threads[thread_count];
		
		for(int i = 0; i < thread_count; i++) {

			int ret = pthread_create( &threads[i], NULL, random_worker, NULL);
			
			if(ret) {
				printf("pthread_create() error: %i\n", ret);
				return 1;
			}
		}
	#else
	//	worker();
	#endif
	
	getchar();
	return 0;
}