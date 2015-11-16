#ifndef CLIENT_FTP__H
#define CLIENT_FTP__H

#include "utils.h"

int getipAddress(char * host);
int connectSocket(FTPInfo * ftp);

void setEndereco(char * end);
char * getEndereco();

#endif