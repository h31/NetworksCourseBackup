#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "writeStruct.h"
#include "registration.h"
#include <time.h>
#include <netdb.h>
#include <pthread.h>
pthread_t t1;

void* thread1(int sock){
	printf("New client\n");
	char buffer[256];
	int n;
	char str[50];
		char result[50];
		FILE *file;
		int i, numberTrueAnswer = 0;
		char *clientFile = "/home/user/workspace/server/registration.txt";
		int numberTest = 0;
		char *name = (char*) malloc(50 * sizeof(char));
		while (1) {
				bzero(buffer, 256);
				n = read(sock, buffer, 1);
				if (n < 0) {
					perror("ERROR reading from socket");
					exit(1);
				}
				n = write(sock, buffer, 1);
				if (n < 0) {
					perror("ERROR writing to socket");
					exit(1);
			}
				if (buffer[0] == '!')
					break;
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
			n = read(sock, buffer, 255);
			if (n < 0) {
				perror("ERROR reading from socket");
				exit(1);
			}
			for (i = 0; i < strlen(buffer) - 1; i++)
				bufferNew[i] = buffer[i];
			for (i = 0; i < clientSize; i++) {
				if (strcmp(bufferNew, c[i].login) == NULL) {
					numberClient = i;
					break;
				}
			}
			//New client
			if (numberClient == -1) {
				clientSize++;
				struct Client client;
				newUser(bufferNew, &client);
				c[clientSize - 1] = client;
				numberClient = clientSize - 1;
			}
			char strres[50];
			sprintf(strres,"%d#%d#%d#%s/\n",c[numberClient].numberTest,
					c[numberClient].sizeQuestion,c[numberClient].sizeTrueAnswer,c[numberClient].login);
			strcpy(result ,strres);
			n = write(sock, result, strlen(result));
				if (n < 0) {
					perror("ERROR writing to socket");
					exit(1);
				}
			//List of test
			while (1) {
				bzero(buffer, 256);
								n = read(sock, buffer, 1);
								if (n < 0) {
									perror("ERROR reading from socket");
									exit(1);
								}
								n = write(sock, buffer, 1);
								if (n < 0) {
									perror("ERROR writing to socket");
									exit(1);
							}
								if (buffer[0] == '1')
									break;
			}
			char res[60]="/" ;
			char s[3];
			for (i = 50; i >0; i--) {
				sprintf(name, "%s%d%s", "/home/user/workspace/server/test/", i, ".txt");
				if ((file = fopen(name, "r")) != NULL) {
					sprintf(s, "#%d", i);
					strcat(res, s);
					fclose(file);
				}
			}
			n = write(sock, res, strlen(res));
			if (n < 0) {
				perror("ERROR writing to socket");
				exit(1);
			}

		//Number test
			bzero(buffer, 256);
				n = read(sock, buffer, 255);
				if (n < 0) {
					perror("ERROR reading from socket");
					exit(1);
				}
				numberTest=toInt(buffer);
				sprintf(name, "%s%d%s", "/home/user/workspace/server/test/", numberTest,
						".txt");
				file = fopen(name, "r");
			int testSize = sizeFile(name);
			int trueAnswer;
			file = fopen(name, "r");
			if (file == NULL) {
				perror("ERROR open file");
				exit(1);
			}
			int end=0;
			while(1){
				if(!fgets(str, sizeof(str), file))
					end=1;
				while (1) {
								bzero(buffer, 256);
												n = read(sock, buffer, 1);
												if (n < 0) {
													perror("ERROR reading from socket");
													exit(1);
												}
												if(end)
													buffer[0]='/';
												n = write(sock, buffer, 1);
												if (n < 0) {
													perror("ERROR writing to socket");
													exit(1);
											}
												if (buffer[0] == '2' || buffer[0] == '/')
													break;
							}
				if(end)
					break;
				trueAnswer=readTrueAnswer(str);
				n = write(sock, str, strlen(str));
					if (n < 0) {
						perror("ERROR writing to socket");
						exit(1);
					}
					//Answer
				bzero(buffer, 256);
				n = read(sock, buffer, 1);
				if (n < 0) {
					perror("ERROR reading from socket");
					exit(1);
				}
				printf("Answer: %s\n", buffer);
				if (buffer[0] == trueAnswer+'0') {
					n = write(sock, "Right\n", 6);
					if (n < 0) {
						perror("ERROR writing to socket");
						exit(1);
					}
					numberTrueAnswer = numberTrueAnswer + 1;
				} else
					n = write(sock, "Wrong\n", 6);
				if (n < 0) {
					perror("ERROR writing to socket");
					exit(1);
				}
			}
			sprintf(name, "%d#%d#%d#%s/", numberTest,testSize, numberTrueAnswer,c[numberClient].login);
			n = write(sock, name, strlen(name));
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
			};
			fclose(file);
			free(name);
}
int main(int argc, char *argv[]) {

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
	//setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	bzero((char *) &serv_addr, sizeof(serv_addr));
	//portno = 5001;
	if (argc < 2) {
			printf("usage default port 5001\n");
			portno = 5001;
	}
	else
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("ERROR on binding");
		exit(1);
	}
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);
	bzero(buffer, 256);
	while(1){
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0) {
		perror("ERROR on accept");
		exit(1);
	}
	pthread_create(&t1, NULL,thread1,newsockfd);
	}
	pthread_join(&t1,NULL);
	return 0;
}
