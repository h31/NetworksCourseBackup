// WinClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

/* 
 * File:   main.c
 * Author: student
 *
 * Created on 27 ќкт€брь 2014 г., 13:42
 */

#include <stdio.h>
#include <sys/types.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string.h>
#include <stdlib.h>
#include <ws2def.h>
#include <ctype.h>
#include "io.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)  
#define bcopy(b1,b2,len) (memmove((b2), (b1), (len)), (void) 0)

int main(int argc, _TCHAR* argv[])  //char *argv[]
{
	//int portno, n;
	int n;
	int portno = 5001;
	SOCKET sockfd;
	WSADATA wsaData;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	char buffer[256];

	/*if (argc < 3) {
		fprintf(stderr,"usage %s hostname port\n", argv[0]);
		exit(0);
	}*/
	
	
	//portno = atoi(argv[2]);
	/* Create a socket point */
	// Initialize Winsock
	
    n = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (n != 0) {
        printf("WSAStartup failed with error: %d\n", n);
        return 1;
    }
	/* Create a socket point */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET)
    {
        perror("ERROR opening socket");
        exit(1);
    }

	/*sockfd = socket(AF_INET, SOCK_STREAM, 0);
	/*if (sockfd < 0)
	{
		perror("ERROR opening socket");
		exit(1);
	}*/
	server = gethostbyname("192.168.56.101");
	if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
	//server = gethostbyname(argv[1]);
	/*if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}*/
	bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
           (char *)&serv_addr.sin_addr.s_addr,
                server->h_length);
    serv_addr.sin_port = htons(portno);

	if (connect(sockfd,(sockaddr*) &serv_addr,sizeof(serv_addr)) <0)
	{
		perror("ERROR connecting");
		exit(1);
	}
	while (1) {
		printf("Input Expression:\n");
		bzero(buffer, 256);
		fgets(buffer, 255, stdin);
		if (strcmp(buffer, "q\n") == 0) {
			printf("Exit\n");
			closesocket(sockfd);
			break;
		}
		n = send(sockfd, buffer, strlen(buffer),0);
		if (n < 0) {
			perror("ERROR writing to socket");
			exit(1);
		}
		bzero(buffer, 256);
		n = recv(sockfd, buffer, 255,0);
		if (n < 0) {
			perror("ERROR reading from socket");
			exit(1);
		}
		printf("%s\n", buffer);
	}
	return 0;
}
