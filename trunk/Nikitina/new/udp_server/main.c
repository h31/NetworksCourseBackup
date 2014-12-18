#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>
#include <pthread.h>
#include "writeStruct.h"
#include "registration.h"
#define maxSize 500
pthread_t t1;
pthread_mutex_t mutex= PTHREAD_MUTEX_INITIALIZER;;
static void* worker(void* arg) {
	pthread_mutex_lock(&mutex);
    int sock = *(int*)arg;
    printf("Worker for %d is up\n", sock);
	int n;
	char buffer[maxSize];
	while(1){
        struct sockaddr_in cliaddr;
        socklen_t len = sizeof(struct sockaddr_in);
	char str[50];
	char result[50];
	FILE *file;
	struct Client c[50];
	int i, numberTrueAnswer = 0;
	char *clientFile = "/home/user/workspace/udp_server/registration.txt";
	char numberTest = '0';
	char *name = (char*) malloc(50 * sizeof(char));

			while (1) {
					bzero(buffer, maxSize);
					n = recvfrom(sock, buffer, 1,0,(struct sockaddr *) &cliaddr, &len);
					if (n < 0) {
						perror("ERROR reading from socket");
						exit(1);
					}
					n = sendto(sock, buffer, 1,0,(struct sockaddr *) &cliaddr, len);
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
				file = fopen(clientFile, "r");
				if (file == NULL) {
					perror("ERROR open file");
					exit(1);
				}
				for (i = 0; fgets(str, sizeof(str), file); i++) {
					writeSizeClient(&c[i], &str);
				}
				fclose(file);
				char bufferNew[maxSize];
				numberClient = -1;
				bzero(buffer, maxSize);
				bzero(bufferNew, maxSize);
				n = recvfrom(sock, buffer, maxSize,0,(struct sockaddr *) &cliaddr, &len);
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
					new(bufferNew, &client);
					c[clientSize - 1] = client;
					numberClient = clientSize - 1;
				}
				char strres[50];
							sprintf(strres,"%d#%d#%d#%s/\n",c[numberClient].numberTest,
									c[numberClient].sizeQuestion,c[numberClient].sizeTrueAnswer,c[numberClient].login);
							strcpy(result ,strres);
				n = sendto(sock, result, strlen(result),0,(struct sockaddr *) &cliaddr, len);
					if (n < 0) {
						perror("ERROR writing to socket");
						exit(1);
					}
				//List of test
				while (1) {
					bzero(buffer, maxSize);
									n = recvfrom(sock, buffer, 1,0,(struct sockaddr *) &cliaddr, &len);
									if (n < 0) {
										perror("ERROR reading from socket");
										exit(1);
									}
									n = sendto(sock, buffer, 1,0,(struct sockaddr *) &cliaddr, len);
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
					sprintf(name, "%s%d%s", "/home/user/workspace/udp_server/test/", i, ".txt");
					if ((file = fopen(name, "r")) != NULL) {
						sprintf(s, "#%d", i);
						strcat(res, s);
						fclose(file);
					}
				}
				n = sendto(sock, res, strlen(res),0,(struct sockaddr *) &cliaddr, len);
				if (n < 0) {
					perror("ERROR writing to socket");
					exit(1);
				}

			//Number test
				bzero(buffer, maxSize);
					n = recvfrom(sock, buffer, maxSize,0,(struct sockaddr *) &cliaddr, &len);
					if (n < 0) {
						perror("ERROR reading from socket");
						exit(1);
					}
					numberTest=toInt(buffer);
					sprintf(name, "%s%d%s", "/home/user/workspace/udp_server/test/", numberTest,
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
									bzero(buffer, maxSize);
													n = recvfrom(sock, buffer, 1,0,(struct sockaddr *) &cliaddr, &len);
													if (n < 0) {
														perror("ERROR reading from socket");
														exit(1);
													}
													if(end)
														buffer[0]='/';
													n = sendto(sock, buffer, 1,0,(struct sockaddr *) &cliaddr, len);
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
					n = sendto(sock, str, strlen(str),0,(struct sockaddr *) &cliaddr, len);
						if (n < 0) {
							perror("ERROR writing to socket");
							exit(1);
						}
						//Answer
					bzero(buffer, maxSize);
					n = recvfrom(sock, buffer, 1,0,(struct sockaddr *) &cliaddr, &len);
					if (n < 0) {
						perror("ERROR reading from socket");
						exit(1);
					}
					printf("Answer: %s\n", buffer);
					if (buffer[0] == trueAnswer+'0') {
						n = sendto(sock, "Right\n", 6,0,(struct sockaddr *) &cliaddr, len);
						if (n < 0) {
							perror("ERROR writing to socket");
							exit(1);
						}
						numberTrueAnswer = numberTrueAnswer + 1;
					} else
						n = sendto(sock, "Wrong\n", 6,0,(struct sockaddr *) &cliaddr, len);
					if (n < 0) {
						perror("ERROR writing to socket");
						exit(1);
					}
				}
				sprintf(name, "%d#%d#%d#%s/", numberTest,testSize, numberTrueAnswer,c[numberClient].login);
				n = sendto(sock, name, strlen(name),0,(struct sockaddr *) &cliaddr, len);
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

	pthread_mutex_unlock(&mutex);
    shutdown(sock, 2);
    close(sock);
}

int main(int argc, char**argv) {
	int sock;
	int portno=5001;
	//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	if(argc<2)
printf("use default port 5001\n");
	else portno=atoi(argv[1]);
	struct sockaddr_in servaddr, cliaddr;
	  pthread_t accept_thread;
	sock = socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(portno);
    int optval = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
	bind(sock, (struct sockaddr *) &servaddr, sizeof(servaddr));
listen(sock,5);
	    pthread_create(&(accept_thread),NULL, worker, (void*) &sock);
    while (1) {
        char command = getchar();
        if (command == 'q') {
            break;
            //int pthread_mutex_destroy(pthread_mutex_t *mutex);
        }
    }
	    printf("Exit...\n");
	    shutdown(sock, 2);
	    close(sock);
	   pthread_join(accept_thread, NULL);
	    printf("Done\n");
	return 0;
}
