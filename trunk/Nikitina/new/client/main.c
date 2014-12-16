#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "write.h"

int main(int argc, char *argv[]) {
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char end;
	char buffer[256];
	int i;

	if (argc < 3) {
		fprintf(stderr, "usage %s hostname port\n", argv[0]);
		exit(0);
	}
	portno = atoi(argv[2]);
	/* Create a socket point */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("ERROR opening socket");
		exit(1);
	}
	server = gethostbyname(argv[1]);
	if (server == NULL) {
		fprintf(stderr, "ERROR, no such host\n");
		exit(0);
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	/* bcopy((char *)server->h_addr,
	 (char *)&serv_addr.sin_addr.s_addr,
	 server->h_length);*/
	serv_addr.sin_port = htons(portno);

	/* Now connect to the server */
	if (connect(sockfd, &serv_addr, sizeof(serv_addr)) < 0) {
		perror("ERROR connecting");
		exit(1);
	}
	/* Now ask for a message from the user, this message
	 * will be read by server
	 */
	//Start connect
	buffer[0] = '-';
	while (buffer[0] != '!') {
		printf("Please enter '!' to connect :");
		bzero(buffer, 256);
		fgets(buffer, 255, stdin);
		n = write(sockfd, buffer, 1);
		if (n < 0) {
			perror("ERROR writing to socket");
			exit(1);
		}
		bzero(buffer, 256);
		n = read(sockfd, buffer, 1);
		if (n < 0) {
			perror("ERROR reading from socket");
			exit(1);
		}
	}
	//Registration
	printf("Enter login: ");
	bzero(buffer, 256);
	fgets(buffer, 255, stdin);
	n = write(sockfd, buffer, strlen(buffer));
	if (n < 0) {
		perror("ERROR writing to socket");
		exit(1);
	}
	bzero(buffer, 256);
	n = read(sockfd, buffer, 255);
	if (n < 0) {
		perror("ERROR reading from socket");
		exit(1);
	}
	struct Client c;
	writeSizeClient(&c, buffer);
	if(c.numberTest==0)
		printf("Welcome, %s.\n",c.login);
	else
	printf("%s your last test is %d. Result %d of %d.\n", c.login,c.numberTest ,c.sizeTrueAnswer,c.sizeQuestion);
	free(c.login);
	buffer[0] = '-';
	while (buffer[0] != '1') {
		printf("Please enter '1' to receive list of test :");
		bzero(buffer, 256);
		fgets(buffer, 255, stdin);
		n = write(sockfd, buffer, 1);
		if (n < 0) {
			perror("ERROR writing to socket");
			exit(1);
		}
		bzero(buffer, 256);
		n = read(sockfd, buffer, 1);
		if (n < 0) {
			perror("ERROR reading from socket");
			exit(1);
		}
	}
	bzero(buffer, 256);
	n = read(sockfd, buffer, 256);
			if (n < 0) {
				perror("ERROR reading from socket");
				exit(1);
			}
	int listOfTest[50];
	int size, number;
	int true=0;
	char res[50];
	size=writeListTest(listOfTest,buffer,res);
	printf("%s\n", res);
	while(true!=1){
		printf("Choose test: ");
		bzero(buffer, 256);
		fgets(buffer, 255, stdin);
		number=toInt(buffer);
		for(i=0;i<size;i++){
		if(number==listOfTest[i]){
			true=1;
			break;
		}}
	}
	n = write(sockfd, buffer, strlen(buffer));
			if (n < 0) {
						perror("ERROR writing to socket");
						exit(1);
			}
	struct Line x;
	//LOOP
	while (1) {
		buffer[0] = '-';
			while (buffer[0] != '2') {
				printf("Please enter '2' to next question :");
				bzero(buffer, 256);
				fgets(buffer, 255, stdin);
				n = write(sockfd, buffer, 1);
				if (n < 0) {
					perror("ERROR writing to socket");
					exit(1);
				}
				bzero(buffer, 256);
				n = read(sockfd, buffer, 1);
				if (n < 0) {
					perror("ERROR reading from socket");
					exit(1);
				}
				if(buffer[0]=='/')
					break;
			}
			if(buffer[0]=='/')
								break;
						bzero(buffer, 256);
						n = read(sockfd, buffer, 256);
						if (n < 0) {
							perror("ERROR reading from socket");
							exit(1);
						}
			writeSize(&x,buffer);
			char str[sizeStr(&x)];
			char* stringOut = (char*) malloc(50 * sizeof(char));
			writeToClient(&x,str,stringOut);
			printf("%s\n", stringOut);

		freeLine(&x);
		while(1){
		bzero(buffer, 256);
		fgets(buffer, 255, stdin);
		if(!strcmp(buffer,"1\n")  || !strcmp(buffer,"2\n")  || !strcmp(buffer,"3\n")  || !strcmp(buffer,"4\n") )
			break;
		else
			printf("Enter your answer(1,2,3,4):\n");
		}
		n = write(sockfd, buffer, 1);
		if (n < 0) {
			perror("ERROR writing to socket");
			exit(1);
		}
		bzero(buffer, 256);
		n = read(sockfd, buffer, 6);
		if (n < 0) {
			perror("ERROR reading from socket");
			exit(1);
		}
	}
	n = read(sockfd, buffer, 256);
			if (n < 0) {
				perror("ERROR reading from socket");
				exit(1);
			}
			struct Client c1;
writeSizeClient(&c1, &buffer);
printf("The end.\n Test is %d. True answer %d of %d.\n", c1.numberTest,c1.sizeTrueAnswer,c1.sizeQuestion);
free(c1.login);
							close(sockfd);
	return 0;
}

