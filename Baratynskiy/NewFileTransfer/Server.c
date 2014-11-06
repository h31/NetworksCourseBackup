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

#define bufSize 1024

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

void listenToClients(FILE *f, int sock, int bufsocket, int clilen,
		struct sockaddr_in clientAddr, char *name, int fs) {
	//Listening for clients
	listen(sock, 5);
	clilen = sizeof(clientAddr);
	//Accept connection
	bufsocket = accept(sock, (struct sockaddr *) &clientAddr, &clilen);
	if (bufsocket < 0) {
		perror("ERROR on accept");
	}
	getFirstMessage(bufsocket, name);
	if (strcmp(name, "ls") == 0) {
		system("ls>ls");
		FILE *ls = fopen("ls", "r");
		char charSize[30];
		char lsStr[30], buffer[30];
		while (!feof(ls)) {
			fgets(lsStr, sizeof(lsStr), ls);
			if (strcmp(lsStr, "ls\n") == 0 || strcmp(lsStr, "newfile\n") == 0) {
				continue;
			}
			write(bufsocket, lsStr, sizeof(lsStr));
			read(bufsocket, buffer, 1);
			bzero(lsStr, sizeof(lsStr));
		}
		write(bufsocket, "&", 2);
		fclose(ls);
		system("rm ls");
		close(bufsocket);
		close(sock);
		fclose(f);
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
		close(bufsocket);
		close(sock);
		fclose(file);
		return;
	}
	if (strcmp(name, "mkdir") == 0) {
		char dirname[30];
		read(bufsocket, dirname, sizeof(dirname));
		write(bufsocket, dirname, sizeof(dirname));
		printf("Directory %s was created\n", dirname);
		mkdir(dirname, S_IRWXU);
		close(bufsocket);
		close(sock);
		return;
	}
	if (strcmp(name, "cd") == 0) {
		char dirname[30];
		read(bufsocket, dirname, sizeof(dirname));
		write(bufsocket, dirname, sizeof(dirname));
		chdir(dirname);
		close(bufsocket);
		close(sock);
		return;
	}
	printf("%s was recieved!\n", name);
	clilen = sizeof(clientAddr);
	//Accept connection
	bufsocket = accept(sock, (struct sockaddr *) &clientAddr, &clilen);
	if (bufsocket < 0) {
	}
	fs = getFileSize(bufsocket, fs);
	if (fs % bufSize > 0) {
		fs = fs / bufSize + 1;
	} else {
		fs = fs / bufSize;
	}
	while (fs > 0) {
		fs--;
		clilen = sizeof(clientAddr);
		bufsocket = accept(sock, (struct sockaddr *) &clientAddr, &clilen);
		if (bufsocket < 0) {
			continue;
		}
		doprocessing(bufsocket, f);
	}
	close(bufsocket);
	close(sock);
	fclose(f);
	rename("newfile", name);
	return;
}

void getUser(int sock, int bufsocket, int port, int clilen,
		struct sockaddr_in serverAddr, struct sockaddr_in clientAddr) {
	char user[30];
	char buffer[bufSize];
	const int on = 1;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("ERROR opening socket");
	}
	bzero((char *) &serverAddr, sizeof(serverAddr));
	port = 7777;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(port);
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	if (bind(sock, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
		perror("ERROR on binding");
	}
	listen(sock, 5);
	clilen = sizeof(clientAddr);
	bufsocket = accept(sock, (struct sockaddr *) &clientAddr, &clilen);
	if (bufsocket < 0) {
		perror("ERROR on accept");
	}
	bzero(buffer, bufSize);
	read(bufsocket, user, sizeof(user));
	//puts(user);
	int ch = chdir(user);
	if (ch == -1) {
		mkdir(user, S_IRWXU);
		chdir(user);
	}
	bzero(user, sizeof(user));
	write(bufsocket, "I got user name", 15);
	close(bufsocket);
	close(sock);
}

int main(int argc, char *argv[]) {
	int sock, bufsocket, port, clilen;
	const int on = 1;
	char buffer[bufSize];
	struct sockaddr_in serverAddr, clientAddr;
	char *name = buffer;
	int fs;
	getUser(sock, bufsocket, port, clilen, serverAddr, clientAddr);
	while (1) {
		FILE *f = fopen("newfile", "ab");
		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock < 0) {
			perror("ERROR opening socket");
		}
		//Initializing socket
		bzero((char *) &serverAddr, sizeof(serverAddr));
		port = 7777;
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_addr.s_addr = INADDR_ANY;
		serverAddr.sin_port = htons(port);

		//Bind the host address
		setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
		if (bind(sock, (struct sockaddr *) &serverAddr, sizeof(serverAddr))
				< 0) {
			perror("ERROR on binding");
		}
		listenToClients(f, sock, bufsocket, clilen, clientAddr, name, fs);
	}
	return 0;
}
