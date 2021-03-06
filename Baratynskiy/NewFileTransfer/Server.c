/*
 * Server.c
 *
 *  Created on: Oct 16, 2014
 *      Author: Baratynskiy
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>

#define bufSize 1024

struct SockParams{
	int sock, bufsocket, port, clilen;
	struct sockaddr_in serverAddr, clientAddr;
};

int doprocessing(int sock, FILE *f) {
	char buffer[bufSize];
	bzero(buffer, bufSize);
	read(sock, buffer, bufSize - 1);
	write(sock, "I got your message", 18);
	fprintf(f, "%s", buffer);
	return 0;
}

char* getFirstMessage(int sock, char *firstMessage) {
	bzero(firstMessage, bufSize);
	read(sock, firstMessage, bufSize - 1);
	if (strcmp(firstMessage, "ls") == 0) {
		write(sock, "ls command", 10);
		firstMessage = "ls";
		return "ls";
	} else if (strcmp(firstMessage, "get") == 0) {
		write(sock, "get command", 11);
		firstMessage = "get";
		return "get";
	} else if (strcmp(firstMessage, "mkdir") == 0) {
		write(sock, "mkdir command", 11);
		firstMessage = "mkdir";
		return "mkdir";
	} else if (strcmp(firstMessage, "cd") == 0) {
		write(sock, "cd command", 11);
		firstMessage = "cd";
		return "cd";
	} else {
		write(sock, "I got file name", 15);
	}
	return firstMessage;
}

int getFileSize(int sock, int size) {
	char buffer[bufSize];
	bzero(buffer, bufSize);
	read(sock, buffer, bufSize - 1);
	write(sock, "I got file size", 15);
	sscanf(buffer, "%d", &size);
	return size;
}

void listenToClients(int sock, int bufsocket, int clilen,
		struct sockaddr_in clientAddr, char *name, int fs) {
	getFirstMessage(bufsocket, name);
	if (strcmp(name, "ls") == 0) {
		system("ls>ls");
		FILE *ls = fopen("ls", "r");
		char charSize[30];
		char lsStr[30], buffer[30];
		while (!feof(ls)) {
			fgets(lsStr, sizeof(lsStr), ls);
			if (strcmp(lsStr, "ls\n") == 0 ) {
				continue;
			}
			write(bufsocket, lsStr, sizeof(lsStr));
			read(bufsocket, buffer, 1);
			bzero(lsStr, sizeof(lsStr));
		}
		write(bufsocket, "&", 2);
		fclose(ls);
		system("rm ls");
		return;
	}
	if (strcmp(name, "get") == 0) {
		char filename[30];
		char buffer[1024];
		read(bufsocket, filename, sizeof(filename));
		write(bufsocket, filename, sizeof(filename));
		printf("%s is wanted\n", filename);
		FILE *file = fopen(filename, "r+");
		while (!feof(file)) {
			fread(buffer, 1, sizeof(buffer), file);
			write(bufsocket, buffer, sizeof(buffer));
			read(bufsocket, buffer, sizeof(buffer));
		}
		write(bufsocket, "&", 1);
		read(bufsocket, buffer, sizeof(buffer));
		fclose(file);
		return;
	}
	if (strcmp(name, "mkdir") == 0) {
		char dirname[30];
		read(bufsocket, dirname, sizeof(dirname));
		write(bufsocket, dirname, sizeof(dirname));
		printf("Directory %s was created\n", dirname);
		mkdir(dirname, S_IRWXU);
		return;
	}
	if (strcmp(name, "cd") == 0) {
		char dirname[30];
		read(bufsocket, dirname, sizeof(dirname));
		write(bufsocket, dirname, sizeof(dirname));
		chdir(dirname);
		return;
	}
	char rmfile[70];
	bzero(rmfile,sizeof(rmfile));
	sscanf(rmfile, "rm %s", name);
	system(rmfile);
	bzero(rmfile,sizeof(rmfile));
	FILE *f = fopen(name, "ab");
	printf("%s was recieved!\n", name);
	fs = getFileSize(bufsocket, fs);
	if (fs % bufSize > 0) {
		fs = fs / bufSize + 1;
	} else {
		fs = fs / bufSize;
	}
	while (fs > 0) {
		fs--;
		doprocessing(bufsocket, f);
	}
	fclose(f);
	return;
}

void getUser(int sock, int bufsocket, int port, int clilen,
		struct sockaddr_in serverAddr, struct sockaddr_in clientAddr) {
	char user[30];
	char buffer[bufSize];
	printf("%d\n",bufsocket);
	bzero(buffer, bufSize);
	read(bufsocket, user, sizeof(user));
	chdir("/home/user/workspace/NewFileTransfer");
	int ch = chdir(user);
	if (ch == -1) {
		mkdir(user, S_IRWXU);
		chdir(user);
	}
	bzero(user, sizeof(user));
	write(bufsocket, "I got user name", 15);
}

void startThread(void *in){
	struct SockParams *sp = (struct SockParams*) in;
	int fs;
	char buffer[bufSize];
	char *name = buffer;
	puts("Thread was created");
	getUser(sp->sock, sp->bufsocket, sp->port, sp->clilen, sp->serverAddr, sp->clientAddr);
		while (1) {
			listenToClients(sp->sock, sp->bufsocket, sp->clilen, sp->clientAddr, name, fs);
		}

}

int main(int argc, char *argv[]) {
	pthread_t threads[5],mainthread;
	int i = 0;
	int j;
	struct SockParams sp;
	const int on = 1;
	sp.sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sp.sock < 0) {
		perror("ERROR opening socket");
	}
	bzero((char *) &sp.serverAddr, sizeof(sp.serverAddr));
	sp.port = 7777;
	sp.serverAddr.sin_family = AF_INET;
	sp.serverAddr.sin_addr.s_addr = INADDR_ANY;
	sp.serverAddr.sin_port = htons(sp.port);
	setsockopt(sp.sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	if (bind(sp.sock, (struct sockaddr *) &sp.serverAddr, sizeof(sp.serverAddr)) < 0) {
		perror("ERROR on binding");
	}
//	listen(sp.sock, 5);
//	sp.clilen = sizeof(sp.clientAddr);
	while(1){
		listen(sp.sock, 5);
		sp.clilen = sizeof(sp.clientAddr);
		sp.bufsocket = accept(sp.sock,(struct sockaddr *) &sp.clientAddr, &sp.clilen);
		pthread_create(&threads[i],NULL,startThread,(void*)&sp);
		i++;
	}
	for (j = 0; j < 5;j++){
		pthread_join(threads[j],NULL);
	}
	return 0;
}
