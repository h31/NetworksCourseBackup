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
#define BUFLEN 512  //Max length of buffer
#define PORT 5001  //The port on which to send data

void die(char *s)
{
    perror(s);
    exit(1);
}

int main(void)
{
    struct sockaddr_in si_other;
    int s, slen=sizeof(si_other);
    char buf[BUFLEN];
    char message[BUFLEN];
	WSADATA wsaData;
	struct hostent *server;
	WSAStartup(MAKEWORD(2,2),&wsaData);
	s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	server = gethostbyname(SERVER);
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}
    memset((char *) &si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	bcopy((char *)server->h_addr,
	(char *)&si_other.sin_addr.s_addr,
	server->h_length);
    si_other.sin_addr.s_addr = INADDR_ANY;
	si_other.sin_port = htons(PORT);
    int optval = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof optval);
	sendto(s, "Start", strlen("Start") , 0 , (struct sockaddr *) &si_other, slen);
	while(1)
	{
		bzero(buf,BUFLEN);
		recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen);
		printf("%s\n",buf);
		bzero(message,BUFLEN);
		printf("Print login\n");
		fgets(message,BUFLEN-1,stdin);
		sendto(s,message, strlen(message),0,(struct sockaddr *) &si_other,slen);
		printf("send mes %s\n",message);
		bzero(buf,BUFLEN);
		recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen);
		bzero(message,BUFLEN);
		printf("Print password\n");
		fgets(message,BUFLEN-1,stdin);
		sendto(s,message, strlen(message),0,(struct sockaddr *) &si_other,slen);
		bzero(buf,BUFLEN);
		recvfrom(s,buf,BUFLEN,0,(struct sockaddr *) &si_other,&slen);
		if(!strncmp(buf,"next",strlen("next")))
		{
    		bzero(message,BUFLEN);
    		strncpy(message,"OK",strlen("OK"));
			sendto(s,message, strlen(message),0,(struct sockaddr *) &si_other,slen);
    		break;
		}
	}
    while(1)
    {
    	bzero(buf,BUFLEN);
		recvfrom(s,buf,BUFLEN,0,(struct sockaddr *) &si_other,&slen);
    	if((strncmp(buf,"OK",strlen("OK"))))
    	{
    		if(strncmp(buf,"next",strlen("next")))
    			printf("%s\n",buf);
    	}
    	if(!(strncmp(buf,"next",strlen("next")))
    	{
    		printf("Writing\n");
    		bzero(message,BUFLEN);
    		fgets(message,BUFLEN-1,stdin);
			sendto(s,message, strlen(message),0,(struct sockaddr *) &si_other,slen);
    	}
    	else
    	{
    		bzero(message,BUFLEN);
    		strncpy(message,"OK",strlen("OK"));
			sendto(s,message, strlen(message),0,(struct sockaddr *) &si_other,slen);
    	}
    }
}

