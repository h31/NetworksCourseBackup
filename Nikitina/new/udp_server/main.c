#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>
#include <pthread.h>
#include "writeStruct.h"
#include "registration.h"
pthread_t t1;

/*void* thread1(int sock) {
}*/
int main(int argc, char**argv) {
	int sock;
	struct sockaddr_in servaddr, cliaddr;

	sock = socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(32000);
	bind(sock, (struct sockaddr *) &servaddr, sizeof(servaddr));
	socklen_t len;
	char mesg[1000];
	int n;
	len = sizeof(cliaddr);
	char str[50];
	char *result;
	FILE *file;
	int i, numberTrueAnswer = 0;
	char *clientFile = "/home/user/workspace/udp_server/registration.txt";
	char numberTest = '0';
	char *name = (char*) malloc(50 * sizeof(char));
	while (1) {
		bzero(mesg, 1000);
		n = recvfrom(sock, mesg, 1000, 0, (struct sockaddr *) &cliaddr, &len);
		if (mesg[0] == '!') {
			sendto(sock, "Login", 5, 0, (struct sockaddr *) &cliaddr,
					sizeof(cliaddr));
			break;
		}
		sendto(sock, "ERROR", 5, 0, (struct sockaddr *) &cliaddr,
				sizeof(cliaddr));
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
	numberClient = -1;
	char bufferNew[1000];
	bzero(bufferNew, 1000);
	bzero(mesg, 1000);
	n = recvfrom(sock, mesg, 1000, 0, (struct sockaddr *) &cliaddr, &len);
	for (i = 0; i < strlen(mesg) - 1; i++)
		bufferNew[i] = mesg[i];
	for (i = 0; i < clientSize; i++) {
	 if (strcmp(bufferNew, c[i].login) == NULL) {
	 numberClient = i;
	 result = writeLastResult(&c[numberClient]);
	 sendto(sock,result, strlen(result),0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
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
	 bzero(mesg, 1000);
	 sendto(sock,"OK", 2,0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
	 }
	//List of test
	while (1) {
		bzero(mesg, 1000);
		n = recvfrom(sock, mesg, 1000, 0, (struct sockaddr *) &cliaddr, &len);
		if (mesg[0] == '!')
			break;
		sendto(sock, "ERROR", 5, 0, (struct sockaddr *) &cliaddr,
				sizeof(cliaddr));
	}
	char res[60] = "/";
	char s[3];
	for (i = 50; i > 0; i--) {
		sprintf(name, "%s%d%s", "/home/user/workspace/udp_server/test/", i, ".txt");
		if ((file = fopen(name, "r")) != NULL) {
			sprintf(s, "#%d", i);
			strcat(res, s);
			fclose(file);
		}
	}
	sendto(sock, res, strlen(res), 0, (struct sockaddr *) &cliaddr,
			sizeof(cliaddr));

	//Number test
	while (1) {
		n = recvfrom(sock, mesg, 1000, 0, (struct sockaddr *) &cliaddr, &len);
		numberTest = mesg[0];
		sprintf(name, "%s%c%s", "/home/user/workspace/udp_server/test/", numberTest,
				".txt");
		if (file = fopen(name, "r") != NULL)
			break;
	}
	int testSize = sizeFile(name);
	int trueAnswer;
	file = fopen(name, "r");
	if (file == NULL) {
		perror("ERROR open file");
		exit(1);
	}
	for (i = 0; fgets(str, sizeof(str), file); i++) {
		sendto(sock, "OK", 2, 0, (struct sockaddr *) &cliaddr,
					sizeof(cliaddr));
		trueAnswer = readTrueAnswer(str);
		while (1) {
			bzero(mesg, 1000);
			n = recvfrom(sock, mesg, 1000, 0, (struct sockaddr *) &cliaddr,
					&len);
			if (mesg[0] == '!')
				break;
			sendto(sock, "ERROR", 5, 0, (struct sockaddr *) &cliaddr,
					sizeof(cliaddr));
		}
		sendto(sock, str, strlen(str), 0, (struct sockaddr *) &cliaddr,
				sizeof(cliaddr));

		while (1) {
			bzero(mesg, 1000);
			n = recvfrom(sock, mesg, 1000, 0, (struct sockaddr *) &cliaddr,
					&len);
			if (mesg[0] == '1' || mesg[0] == '2' || mesg[0] == '3'
					|| mesg[0] == '4')
				break;
		}
		printf("Answer: %s\n", mesg);
		if (mesg[0] == trueAnswer + '0') {
			sendto(sock, "Right", 5, 0, (struct sockaddr *) &cliaddr,
					sizeof(cliaddr));
			numberTrueAnswer = numberTrueAnswer + 1;
		} else
			sendto(sock, "Wrong", 5, 0, (struct sockaddr *) &cliaddr,
					sizeof(cliaddr));
	}
	newResult(&c[numberClient], numberTest, testSize, numberTrueAnswer);
	file = fopen(clientFile, "w");
	if (file == NULL) {
		perror("ERROR open file");
		exit(1);
	}
	for (i = 0; i < clientSize; i++) {
		fprintf(file, "%d#%d#%d#%s/\n", c[i].numberTest, c[i].sizeQuestion,
				c[i].sizeTrueAnswer, c[i].login);
	}
	fclose(file);
	sprintf(name, "%s%d\n%s%d!", "Number of question ", testSize,
			"Number of true answer is ", numberTrueAnswer);
	sendto(sock, name, strlen(name), 0, (struct sockaddr *) &cliaddr,
			sizeof(cliaddr));
	free(name);

	/*while (1) {
		pthread_create(&t1, NULL, thread1, sockfd);
	}
	pthread_join(&t1, NULL);*/
	return 0;
}
