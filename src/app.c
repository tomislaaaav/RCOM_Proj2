#include "app.h"
#include "clientFTP.h"

#include <stdio.h>

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

int closeFTP(FTPInfo *ftp) {

	if (closeSockets(ftp) != 0) {
		fprintf(stderr, "Error closing Application\n");
		return -1;
	}

	free(ftp);

	return 0;
}