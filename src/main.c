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

	getipAddress(SERVER_ADDR);
	connectSocket(ftp);

	fprintf(stderr, "\nHost: %s\nUser: %s\nPassword: %s\nPath: %s\n", ftp->host, ftp->user, ftp->password, ftp->path);
	
	return 0;
}