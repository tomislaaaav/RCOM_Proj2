#include "app.h"
#include "clientFTP.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int settings = START;

int runApp(FTPInfo *ftp) {
	if (connectSocket(ftp, ftp->ip, SERVER_PORT, READ) != 0) {
		fprintf(stderr, "Exiting Application\n");
		return -1;
	}

	fprintf(stderr, "\nHost: %s\nUser: %s\nPassword: %s\nPath: %s\n\n", ftp->host, ftp->user, ftp->password, ftp->path);

	if (loginHost(ftp) != 0) {
		fprintf(stderr, "Exiting Application\n");
		return -1;
	}

	if (setPasv(ftp) != 0) {
		fprintf(stderr, "Exiting Application\n");
		return -1;
	}

	if (retrFile(ftp) != 0) {
		fprintf(stderr, "Exiting Application\n");
		return -1;
	}

	if (downloadFile(ftp) != 0) {
		fprintf(stderr, "Exiting Application\n");
		return -1;
	}

	return 0;
}

int runSettings(FTPInfo *ftp) {

	settings = START;
	//change size of data packets
	fprintf(stderr, "Only setting available to change is the size of the data packets. Would you like to change it? (Y/N)? ");

	char data[MAX_ARRAY_SIZE];

	int bytesPacket;

	while(settings == START) {
		
		if (read(STANDARD_INPUT, data, MAX_ARRAY_SIZE) < 0) {
			fprintf(stderr, "Error reading user input.\n\n");
			return -1;
		}

		switch(data[0]) {
			case 'Y':
			fprintf(stderr, "State the number of bytes of the data packet (between 1 and 9000): ");

			if (read(STANDARD_INPUT, data, MAX_ARRAY_SIZE) < 0) {
				fprintf(stderr, "Error reading user input.\n\n");
				return -1;
			}
			
			if (sscanf(data, "%d", &bytesPacket) == EOF) {
	 			fprintf(stderr, "Please, write a number.\n");
	 			break;
	 		}

	 		if (bytesPacket < 0 || bytesPacket > 9000) {
	 			fprintf(stderr, "Please write a number between 1 and 9000.\n");
	 			break;
	 		}

	 		ftp->data_size_packet = bytesPacket;
			settings = CLOSE;
			break;
			case 'N':
			settings = CLOSE;
			break;
			default:
			fprintf(stderr, "Please, write only Y or N.\n");
			break;
		}

	}
	
	return 0;
}

int closeFTP(FTPInfo *ftp, int app_init) {

	if (app_init == SOCKETS_INIT) {
		if (closeSockets(ftp) != 0) {
			fprintf(stderr, "Error closing Application\n");
			return -1;
		}
	}

	free(ftp);

	return 0;
}