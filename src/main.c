#include "url.h"
#include "utils.h"
#include "clientFTP.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv) {

	FTPInfo* ftp = malloc(sizeof(FTPInfo));

	if (validURL(argv[1], sizeof(argv[1]))) {
		fprintf(stderr, "Exiting Program\n");
		return -1;
	}
	else parseURL(argv[1], sizeof(argv[1]), ftp);
	
	fprintf(stderr, "\nHost: %s\nUser: %s\nPassword: %s\nPath: %s\n\n", ftp->host, ftp->user, ftp->password, ftp->path);

	if (getipAddress(ftp) != 0) {
		fprintf(stderr, "Exiting Program\n");
		return -1;
	}
	
	if (connectSocket(ftp, ftp->ip, SERVER_PORT, READ) != 0) {
		fprintf(stderr, "Exiting Program\n");
		return -1;
	}

	fprintf(stderr, "\nHost: %s\nUser: %s\nPassword: %s\nPath: %s\n\n", ftp->host, ftp->user, ftp->password, ftp->path);

	if (loginHost(ftp) != 0) {
		fprintf(stderr, "Exiting Program\n");
		return -1;
	}

	if (setPasv(ftp) != 0) {
		fprintf(stderr, "Exiting Program\n");
		return -1;
	}

	if (retrFile(ftp) != 0) {
		fprintf(stderr, "Exiting Program\n");
		return -1;
	}

	if (downloadFile(ftp) != 0) {
		fprintf(stderr, "Exiting Program\n");
		return -1;
	}

	if (closeSockets(ftp) != 0) {
		fprintf(stderr, "Exiting Program\n");
		return -1;
	}

	fprintf(stderr, "Program runned without errors.\n");

	free(ftp);
	
	return 0;
}