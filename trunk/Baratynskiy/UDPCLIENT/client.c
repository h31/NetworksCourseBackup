#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>

#define SERVER "127.0.0.1"
#define BUFLEN 1024  //Max length of buffer
#define PORT 8888   //The port on which to send data

void die(char *s)
{
    perror(s);
    exit(1);
}

int main(void)
{
    struct sockaddr_in si_other;
    int sock, slen=sizeof(si_other);
    char message[BUFLEN];

    if ( (sock=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }

    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);

    if (inet_aton(SERVER , &si_other.sin_addr) == 0)
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    while(1)
    {
    	bzero(message,sizeof(message));
        scanf("%s",message);

        if (sendto(sock, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen)==-1)
        {
            die("sendto()");
        }

        bzero(message,sizeof(message));
        if (recvfrom(sock, message, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1)
        {
            die("recvfrom()");
        }
        if (strcmp(message,"transfer")==0){
        	bzero(message,sizeof(message));
        	scanf("%s",message);
        	sendto(sock, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen);
        	recvfrom(sock, message, BUFLEN, 0, (struct sockaddr *) &si_other, &slen);
        	FILE *file = fopen(message, "r+");
        	while (!feof(file)) {
        		bzero(message,sizeof(message));
        		fread(message, 1, sizeof(message), file);
        		sendto(sock, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen);
        		recvfrom(sock, message, BUFLEN, 0, (struct sockaddr *) &si_other, &slen);
        	}
        	fclose(file);
        	sendto(sock, "&", BUFLEN , 0 , (struct sockaddr *) &si_other, slen);
    		recvfrom(sock, message, BUFLEN, 0, (struct sockaddr *) &si_other, &slen);
        }
    }

    close(sock);
    return 0;
}
