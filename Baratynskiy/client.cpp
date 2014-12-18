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

int main(int argc, char *argv[]) {
	WSADATA wsa;
	SOCKET sock;
	int port, n;
	struct sockaddr_in serv_addr;
	long size;
	int b;
	char fileName[30], user[30];
	char commandLine[15];
	char buffer[bufSize];
	char charSize[bufSize];
	int i = 0;
	int firstTime = 1;

	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return 1;
    }
	printf("User:");
	scanf("%s", user);
	if((sock = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d" , WSAGetLastError());
    }
	memset((char *) &serv_addr,0, sizeof(serv_addr));
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(7777);
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		perror("ERROR connecting");
		exit(1);
	}
	int ch = _chdir(user);
	if (ch == -1) {
		_mkdir(user);
		_chdir(user);
	}
	memset(user, 0,sizeof(user));
	memset(buffer,0, bufSize);
	while (1) {
		scanf("%s", commandLine);
		if (strcmp(commandLine, "transfer") == 0) {
			firstTime = 1;
			printf("Enter file name: ");
			scanf("%s", fileName);
			system("dir /B>ls");
			FILE *ls = fopen("ls", "r");
			int wasFile = 0;
			char charSize[30];
			char lsStr[30], newname[30];
			//sscanf(fileName, "%s\n",buffer);
			while (!feof(ls)) {
				fgets(lsStr, sizeof(lsStr), ls);
				int j;
				memset(newname,0,sizeof(newname));
				for (j=0;j<30;j++){
					if (lsStr[j]!='\n' ){
						newname[j] = lsStr[j];
					}
					else break;
				}
				if (strcmp(newname,fileName)== 0){
					wasFile = 1;
					break;
				}
				memset(lsStr, 0, sizeof(lsStr));
				memset(newname,0, sizeof(newname));
			}
			fclose(ls);
			system("del ls");
			if (wasFile == 0){
					puts("There is no such file!");
					continue;
			}
			FILE *file = fopen(fileName, "r+");
			fseek(file, 0, SEEK_END);
			size = ftell(file);
			if (size > 1073741824){
				printf("This file is too big!!!\n");
				continue;
			}
			sprintf(charSize, "%d", size);
			fseek(file, 0, SEEK_SET);
			/* Create a socket point */
			while (!feof(file)) {
				if (firstTime == 0) {
					b = fread(buffer, 1, sizeof(buffer), file);
				}
				if (firstTime == 1) {
					send(sock, fileName, strlen(fileName),0);
					firstTime = 2;
					memset(buffer,0, bufSize);
					recv(sock, buffer, bufSize - 1,0);
					printf("%s\n", buffer);
					/*if (n < 0) {
						perror("ERROR recving from socket");
						exit(1);
					}*/
					continue;
				} else if (firstTime == 2) {
					send(sock, charSize, strlen(charSize),0);
					firstTime = 0;
					memset(buffer,0, bufSize);
					recv(sock, buffer, bufSize - 1,0);
					printf("%s\n", buffer);
					/*if (n < 0) {
						perror("ERROR recving from socket");
						exit(1);
					}*/
					continue;

				}
				n = send(sock, buffer, strlen(buffer),0);
				if (n < 0) {
					perror("ERROR writing to socket");
					exit(1);
				}

				/* Now recv server response */
				memset(buffer,0, bufSize);
				n = recv(sock, buffer, bufSize - 1,0);
				if (n < 0) {
					perror("ERROR recving from socket");
					exit(1);
				}
				printf("%s\n", buffer);
			}
			fclose(file);
		} else if (strcmp(commandLine, "ls") == 0) {
			send(sock, "ls", 2,0);
			memset(buffer,0, bufSize);
			recv(sock, buffer, bufSize - 1,0);
			while (1) {
				memset(buffer,0, bufSize);
				recv(sock, buffer, bufSize - 1,0);
				if (strcmp(buffer, "&") == 0) {
					break;
				}
				printf("%s", buffer);
				send(sock, "o", 1,0);
			}

		} else if (strcmp(commandLine, "get") == 0) {
			char filename[30];
			printf("Get ");
			scanf("%s", filename);
			send(sock, "get", 3,0);
			memset(buffer,0, bufSize);
			recv(sock, buffer, bufSize - 1,0);
			send(sock, filename, sizeof(filename),0);
			FILE *f = fopen(filename, "ab");
			memset(buffer,0, bufSize);
			recv(sock, buffer, bufSize - 1,0);
			while (1) {
				memset(buffer,0, bufSize);
				recv(sock, buffer, bufSize - 1,0);
				if (strcmp(buffer, "&") == 0) {
					fclose(f);
					break;
				} else {
					fprintf(f, "%s", buffer);
					send(sock, "m", 1,0);
				}
			}

		} else if (strcmp(commandLine, "_mkdir") == 0) {
			char dirname[30];
			printf("_mkdir ");
			scanf("%s", dirname);
			send(sock, "_mkdir", 5,0);
			memset(buffer, 0,bufSize);
			recv(sock, buffer, bufSize - 1,0);
			send(sock, dirname, sizeof(dirname),0);
			memset(buffer,0, bufSize);
			recv(sock, buffer, bufSize - 1,0);
			memset(buffer,0, bufSize);

		} else if (strcmp(commandLine, "cd") == 0) {
			char dirname[30];
			printf("cd ");
			scanf("%s", dirname);
			send(sock, "cd", 2,0);
			memset(buffer,0, bufSize);
			recv(sock, buffer, bufSize - 1,0);
			send(sock, dirname, sizeof(dirname),0);
			memset(buffer,0, bufSize);
			recv(sock, buffer, bufSize - 1,0);
			memset(buffer,0, bufSize);
		} else if (strcmp(commandLine, "close") == 0) {
			return 0;
		}
	}
	return 0;
}
