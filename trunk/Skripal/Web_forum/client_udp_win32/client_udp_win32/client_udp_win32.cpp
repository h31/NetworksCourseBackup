#define _CRT_SECURE_NO_DEPRECATE
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

#define SERVER "127.0.0.1"
//#define SERVER "192.168.0.104"
#define BUFLEN 256  //Max length of buffer
#define PORT 8888  //The port on which to send data

void die(char *s)
{
    perror(s);
    exit(1);
}


int main(void)
{
    struct sockaddr_in serv_addr;
    int s, slen=sizeof(serv_addr);
    char buffer[BUFLEN];
    char message[BUFLEN];
    WSADATA wsa;
	int n;
	int N=256;
    //Initialise winsock
    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    printf("Initialised.\n");
     
    //create socket
    if ( (s=socket(AF_INET, SOCK_DGRAM, 0)) == SOCKET_ERROR)
    {
        printf("socket() failed with error code : %d" , WSAGetLastError());
        exit(EXIT_FAILURE);
    }
     
    //setup address structure
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.S_un.S_addr = inet_addr(SERVER);

	sendto(s, "Start", strlen("Start") , 0 , (struct sockaddr *) &serv_addr, slen);
	while(1)
	{
		bzero(buffer,BUFLEN);
		recvfrom(s, buffer, BUFLEN, 0, (struct sockaddr *) &serv_addr, &slen);
		printf("%s\n",buffer);
		bzero(message,BUFLEN);
		printf("Print login\n");
		fgets(message,BUFLEN-1,stdin);
		sendto(s,message, strlen(message),0,(struct sockaddr *) &serv_addr,slen);
		printf("send mes %s\n",message);
		bzero(buffer,BUFLEN);
		recvfrom(s, buffer, BUFLEN, 0, (struct sockaddr *) &serv_addr, &slen);
		bzero(message,BUFLEN);
		printf("Print password\n");
		fgets(message,BUFLEN-1,stdin);
		sendto(s,message, strlen(message),0,(struct sockaddr *) &serv_addr,slen);
		bzero(buffer,BUFLEN);
		recvfrom(s,buffer,BUFLEN,0,(struct sockaddr *) &serv_addr,&slen);
		if(!strncmp(buffer,"next",strlen("next")))
		{
    		bzero(message,BUFLEN);
    		strncpy(message,"OK",strlen("OK"));
			sendto(s,message, strlen(message),0,(struct sockaddr *) &serv_addr,slen);
    		break;
		}
	}
    while(1)
    {
    	bzero(buffer,BUFLEN);
		recvfrom(s,buffer,BUFLEN,0,(struct sockaddr *) &serv_addr,&slen);
    	if((strncmp(buffer,"OK",strlen("OK"))))
    	{
    		if(strncmp(buffer,"next",strlen("next")))
    			printf("%s\n",buffer);
    	}
    	if(!(strncmp(buffer,"next",strlen("next"))))
    	{
    		printf("Writing\n");
    		bzero(message,BUFLEN);
    		fgets(message,BUFLEN-1,stdin);
			sendto(s,message, strlen(message),0,(struct sockaddr *) &serv_addr,slen);
    	}
    	else
    	{
    		bzero(message,BUFLEN);
    		strncpy(message,"OK",strlen("OK"));
			sendto(s,message, strlen(message),0,(struct sockaddr *) &serv_addr,slen);
    	}
    }
}

