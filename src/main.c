#include "url.h"
#include "utils.h"
#include "clientFTP.h"
#include <stdio.h>
#include <stdlib.h>

int opt = START;

int menu = TRUE;

int main (int argc, char **argv) {

	FTPInfo* ftp = malloc(sizeof(FTPInfo));

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

	while (menu) {

		system("clear");

		printf("FTP Client\n\n");
		printf("1 - Run application.\n");
		printf("2 - Change Settings.\n");
		printf("3 - Close application.\n");
		printf("Your option: %d", opt);
 		// fazer read
		switch(opt) {
			case RUN:
			if (runApp(ftp) != 0) {
				fprintf(stderr, "Error running application.\n");
				return -1;
			}
			break;
			case SETTINGS:
			if (runSettings(ftp)) {
				fprintf(stderr, "Error running settings.\n");
				return -1;
			}
			break;
			case CLOSE:
			menu = FALSE;
			break;
			default:
			fprintf(stderr, "Choose a valid option (between 1 and 3)\n\n");
			break;
		}
	}

	if (closeFTP(ftp) != 0) {
		fprintf(stderr, "Error closing FTP Client\n");
		return -1;
	}

	fprintf(stderr, "Program runned without errors.\n");
	
	return 0;
}