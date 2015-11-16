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

int getipAddress(char * host)
{
	struct hostent *h;
    
    if ((h=gethostbyname(host)) == NULL) {  
        herror("gethostbyname");
       	return -1;
    }

    endereco = inet_ntoa(*((struct in_addr *)h->h_addr));

    return 0;
}

// Client TCP
int connectSocket(FTPInfo * ftp) {

	int	sockfd;
	struct	sockaddr_in server_addr;
	
	/*server address handling*/
	bzero((char*)&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(endereco);	/*32 bit Internet address network byte ordered*/
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

void setEndereco(char * end) {
	endereco = end;
}

char * getEndereco() {
	return endereco;
}