// connect ao FTP, chamando connect socket

// connect socket:

	// abrir socket para connect ao FTP
	// login no FTP
	// esperar resposta (replyCode), checkar e atuar
	// se der erro, terminar socket, terminar programa
	
	// entrar em modo passivo, calcular os últimos 2 bytes (penultimo * 256 + ultimo)
	// os primeiros 4 bytes serão o ip do segundo socket, que servirá para fazer download
	// abrir socket e guardar
	// pedir ficheiro ao FTP atraves do primeiro socket
	// usar o segundo socket e ler o ficheiro atraves de packets (ler, escrever, ler, escrever)
	// fechar os 2 sockets (close do fd). antes, mandar comando quit para primeiro
	// terminar programa

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

char * endereco;

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

int connectSocket(FTPInfo * ftp) {

	int	sockfd;
	struct	sockaddr_in server_addr;
	
	/*server address handling*/
	bzero((char*)&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ftp->ip);	/*32 bit Internet address network byte ordered*/
	server_addr.sin_port = htons(SERVER_PORT);		/*server TCP port must be network byte ordered */
    
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
    ftp->socket_comms_fd = sockfd;
	
	close(sockfd);
	return 0;
}

int loginHost(FTPInfo * ftp) {

	char command[4 + 1 + 1 + strlen(ftp->user)]; //o 6 vem dos espaços e do 'user' e do /n
	
	char answer[MAX_ARRAY_SIZE];

	int ret, bytesReadPass, bytesReadUser;

	sprintf(command,"user %s\n", ftp->user);
	if ((ret = sendCommand(ftp, command, strlen(command))) != 0) {
		fprintf(stderr, "Error sendind username command\n");
		return -1;
	}

	if ((bytesReadUser = readAnswer(ftp, answer, MAX_ARRAY_SIZE))) {
		fprintf(stderr, "Error reading answer from username\n");
		return -1;
	}

	sprintf(command,"pass %s\n", ftp->password);
	if ((ret = sendCommand(ftp, command, strlen(command))) != 0) {
		fprintf(stderr, "Error sendind password command\n");
		return -1;
	}

	if ((bytesReadPass = readAnswer(ftp, answer, MAX_ARRAY_SIZE))) {
		fprintf(stderr, "Error reading answer from password\n");
		return -1;
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

void setEndereco(char * end) {
	endereco = end;
}

char * getEndereco() {
	return endereco;
}