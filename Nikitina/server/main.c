#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "writeStruct.h"
#include "registration.h"

int main(int argc, char *argv[]) {
	char str[50];
	FILE *file;
	int i,numberTrueAnswer=0;
	char *clientFile="/home/user/workspace/server/registration.txt";
	char numberTest='0';
	char *name=(char*)malloc(50*sizeof(char));


	int sockfd, newsockfd, portno, clilen;
	const int on = 1;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("ERROR opening socket");
		exit(1);
	}
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = 5001;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("ERROR on binding");
		exit(1);
	}
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);

	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0) {
		perror("ERROR on accept");
		exit(1);
	}

	//Connect from client

	while(1){
	bzero(buffer, 256);
		n = read(newsockfd, buffer, 255);
		if (n < 0) {
				perror("ERROR reading from socket");
				exit(1);
		}
	if(buffer[0]=='!'){
		n = write(newsockfd, "Login", 5);
		break;
	}
	n = write(newsockfd, "ERROR", 5);
	}
	/*//Registration
	int numberClient;
	int clientSize=sizeFile(clientFile);
	struct Client c[clientSize];
	file = fopen(clientFile, "r");
	for (i = 0; fgets(str, sizeof(str), file); i++) {
						writeSizeClient(&c[i], &str);
	}
	fclose(file);
	while(1){
		numberClient=-1;
		bzero(buffer, 256);
		n = read(newsockfd, buffer, 255);
		for (i = 0; i<clientSize; i++) {
			if(strcmp(buffer, strcat(c[i].login,"\n"))==0){
				numberClient=i;
				break;
		}}
		if(numberClient!=-1){
			n = write(newsockfd, "Parol", 5);
			break;
		}
		n = write(newsockfd, "Login", 5);
		}
	//Enter Password
	while(1){
			bzero(buffer, 256);
			n = read(newsockfd, buffer, 255);
			if(strcmp(buffer, strcat(c[numberClient].parol, "\n"))==0){
					n = write(newsockfd, "OK", 2);
					break;
			}
			n = write(newsockfd, "Parol", 5);
			}
	//Enter last result
	char *string=writeLastResult(&c[numberClient]);
	printf("%s",string);*/

	//Number test
	while (1){
		bzero(buffer, 256);
		n = read(newsockfd, buffer, 255);
		numberTest = buffer[0];
		name=(char*)malloc(50*sizeof(char));
		sprintf(name, "%s%c%s", "/home/user/workspace/server/",numberTest, ".txt");
		if((file = fopen(name, "r"))!=NULL){
			n = write(newsockfd, "OK", 2);
			break;
		}
		n = write(newsockfd, "ERROR", 5);
		free(name);
	}
	int testSize = sizeFile(name);
	struct Line x[testSize];
	file = fopen(name, "r");
		for (i = 0; fgets(str, sizeof(str), file); i++) {
			writeSize(&x[i], &str);
		}
		fclose(file);
		for (i = 0; i < testSize; i++) {
		char *stringOut = writeToClient(&x[i]);
		n = write(newsockfd, stringOut, strlen(stringOut));
		printf("%d", strlen(stringOut));
		bzero(buffer, 256);
		n = read(newsockfd, buffer, 255);
		if (n < 0) {
			perror("ERROR reading from socket");
			exit(1);
		}
		printf("Answer: %s\n", buffer);
		if (buffer[0] == x[i].trueAnswer){
			n = write(newsockfd, "Right\n", 6);
			numberTrueAnswer=numberTrueAnswer+1;
		}
		else
			n = write(newsockfd, "Wrong\n", 6);
		if (n < 0) {
			perror("ERROR writing to socket");
			exit(1);
		}
	}

	free(name);
	name=(char*)malloc(100*sizeof(char));
	sprintf(name, "%s%d\n%s%d\n", "Number of question ",testSize,
			"Number of true answer is ",numberTrueAnswer);
	n = write(newsockfd, name, strlen(name));
	if (n < 0) {
				perror("ERROR writing to socket");
				exit(1);
			}
	n = write(newsockfd, "!\n", 2);
	for (i = 0; i < testSize; i++)
		freeLine(&x[i]);
	free(name);
	//for (i = 0; i < clientSize; i++)
		//freeClient(&c[i]);
	return 0;
}
