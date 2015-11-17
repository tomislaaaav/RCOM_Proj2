#ifndef UTILS__H
#define UTILS__H

#define MAX_ARRAY_SIZE 999
#define MAX_STRING_SIZE 255

#define SERVER_PORT 21
#define SERVER_ADDR "tom.fe.up.pt"

#define MAX_IP_SIZE	15
#define FILE_FAIL 550
#define LOGIN_FAIL 530
#define FILE_SUCCESS 150
#define DATA_PACKET_SIZE 8192

#define DONT_READ 0
#define READ 1

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

} FTPInfo;

#endif