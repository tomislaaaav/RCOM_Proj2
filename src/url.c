// pegar no FTP e debugar em parametros
#include "url.h"
#include "utils.h"
#include <string.h>
#include <stdio.h>
#include <regex.h>

int validURL(char * url, int size) {

	regex_t regExpression;
	int retReg;

	if (retReg = regcomp(&regExpression, "ftp://[[a-zA-Z0-9]+:[a-zA-Z0-9]+@][a-zA-Z0-9._~:/?#@!$&'()*+,;=]+/[a-zA-Z0-9._~:/?#@!$&'()*+,;=]+", REG_EXTENDED)) {
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

int parseURL(char* url, int size, char* host, char* user, char* password, char* path, char * ftp_url) {

	char * temp_url = url;

	char * temp_path = strchr(temp_url, '[');

	int i = 0;
	while(temp_url != temp_path){
		ftp_url[i] = *temp_url;
		i++;
		temp_url++;
	}
	
	temp_url++;
	ftp_url[i] = '\0';

	temp_path = strchr(temp_url, ':');
	i = 0;

	while(temp_url != temp_path){
		user[i] = *temp_url;
		i++;
		temp_url++;
	} 
	temp_url++;
	user[i] = '\0';

	temp_path = strchr(temp_url, '@');
	i = 0;

	while(temp_url != temp_path) {
		password[i] = *temp_url;
		i++;
		temp_url++;
	}
	temp_url+=2;
	password[i] = '\0';

	temp_path = strchr(temp_url, '/');
	i = 0;

	while(temp_url != temp_path) {
		host[i] = *temp_url;
		i++;
		temp_url++;
	}
	temp_url++;
	host[i] = '\0';

	strcpy(path, temp_url);

	fprintf(stderr, "\nHost: %s\nUser: %s\nPassword: %s\nPath: %s\n", host, user, password, path);

	return 0;
}