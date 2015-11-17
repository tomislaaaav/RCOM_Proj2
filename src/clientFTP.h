#ifndef CLIENT_FTP__H
#define CLIENT_FTP__H

#include "utils.h"

int getipAddress(FTPInfo * ftp);

int connectSocket(FTPInfo * ftp, char * ip, int port, int flag);

int closeSockets(FTPInfo * ftp);

int loginHost(FTPInfo * ftp);

int setPasv(FTPInfo * ftp);

int sendCommand(FTPInfo* ftp, char* command, int size);
int readAnswer(FTPInfo* ftp, char* answer, int size);

int retrFile(FTPInfo* ftp);
int downloadFile(FTPInfo *ftp);

void setEndereco(char * end);
char * getEndereco();

#endif