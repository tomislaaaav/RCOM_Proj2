#include "url.h"
#include "utils.h"
#include "clientFTP.h"
#include "app.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int opt = START;

int menu = TRUE;

int main (int argc, char **argv) {

	FTPInfo* ftp = malloc(sizeof(FTPInfo));

	ftp->data_size_packet = DATA_PACKET_SIZE;

	int app_init; // ve se a app foi inicializada

	/* validar tudo e parsar */
	if (validURL(argv[1], sizeof(argv[1]))) {
		fprintf(stderr, "Exiting Program\n");
		return -1;
	}
	else parseURL(argv[1], sizeof(argv[1]), ftp);

	if (getipAddress(ftp) != 0) {
		fprintf(stderr, "Exiting Program\n");
		return -1;
	}

	while (menu == 0) {

		app_init = SOCKETS_NOT_INIT;
		system("clear");

		fprintf(stderr, "FTP Client\n\n");
		fprintf(stderr, "1 - Run application.\n");
		fprintf(stderr, "2 - Change Settings.\n");
		fprintf(stderr, "3 - Close application.\n");
		fprintf(stderr, "Your option: ");

		char data[MAX_ARRAY_SIZE];

 		if (read(STANDARD_INPUT, data, MAX_ARRAY_SIZE) < 0) {
 			fprintf(stderr, "Error reading user input. Exiting Client.\n\n");
 			return -1;
 		}

 		if (sscanf(data, "%d", &opt) == EOF) {
 			opt = START;
 		}

		switch(opt) {
			case RUN:
			if (runApp(ftp) != 0) {
				fprintf(stderr, "Error running application.\n");
				return -1;
			}

			fprintf(stderr, "Press enter to advance.\n");
			read(STANDARD_INPUT, data, MAX_ARRAY_SIZE);
			
			app_init = SOCKETS_INIT;
			break;
			case SETTINGS:
			if (runSettings(ftp)) {
				fprintf(stderr, "Error running settings.\n");
				return -1;
			}
			
			fprintf(stderr, "Press enter to advance.\n");
			read(STANDARD_INPUT, data, MAX_ARRAY_SIZE);
			
			break;
			case CLOSE:
			menu = FALSE;
			break;
			default:
			fprintf(stderr, "Choose a valid option (between 1 and 3)\n\n");
			sleep(1);
			break;
		}
	}

	if (closeFTP(ftp, app_init) != 0) {
		fprintf(stderr, "Error closing FTP Client\n");
		return -1;
	}

	fprintf(stderr, "Program runned without errors.\n");
	
	return 0;
}