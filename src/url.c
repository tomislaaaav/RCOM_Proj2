// pegar no FTP e debugar em parametros
#include "url.h"
#include <string.h>
#include <stdio.h>
#include <regex.h>

int validURL(char * url, int size) {

	regex_t regExpression;
	int retReg = regcomp(&regExpression, "ftp://+[a-zA-Z0-9]+:[a-zA-Z0-9]+@+[a-zA-Z0-9._~:/?#@!$&'()*+,;=]+/[a-zA-Z0-9._~:/?#@!$&'()*+,;=]+", REG_EXTENDED);

	if (retReg) {
		fprintf(stderr, "Can't compile RegExpression\n");
		return -1;
	}

	if (!(retReg = regexec(&regExpression, url, 0, NULL, 0))) {
		return 0;
	}
	else if (retReg == REG_NOMATCH) {
		fprintf(stderr, "URL is not valid\n");
		return -1;
	}
	else {
		fprintf(stderr, "Error validating URL\n");
		return -1;
	}
}

int parseURL(char* url, int size, FTPInfo * ftp) {

	char * temp_url = url + 6;

	char * temp_path = strchr(temp_url, ':');
	int i = 0;

	while(temp_url != temp_path){
		ftp->user[i] = *temp_url;
		i++;
		temp_url++;
	} 
	temp_url++;
	ftp->user[i] = '\0';

	temp_path = strchr(temp_url, '@');
	i = 0;

	while(temp_url != temp_path) {
		ftp->password[i] = *temp_url;
		i++;
		temp_url++;
	}
	temp_url++;
	ftp->password[i] = '\0';

	temp_path = strchr(temp_url, '/');
	i = 0;

	while(temp_url != temp_path) {
		ftp->host[i] = *temp_url;
		i++;
		temp_url++;
	}
	temp_url++;
	ftp->host[i] = '\0';

	strcpy(ftp->path, temp_url);

	return 0;
}