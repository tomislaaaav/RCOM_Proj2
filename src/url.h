#ifndef URL__H
#define URL__H

#include "utils.h"

int validURL(char * url, int size);
int parseURL(char* url, int size, FTPInfo * ftp);

#endif