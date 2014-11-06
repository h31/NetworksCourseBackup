#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "writeStruct.h"
#include "registration.h"

int main(int argc, char *argv[]) {
	char str[50];
	char *result;
	FILE *file;
	int i, numberTrueAnswer = 0;
	char *clientFile = "/home/user/workspace/server/registration.txt";
	char numberTest = '0';
	char *name = (char*) malloc(50 * sizeof(char));
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

	while (1) {
		bzero(buffer, 256);
		n = read(newsockfd, buffer, 255);
		if (n < 0) {
			perror("ERROR reading from socket");
			exit(1);
		}
		if (buffer[0] == '!') {
			n = write(newsockfd, "Login", 5);
			break;
		}
		n = write(newsockfd, "ERROR", 5);
	}
	//Registration
	int numberClient;
	int clientSize = sizeFile(clientFile);
	struct Client *cNew;
	struct Client *c = (struct Client*) malloc(
			clientSize * sizeof(struct Client));
	file = fopen(clientFile, "r");
	for (i = 0; fgets(str, sizeof(str), file); i++) {
		writeSizeClient(&c[i], &str);
	}
	fclose(file);
	char bufferNew[256];
	bzero(buffer, 256);
	n = read(newsockfd, buffer, 255);
	if (strcmp(buffer, "yes\n") == NULL) {
		char login[256];
		//Enter new login
		while (1) {
			numberClient = -1;
			bzero(buffer, 256);
			bzero(bufferNew, 256);
			n = read(newsockfd, buffer, 255);
			for (i = 0; i < strlen(buffer) - 1; i++)
				bufferNew[i] = buffer[i];
			for (i = 0; i < clientSize; i++) {
				if (strcmp(bufferNew, c[i].login) == NULL) {
					numberClient = i;
					break;
				}
			}
			if (numberClient == -1) {
				strcpy(login, bufferNew);
				n = write(newsockfd, "OK", 2);
				break;
			}
			n = write(newsockfd, "ERROR", 5);
		}
		//Enter password
		bzero(buffer, 256);
		bzero(bufferNew, 256);
		n = read(newsockfd, buffer, 255);
		for (i = 0; i < strlen(buffer) - 1; i++)
			bufferNew[i] = buffer[i];
		clientSize++;
		//New client
		struct Client client;
		new(login, bufferNew, &client);
		c = (struct Client*) realloc(c, clientSize * sizeof(struct Client));
		c[clientSize] = client;
		numberClient=clientSize;
	}
	//Enter last login
	else {
		while (1) {
			numberClient = -1;
			bzero(buffer, 256);
			bzero(bufferNew, 256);
			n = read(newsockfd, buffer, 255);
			for (i = 0; i < strlen(buffer) - 1; i++)
				bufferNew[i] = buffer[i];
			for (i = 0; i < clientSize; i++) {
				if (strcmp(bufferNew, c[i].login) == NULL) {
					numberClient = i;
					break;
				}
			}
			if (numberClient != -1) {
				n = write(newsockfd, "OK", 2);
				break;
			}
			n = write(newsockfd, "ERROR", 5);
		}
		//Enter last parol
		while (1) {
			bzero(buffer, 256);
			bzero(bufferNew, 256);
			n = read(newsockfd, buffer, 255);
			for (i = 0; i < strlen(buffer) - 1; i++)
				bufferNew[i] = buffer[i];
			if (strcmp(bufferNew, c[numberClient].parol) == NULL) {
				result = writeLastResult(&c[numberClient]);
				n = write(newsockfd, result, strlen(result));
				break;
			}
			n = write(newsockfd, "ERROR", 5);
		}
	}
	//Number test
	while (1) {
		free(name);
		bzero(buffer, 256);
		n = read(newsockfd, buffer, 255);
		numberTest = buffer[0];
		name = (char*) malloc(50 * sizeof(char));
		sprintf(name, "%s%c%s", "/home/user/workspace/server/test/", numberTest,
				".txt");
		if ((file = fopen(name, "r")) != NULL) {
			n = write(newsockfd, "OK", 2);
			break;
		}
		n = write(newsockfd, "ERROR", 5);
	}
	int testSize = sizeFile(name);
	struct Line x[testSize];
	file = fopen(name, "r");
	for (i = 0; fgets(str, sizeof(str), file); i++) {
		writeSize(&x[i], &str);
		}
fclose(file);
		for (i = 0; i<testSize; i++) {
		char *stringOut = writeToClient(&x[i]);
		n = write(newsockfd, stringOut, strlen(stringOut));
		bzero(buffer, 256);
		n = read(newsockfd, buffer, 255);
		if (n < 0) {
			perror("ERROR reading from socket");
			exit(1);
		}
		printf("Answer: %s\n", buffer);
		if (buffer[0] == x[i].trueAnswer) {
			n = write(newsockfd, "Right\n", 6);
			numberTrueAnswer = numberTrueAnswer + 1;
		} else
			n = write(newsockfd, "Wrong\n", 6);
	}
	newResult(&c[numberClient],1,testSize,numberTrueAnswer);
	for (i = 0; i < clientSize; i++) {
		registration(&c[i],1);
	}
	free(name);
	name = (char*) malloc(100 * sizeof(char));
	sprintf(name, "%s%d\n%s%d!\n", "Number of question ", testSize,
			"Number of true answer is ", numberTrueAnswer);
	n = write(newsockfd, name, strlen(name));
	if (n < 0) {
		perror("ERROR writing to socket");
		exit(1);
	}
	for (i = 0; i < testSize; i++)
		freeLine(&x[i]);
	free(name);
	for (i = 0; i < clientSize; i++)
		freeClient(&c[i]);
	return 0;
}
