#ifndef UTILS__H
#define UTILS__H

#define MAX_ARRAY_SIZE 100
#define MAX_STRING_SIZE 255

#define SERVER_PORT 21
#define SERVER_ADDR "tom.fe.up.pt"

#define MAX_IP_SIZE	15
#define FILE_FAIL 550
#define LOGIN_FAIL 530
#define FILE_SUCCESS 150
#define DATA_PACKET_SIZE 100

#define DONT_READ 0
#define READ 1
#define SOCKETS_INIT 0
#define SOCKETS_NOT_INIT 1

#define START 0
#define RUN 1
#define SETTINGS 2
#define CLOSE 3

#define TRUE 0
#define FALSE 1

#define STANDARD_INPUT 0
#define STANDARD_OUTPUT 1
#define STANDARD_ERROR 2

typedef struct {
	
	char host[MAX_ARRAY_SIZE];
	char password[MAX_ARRAY_SIZE];
	char user[MAX_ARRAY_SIZE];
	char path[MAX_ARRAY_SIZE];
	char ftp_url[MAX_ARRAY_SIZE];

	char ip[MAX_IP_SIZE];

	int socket_comms_fd;
	int socket_data_fd;

	int fileSize;

	int pasv;

	int data_size_packet;

} FTPInfo;

#endif