//#include <stdio.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>

#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <time.h>

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string.h>
#include <assert.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)
#define bcopy(b1,b2,len) (memmove((b2), (b1), (len)), (void) 0)

void print_mesgs(char* buffer, int sockfd, char* user);

void write_socket_start(char* buffer, int sockfd){
	int n;
	bzero(buffer, 256);
	fgets(buffer, 255, stdin);
	//puts(buffer);
	/* Send message to the server */
	n = write(sockfd, buffer, strlen(buffer));
	if (n < 0)
	{
		perror("ERROR writing to socket");
		exit(1);
	}
}

void read_response(char* buffer, int sockfd){
	int n;
	bzero(buffer, 256);
	n = read(sockfd, buffer, 255);
	if (n < 0)
	{
		perror("ERROR reading from socket");
		exit(1);
	}
	printf("%s", buffer);
}
//------------------------------------------
void get_args(char* inp_str, char* arg[]){//buffer and array to write
	int i = 0, j;
	const char s[2] = "#";
	char *token;
	token = strtok(inp_str, s);
	arg[i] = token;
	while (token != NULL){
		i++;
		token = strtok(NULL, s);
		arg[i] = token;
	}
	i--;
	arg[i] = NULL;
}

char *get_mes_from_client(int newsockfd, char *buffer){
	int n;
	bzero(buffer, 256);
	n = read(newsockfd, buffer, 255);
	if (n < 0)
	{
		perror("ERROR reading from socket");
		exit(1);
	}
	return buffer;
}

void resp_send(char* from, char* to, char* mes){
}

void resp_read(int newsockfd, char *buffer, char* client){
	print_mesgs(buffer, newsockfd, client);
}

void resp_exit(){
	exit(1);
}

void print_mesgs(char* buffer, int sockfd, char* user){
	read_response(buffer, sockfd);
	read_response(buffer, sockfd);
}

char* put_hello(int sockfd, char *buffer){
	write_socket_start(buffer, sockfd);
	char* arg[4];
	get_args(buffer, arg);
	int i, j = 0;
	char user[10] = "no user";
	if ((arg[0] != NULL) && (arg[1] != NULL)){
		strcpy(user, arg[0]);
		char command[7];
		strcpy(command, arg[1]);
		j = atoi(command);
		if (j == 1){
			resp_send(user, arg[2], arg[3]);
		}
		else if (j == 2){
			resp_read(sockfd, buffer, user);
		}
		else if (j == 3){
			resp_exit();
		}
		else {
			puts("Wrong command!\n");
		}
	}
	return user;
}

void keep_talking(int sockfd, char *buffer){
	write_socket_start(buffer, sockfd);
	char* arg[4];
	get_args(buffer, arg);
	int i, j = 0;
	char user[10] = "no user";
	if ((arg[0] != NULL)){
		strcpy(user, arg[0]);
		char command[7];
		strcpy(command, arg[0]);
		j = atoi(command);
		if (j == 1){
			resp_send(user, arg[2], arg[3]);
		}
		else if (j == 2){
			resp_read(sockfd, buffer, user);
		}
		else if (j == 3){
			resp_exit();
		}
		else {
			puts("Wrong command!\n");
		}
	}
}

int main(int argc, char *argv[])
{
	SOCKET sockfd;
	int portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	char buffer[256];

	WSADATA wsaData;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (argc < 3) {
		fprintf(stderr, "usage %s hostname port\n", argv[0]);
		exit(0);
	}
	portno = atoi(argv[2]);
	/* Create a socket point */
	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd == INVALID_SOCKET)
	{
		perror("ERROR opening socket");
		exit(1);
	}
	server = gethostbyname(argv[1]);
	if (server == NULL) {
		fprintf(stderr, "ERROR, no such host\n");
		exit(0);
	}

	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;

	bcopy((char *)server->h_addr,
	(char *)&serv_addr.sin_addr.s_addr,
	server->h_length);

	serv_addr.sin_port = htons(portno);

	/* Now connect to the server */
	//if (connect(sockfd, (sockaddr *)&serv_addr, sizeof(serv_addr))>0)

	if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) > 0)
	{
		perror("ERROR connecting");
		exit(1);
	}
	/* Now ask for a message from the user, this message
	* will be read by server
	*/
	read_response(buffer, sockfd);
	put_hello(sockfd, buffer);
	read_response(buffer, sockfd);
	while (1)
		keep_talking(sockfd, buffer);
	return 0;
}
