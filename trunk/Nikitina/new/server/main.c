#include <stdio.h>
#include <stdlib.h>
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
			if (n < 0) {
				perror("ERROR writing to socket");
				exit(1);
			}
			break;
		}
		n = write(newsockfd, "ERROR", 5);
		if (n < 0) {
			perror("ERROR writing to socket");
			exit(1);
		}
	}
	//Registration
	int numberClient;
	int clientSize = sizeFile(clientFile);
	struct Client c[50];
	file = fopen(clientFile, "r");
	if (file == NULL) {
		perror("ERROR open file");
		exit(1);
	}
	for (i = 0; fgets(str, sizeof(str), file); i++) {
		writeSizeClient(&c[i], &str);
	}
	fclose(file);
	char bufferNew[256];
	numberClient = -1;
	bzero(buffer, 256);
	bzero(bufferNew, 256);
	n = read(newsockfd, buffer, 255);
	if (n < 0) {
		perror("ERROR reading from socket");
		exit(1);
	}
	for (i = 0; i < strlen(buffer) - 1; i++)
		bufferNew[i] = buffer[i];
	for (i = 0; i < clientSize; i++) {
		if (strcmp(bufferNew, c[i].login) == NULL) {
			numberClient = i;
			result = writeLastResult(&c[numberClient]);
			n = write(newsockfd, result, strlen(result));
			if (n < 0) {
				perror("ERROR writing to socket");
				exit(1);
			}
			break;
		}
	}

	//New client
	if (numberClient == -1) {
		clientSize++;
		struct Client client;
		new(bufferNew, &client);
		c[clientSize - 1] = client;
		numberClient = clientSize - 1;
		n = write(newsockfd, "OK", 2);
		if (n < 0) {
			perror("ERROR writing to socket");
			exit(1);
		}
	}
	//List of test
	while (1) {
		bzero(buffer, 256);
		n = read(newsockfd, buffer, 255);
		if (n < 0) {
			perror("ERROR reading from socket");
			exit(1);
		}
		if (buffer[0] == '!')
			break;
		n = write(newsockfd, "ERROR", 5);
		if (n < 0) {
			perror("ERROR writing to socket");
			exit(1);
		}
	}
	char res[60] = "List of tests: ";
	char s[3];
	for (i = 1; i < 50; i++) {
		sprintf(name, "%s%d%s", "/home/user/workspace/server/test/", i, ".txt");
		if ((file = fopen(name, "r")) != NULL) {
			sprintf(s, "%d ", i);
			strcat(res, s);
			fclose(file);
		}
	}

	n = write(newsockfd, res, strlen(res));
	if (n < 0) {
		perror("ERROR writing to socket");
		exit(1);
	}

//Number test
	while (1) {
		bzero(buffer, 256);
		n = read(newsockfd, buffer, 255);
		if (n < 0) {
			perror("ERROR reading from socket");
			exit(1);
		}
		numberTest = buffer[0];
		sprintf(name, "%s%c%s", "/home/user/workspace/server/test/", numberTest,
				".txt");
		if ((file = fopen(name, "r")) != NULL) {
			n = write(newsockfd, "OK", 2);
			if (n < 0) {
				perror("ERROR writing to socket");
				exit(1);
			}
			break;
		}
		n = write(newsockfd, "ERROR", 5);
		if (n < 0) {
			perror("ERROR writing to socket");
			exit(1);
		}
	}
	int testSize = sizeFile(name);
	struct Line x[testSize];
	file = fopen(name, "r");
	if (file == NULL) {
		perror("ERROR open file");
		exit(1);
	}
	for (i = 0; fgets(str, sizeof(str), file); i++) {
		writeSize(&x[i], &str);
	}
	fclose(file);

	for (i = 0; i < testSize; i++) {
		while (1) {
			bzero(buffer, 256);
			n = read(newsockfd, buffer, 255);
			if (n < 0) {
				perror("ERROR reading from socket");
				exit(1);
			}
			if (buffer[0] == '!')
				break;
			n = write(newsockfd, "ERROR", 5);
			if (n < 0) {
				perror("ERROR writing to socket");
				exit(1);
			}
		}

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
			if (n < 0) {
				perror("ERROR writing to socket");
				exit(1);
			}
			numberTrueAnswer = numberTrueAnswer + 1;
		} else
			n = write(newsockfd, "Wrong\n", 6);
		if (n < 0) {
			perror("ERROR writing to socket");
			exit(1);
		}
	}
	free(name);
	name = (char*) malloc(100 * sizeof(char));
	sprintf(name, "%s%d\n%s%d!", "Number of question ", testSize,
			"Number of true answer is ", numberTrueAnswer);
	n = write(newsockfd, name, strlen(name));
	if (n < 0) {
		perror("ERROR writing to socket");
		exit(1);
	}
	newResult(&c[numberClient], numberTest, testSize, numberTrueAnswer);
	file = fopen("/home/user/workspace/server/registration.txt", "w");
	if (file == NULL) {
		perror("ERROR open file");
		exit(1);
	}
	for (i = 0; i < clientSize; i++) {
		fprintf(file, "%d#%d#%d#%s/\n", c[i].numberTest, c[i].sizeQuestion,
				c[i].sizeTrueAnswer, c[i].login);
	}
	fclose(file);
	for (i = 0; i < testSize; i++)
		freeLine(&x[i]);
	free(name);
	return 0;
}
