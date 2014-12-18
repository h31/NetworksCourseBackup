/*
 * Server.c
 *
 *  Created on: Oct 16, 2014
 *      Author: Baratynskiy
 */

#include <io.h>
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <locale.h>
#include <sys/types.h>
#include <limits.h>
#include <sys/stat.h>
#pragma comment(lib, "WS2_32.lib")
#define bufSize 1024
HANDLE h1;


struct SockParams{
	WSADATA wsa;
	SOCKET sock, bufsocket; 
	int port, clilen;
	struct sockaddr_in serverAddr, clientAddr;
};

int doprocessing(int sock, FILE *f) {
	char buffer[bufSize];
	memset(buffer,0,bufSize);
	recv(sock, buffer, bufSize - 1,0);
	send(sock, "I got your message", 18,0);
	fprintf(f, "%s", buffer);
	return 0;
}

char* getFirstMessage(int sock, char *firstMessage) {
	memset(firstMessage,0,bufSize);
	recv(sock, firstMessage, bufSize - 1,0);
	if (strcmp(firstMessage, "ls") == 0) {
		send(sock, "ls command", 10,0);
		firstMessage = "ls";
		return "ls";
	} else if (strcmp(firstMessage, "get") == 0) {
		send(sock, "get command", 11,0);
		firstMessage = "get";
		return "get";
	} else if (strcmp(firstMessage, "mkdir") == 0) {
		send(sock, "mkdir command", 11,0);
		firstMessage = "mkdir";
		return "mkdir";
	} else if (strcmp(firstMessage, "cd") == 0) {
		send(sock, "cd command", 11,0);
		firstMessage = "cd";
		return "cd";
	} else {
		send(sock, "I got file name", 15,0);
	}
	return firstMessage;
}

int getFileSize(int sock, int size) {
	char buffer[bufSize];
	memset(buffer,0,bufSize);
	recv(sock, buffer, bufSize - 1,0);
	send(sock, "I got file size", 15,0);
	sscanf(buffer, "%d", &size);
	return size;
}

void listenToClients(int sock, int bufsocket, int clilen,
		struct sockaddr_in clientAddr, char *name, int fs) {
	getFirstMessage(bufsocket, name);
	if (strcmp(name, "ls") == 0) {
		system("dir>ls");
		FILE *ls = fopen("ls", "r");
		char charSize[30];
		char lsStr[30], buffer[30];
		while (!feof(ls)) {
			fgets(lsStr, sizeof(lsStr), ls);
			if (strcmp(lsStr, "ls\n") == 0 ) {
				continue;
			}
			send(bufsocket, lsStr, sizeof(lsStr),0);
			recv(bufsocket, buffer, 1,0);
			memset(lsStr,0,sizeof(lsStr));
		}
		send(bufsocket, "&", 2,0);
		fclose(ls);
		system("del ls");
		return;
	}
	else if (strcmp(name, "get") == 0) {
		char filename[30];
		char buffer[1024];
		recv(bufsocket, filename, sizeof(filename),0);
		send(bufsocket, filename, sizeof(filename),0);
		printf("%s is wanted\n", filename);
		FILE *file = fopen(filename, "r+");
		while (!feof(file)) {
			fread(buffer, 1, sizeof(buffer), file);
			send(bufsocket, buffer, sizeof(buffer),0);
			recv(bufsocket, buffer, sizeof(buffer),0);
		}
		send(bufsocket, "&", 1,0);
		recv(bufsocket, buffer, sizeof(buffer),0);
		fclose(file);
		return;
	}
	else if (strcmp(name, "mkdir") == 0) {
		char dirname[30];
		recv(bufsocket, dirname, sizeof(dirname),0);
		send(bufsocket, dirname, sizeof(dirname),0);
		printf("Directory %s was created\n", dirname);
		_mkdir(dirname);
		return;
	}
	else if (strcmp(name, "cd") == 0) {
		char dirname[30];
		recv(bufsocket, dirname, sizeof(dirname),0);
		send(bufsocket, dirname, sizeof(dirname),0);
		_chdir(dirname);
		return;
	}
	char rmfile[70];
	memset(rmfile,0,sizeof(rmfile));
	sscanf(name, "del %s", rmfile);
	system(rmfile);
	memset(rmfile,0,sizeof(rmfile));
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
	memset(buffer,0,bufSize);
	recv(bufsocket, user, sizeof(user),0);
	_chdir("SERVER");
	int ch = _chdir(user);
	if (ch == -1) {
		_mkdir(user);
		_chdir(user);
	}
	memset(user,0,sizeof(user));
	send(bufsocket, "I got user name", 15,0);
}

DWORD WINAPI startThread(void *in){
	struct SockParams *sp = (struct SockParams*) in;
	int fs=0;
	char buffer[bufSize];
	char *name = buffer;
	puts("Thread was created");
	//getUser(sp->sock, sp->bufsocket, sp->port, sp->clilen, sp->serverAddr, sp->clientAddr);
		while (1) {
			listenToClients(sp->sock, sp->bufsocket, sp->clilen, sp->clientAddr, name, fs);
		}

}

int main(int argc, char *argv[]) {
	//pthread_t mainthread;
	int i = 0;
	int j;
	struct SockParams sp;
	const int on = 1;
	if (WSAStartup(MAKEWORD(2,2),&sp.wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return 1;
    }
	    //Create a socket
    if((sp.sock = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d" , WSAGetLastError());
    }
	memset((char *) &sp.serverAddr,0, sizeof(sp.serverAddr));
	sp.port = 7777;
	sp.serverAddr.sin_family = AF_INET;
	sp.serverAddr.sin_addr.s_addr = INADDR_ANY;
	sp.serverAddr.sin_port = htons(sp.port);
	if (bind(sp.sock, (struct sockaddr *) &sp.serverAddr, sizeof(sp.serverAddr)) == SOCKET_ERROR) {
		printf("Bind failed with error code : %d" , WSAGetLastError());
	}
	_mkdir("SERVER");
	_chdir("SERVER");
	while(1){
		listen(sp.sock, 5);
		sp.clilen = sizeof(sp.clientAddr);
		sp.bufsocket = accept(sp.sock,(struct sockaddr *) &sp.clientAddr, &sp.clilen);
		if (sp.bufsocket == INVALID_SOCKET)
		{
			printf("accept failed with error code : %d" , WSAGetLastError());
		 }
		h1 = CreateThread(NULL, 0, startThread, (void*)&sp, 0, NULL);
		startThread((void*)&sp);
	}	
	
    closesocket(sp.sock);
	CloseHandle(h1);
    WSACleanup();
	return 0;
}