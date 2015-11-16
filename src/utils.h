#ifndef UTILS__H
#define UTILS__H

#define MAX_ARRAY_SIZE 999

#define SERVER_PORT 21
#define SERVER_ADDR "tom.fe.up.pt"

typedef struct {
	
	char host[MAX_ARRAY_SIZE];
	char password[MAX_ARRAY_SIZE];
	char user[MAX_ARRAY_SIZE];
	char path[MAX_ARRAY_SIZE];
	char ftp_url[MAX_ARRAY_SIZE];

	char ip[MAX_ARRAY_SIZE];

	int socket_comms_fd;
	int socket_data_fd;

	int fileSize;

} FTPInfo;

#endif