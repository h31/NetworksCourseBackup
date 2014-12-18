#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define BUFLEN 1000

int main(int argc, char*argv[])
{
	int sockfd,n,portno;
	struct sockaddr_in servaddr,cliaddr;
	char sendline[BUFLEN];
	char recvline[BUFLEN];

	portno = atoi(argv[2]);
	sockfd=socket(AF_INET,SOCK_DGRAM,0);

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr=inet_addr(argv[1]);
	servaddr.sin_port=htons(portno);

	//Start connect
	while (1) {
		printf("\nInput Expression:\n");
		if(fgets(sendline, BUFLEN,stdin) != NULL)
			sendto(sockfd,sendline,strlen(sendline),0,
					(struct sockaddr *)&servaddr,sizeof(servaddr));
		/*if (sendline[strlen(sendline)-2]== '!' || sendline[strlen(sendline)-2]== '#') {
					printf("Wait...\n");
				}*/
		if (strcmp(sendline, "q\n") == 0) {
					printf("Exit\n");
					close(sockfd);
					break;
		}
		bzero(recvline,BUFLEN);
		n=recvfrom(sockfd,recvline,BUFLEN,0,NULL,NULL);
		printf("%s\n", recvline);
	}
	return 0;
}
