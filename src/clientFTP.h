#ifndef CLIENT_FTP__H
#define CLIENT_FTP__H

#include "utils.h"

int getipAddress(FTPInfo * ftp);

int connectSocket(FTPInfo * ftp);

int loginHost(FTPInfo * ftp);

int setPasv(FTPInfo * ftp);

int sendCommand(FTPInfo* ftp, char* command, int size);
int readAnswer(FTPInfo* ftp, char* answer, int size);

void setEndereco(char * end);
char * getEndereco();

#endif