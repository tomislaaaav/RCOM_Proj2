#include "clientFTP.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <netdb.h> 
#include <sys/types.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <libgen.h>

int getipAddress(FTPInfo * ftp)
{
	struct hostent *h;
    
    if ((h=gethostbyname(ftp->host)) == NULL) {  
        herror("gethostbyname");
       	return -1;
    }

    strcpy(ftp->ip, inet_ntoa(*((struct in_addr *)h->h_addr)));

    return 0;
}

int closeSockets(FTPInfo * ftp) {
	char answer[MAX_STRING_SIZE];

	int ret, bytesReadQuit;

	if ((ret = sendCommand(ftp, "quit\n", strlen("quit\n"))) != 0) {
		fprintf(stderr, "Error sendind quit command\n");
		return -1;
	}

	if ((bytesReadQuit = readAnswer(ftp, answer, MAX_STRING_SIZE)) == 0) {
		fprintf(stderr, "Error reading answer from quit\n");
		return -1;
	}
	fprintf(stderr, "%s\n", answer);

	close(ftp->socket_comms_fd);
	close(ftp->socket_data_fd);
	return 0;
}

int connectSocket(FTPInfo * ftp, char * ip, int port, int flag) {

	int	sockfd;
	struct	sockaddr_in server_addr;
	
	/*server address handling*/
	bzero((char*)&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ip);	/*32 bit Internet address network byte ordered*/
	server_addr.sin_port = htons(port);		/*server TCP port must be network byte ordered */
    
	/*open an TCP socket*/
	if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0) {
    		perror("socket()");
        	return -1;
    	}
	/*connect to the server*/
    	if(connect(sockfd, 
	           (struct sockaddr *)&server_addr, 
		   sizeof(server_addr)) < 0){
        	perror("connect()");
		return -1;
	}
	if (port == 21)
    	ftp->socket_comms_fd = sockfd;
    else ftp->socket_data_fd = sockfd;

    int ret;
    char answer[MAX_STRING_SIZE];

    if (flag == READ) {
	    if ((ret = readAnswer(ftp, answer, MAX_STRING_SIZE)) == 0) {
			fprintf(stderr, "Error reading answer from connection\n");
			return -1;
		}
	}
	
	return 0;
}

int loginHost(FTPInfo * ftp) {

	char command[4 + 1 + 1 + strlen(ftp->user)]; //o 6 vem dos espaços e do 'user' e do /n
	
	char answer[MAX_STRING_SIZE];

	int ret, bytesReadPass, bytesReadUser;

	sprintf(command,"user %s\n", ftp->user);
	if ((ret = sendCommand(ftp, command, strlen(command))) != 0) {
		fprintf(stderr, "Error sendind username command\n");
		return -1;
	}

	if ((bytesReadUser = readAnswer(ftp, answer, MAX_STRING_SIZE)) == 0) {
		fprintf(stderr, "Error reading answer from username\n");
		return -1;
	}
	fprintf(stderr, "%s\n", answer);


	sprintf(command,"pass %s\n", ftp->password);
	if ((ret = sendCommand(ftp, command, strlen(command))) != 0) {
		fprintf(stderr, "Error sendind password command\n");
		return -1;
	}

	if ((bytesReadPass = readAnswer(ftp, answer, MAX_STRING_SIZE)) == 0) {
		fprintf(stderr, "Error reading answer from password\n");
		return -1;
	}
	fprintf(stderr, "%s\n", answer);


	int reply;
	sscanf(answer, "%d", &reply);

	if (reply == LOGIN_FAIL) {
		fprintf(stderr, "Wrong combination.\n");
		return -1;
	}

	fprintf(stderr, "Successfully logged in ip: %s\n", ftp->ip);

	return 0;
}


int setPasv(FTPInfo * ftp) {
	
	char answer[MAX_STRING_SIZE];

	int ret, bytesReadPasv;

	int ip1, ip2, ip3, ip4, port1, port2;

	if ((ret = sendCommand(ftp, "pasv\n", strlen("pasv\n"))) != 0) {
		fprintf(stderr, "Error sendind pasv command\n");
		return -1;
	}

	if ((bytesReadPasv = readAnswer(ftp, answer, MAX_STRING_SIZE)) == 0) {
		fprintf(stderr, "Error reading answer from pasv\n");
		return -1;
	}

	fprintf(stderr, "%s\n", answer);

	if ((sscanf(answer, "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d)", &ip1, &ip2, &ip3, &ip4, &port1, &port2)) < 0){
		fprintf(stderr, "Error parsing the ports from passive mode.\n");
		return -1;
	}

	ftp->pasv = port1 * 256 + port2;

	char ip[MAX_IP_SIZE];

	// os primeiros 4 bytes serão o ip do segundo socket, que servirá para fazer download !
	sprintf(ip, "%d.%d.%d.%d", ip1, ip2, ip3, ip4);
	
	if (connectSocket(ftp, ip, ftp->pasv, DONT_READ) != 0) {
		fprintf(stderr, "Error connecting the data socket.\n");
		return -1;
	}
	
	return 0;
}

int retrFile(FTPInfo *ftp) {
	char answer[MAX_STRING_SIZE];

	char command[4 + 1 + 1 + strlen(ftp->path)];

	sprintf(command, "retr %s\n", ftp->path);

	fprintf(stderr, "Retrieving file: %s\n", ftp->path);

	int ret, bytesReadRetr;

	if ((ret = sendCommand(ftp, command, strlen(command))) != 0) {
		fprintf(stderr, "Error sendind retr command\n");
		return -1;
	}

	if ((bytesReadRetr = readAnswer(ftp, answer, MAX_STRING_SIZE)) == 0) {
		fprintf(stderr, "Error reading answer from retr\n");
		return -1;
	}
	fprintf(stderr, "%s\n", answer);

	int reply;

	sscanf(answer, "%d", &reply);

	if (reply == FILE_FAIL) {
		fprintf(stderr, "File failed opening.\n");
		return -1;
	}
	else if (reply == FILE_SUCCESS) {

		char * init = strrchr(answer, '(');

		char fill[MAX_ARRAY_SIZE];

		strcpy(fill, init);

		int size;

		sscanf(fill, "(%d bytes).", &size);
		ftp->fileSize = size;
		return 0;
	}
	else {
		fprintf(stderr, "Unexpected reply.\n");
		return -1;
	}
}

int downloadFile(FTPInfo *ftp) {

	char data_packet[ftp->data_size_packet];
	int bytesRead;
	int bytesWritten = 0;

	char * fileName = basename(ftp->path);

	FILE * fileW = fopen(fileName, "w");

	while ((bytesRead = read(ftp->socket_data_fd, data_packet, ftp->data_size_packet)) != 0) {
		if (bytesRead < 0) {
			fprintf(stderr, "Error reading file.\n");
			return -1;
		}
		bytesWritten+=bytesRead;
		fprintf(stderr, "Downloading File: %.2f%%\n", (double)(bytesWritten*100)/ftp->fileSize);
		fwrite(data_packet, sizeof(char), bytesRead, fileW);
			if (ferror(fileW) != 0) {
				fprintf(stderr, "Error writing into file.\n");
				return -1;
			}
	}
	
	return 0;
}

int readAnswer(FTPInfo* ftp, char* answer, int size){	
	int bytesRead;
	memset(answer, 0, size);
	bytesRead = read(ftp->socket_comms_fd, answer, size);
	return bytesRead;
}

int sendCommand(FTPInfo* ftp, char* command, int size){

	int bytesSent = write(ftp->socket_comms_fd, command, size);

	if(bytesSent <= 0){
		fprintf(stderr, "No information sent to server.\n");
		return -1;
	}

	if(bytesSent != size){
		fprintf(stderr, "Information only partly sent to server\n");
		return -1;
	}

	return 0;
}