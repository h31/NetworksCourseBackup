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

#define BUFSIZE 512
#define SERVER	"127.0.0.1"
#define	PORT	5001

int main(int argc, char *argv[])
{
	SOCKET sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[BUFSIZE],
		message[BUFSIZE];
	WSADATA wsaData;

	WSAStartup(MAKEWORD(2,2),&wsaData);
	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd == INVALID_SOCKET)
    {
        perror("ERROR opening socket");
        exit(1);
    }
	server = gethostbyname(SERVER);
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,
	(char *)&serv_addr.sin_addr.s_addr,
	server->h_length);
	serv_addr.sin_port = htons(PORT);
    if (connect(sockfd,(sockaddr *)&serv_addr,sizeof(serv_addr)) > 0)
    {
         perror("ERROR connecting");
         exit(1);
    }
	while(1)
	{
		bzero(buffer,BUFSIZE);
		recv(sockfd,buffer,BUFSIZE-1,0);	
		printf("%s\n",buffer);
		bzero(message,BUFSIZE);
		printf("Print login\n");
		fgets(message,BUFSIZE-1,stdin);
		send(sockfd,message,strlen(message),0);
		bzero(buffer,BUFSIZE);
		recv(sockfd,buffer,BUFSIZE-1,0);
		bzero(message,BUFSIZE);
		printf("Print password\n");
		fgets(message,BUFSIZE-1,stdin);
		send(sockfd,message,strlen(message),0);
		bzero(buffer,BUFSIZE);
		recv(sockfd,buffer,BUFSIZE-1,0);
		if((!strncmp(buffer,"next",4)))
		{
    		bzero(message,BUFSIZE);
    		strncpy_s(message,"OK",2);
			send(sockfd,buffer,strlen(buffer),0);
    		break;
		}
	}
    while(1)
    {
    	bzero(buffer,BUFSIZE);
		recv(sockfd,buffer,BUFSIZE-1,0);
    	if(strncmp(buffer,"OK",strlen("OK")))
    	{
    		if(strncmp(buffer,"next",strlen("next")))
    			printf("%s\n",buffer);
    	}

    	if(!(strncmp(buffer,"next",strlen("next"))))
    	{
    		printf("Writing\n");
    		bzero(message,BUFSIZE);
    		fgets(message,BUFSIZE-1,stdin);
			send(sockfd,message,strlen(message),0);
			recv(sockfd,buffer,BUFSIZE-1,0);
			send(sockfd,"next",strlen("next"),0);
    	}
    	else
    	{
    		bzero(message,BUFSIZE);
    		strncpy_s(message,"OK",strlen("OK"));
			send(sockfd,message,strlen(message),0);
    	}
    }
}
