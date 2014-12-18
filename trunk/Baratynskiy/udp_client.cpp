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
#define SERVER "127.0.0.1"
#define BUFLEN 1024  //Max length of buffer
#define PORT 8888   //The port on which to send data

void die(char *s) {
	perror(s);
	exit(1);
}

int main(void) {
	struct sockaddr_in si_other;
	SOCKET sock;
	WSADATA wsa;
	int  slen = sizeof(si_other);
	char message[BUFLEN];
	char user[30];
	    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        exit(EXIT_FAILURE);
    }
	printf("User:");
	scanf("%s", user);
	int ch = _chdir(user);
	if (ch == -1) {
		_mkdir(user);
		_chdir(user);
	}
	memset(user,0, sizeof(user));
	    if ( (sock=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
    {
        printf("socket() failed with error code : %d" , WSAGetLastError());
        exit(EXIT_FAILURE);
    }

	memset((char *) &si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(PORT);
    si_other.sin_addr.S_un.S_addr = inet_addr(SERVER);

	while (1) {
		memset(message,0, sizeof(message));
		scanf("%s", message);

		if (sendto(sock, message, strlen(message), 0,
				(struct sockaddr *) &si_other, slen) == -1) {
			die("sendto()");
		}

		memset(message,0, sizeof(message));
		if (recvfrom(sock, message, BUFLEN, 0, (struct sockaddr *) &si_other,
				&slen) == -1) {
			die("recvfrom()");
		}
		if (strcmp(message, "transfer") == 0) {
			memset(message,0, sizeof(message));
			scanf("%s", message);
			system("dir /B>ls");
			FILE *ls = fopen("ls", "r");
			int wasFile = 0;
			char charSize[30];
			char lsStr[30], newname[30];
			while (!feof(ls)) {
				fgets(lsStr, sizeof(lsStr), ls);
				int j;
				memset(newname,0, sizeof(newname));
				for (j = 0; j < 30; j++) {
					if (lsStr[j] != '\n') {
						newname[j] = lsStr[j];
					} else
						break;
				}
				if (strcmp(newname, message) == 0) {
					wasFile = 1;
					break;
				}
				memset(lsStr, 0,sizeof(lsStr));
				memset(newname,0, sizeof(newname));
			}
			fclose(ls);
			system("del ls");
			if (wasFile == 0) {
				puts("There is no such file!");
				continue;
			}
			sendto(sock, message, strlen(message), 0,
					(struct sockaddr *) &si_other, slen);
			recvfrom(sock, message, BUFLEN, 0, (struct sockaddr *) &si_other,
					&slen);
			FILE *file = fopen(message, "r+");
			while (!feof(file)) {
				memset(message,0, sizeof(message));
				fread(message, 1, sizeof(message), file);
				sendto(sock, message, strlen(message), 0,
						(struct sockaddr *) &si_other, slen);
				recvfrom(sock, message, BUFLEN, 0,
						(struct sockaddr *) &si_other, &slen);
			}
			fclose(file);
			sendto(sock, "&", BUFLEN, 0, (struct sockaddr *) &si_other, slen);
			recvfrom(sock, message, BUFLEN, 0, (struct sockaddr *) &si_other,
					&slen);
		}
		if (strcmp(message, "ls") == 0) {
			while (strcmp(message, "&") != 0) {
				memset(message, 0,sizeof(message));
				sendto(sock, message, BUFLEN, 0, (struct sockaddr *) &si_other,
						slen);
				recvfrom(sock, message, BUFLEN, 0,
						(struct sockaddr *) &si_other, &slen);
				if (strcmp(message, "&") != 0)
					printf("%s", message);
				sendto(sock, message, BUFLEN, 0, (struct sockaddr *) &si_other,
						slen);
				recvfrom(sock, message, BUFLEN, 0,
						(struct sockaddr *) &si_other, &slen);
			}
		}
		if (strcmp(message, "get") == 0) {
			memset(message, 0,sizeof(message));
			printf("get ");
			scanf("%s", message);
			sendto(sock, message, BUFLEN, 0, (struct sockaddr *) &si_other,
					slen);
			FILE *f = fopen(message, "ab");
			recvfrom(sock, message, BUFLEN, 0, (struct sockaddr *) &si_other,
					&slen);
			while (strcmp(message, "&") != 0) {
				sscanf("message", "%s", message);
				sendto(sock, message, BUFLEN, 0, (struct sockaddr *) &si_other,
						slen);
				recvfrom(sock, message, BUFLEN, 0,
						(struct sockaddr *) &si_other, &slen);
				if (strcmp(message, "&") != 0) {
					fprintf(f, "%s", message);
				} else
					break;
				memset(message,0, sizeof(message));
			}
			fclose(f);
		}
		if (strcmp(message, "cd") == 0) {
			printf("cd ");
			scanf("%s", message);
			sendto(sock, message, BUFLEN, 0, (struct sockaddr *) &si_other,
					slen);
			recvfrom(sock, message, BUFLEN, 0, (struct sockaddr *) &si_other,
					&slen);
			memset(message,0, sizeof(message));
		}
		if (strcmp(message, "mkdir") == 0) {
			printf("mkdir ");
			scanf("%s", message);
			sendto(sock, message, BUFLEN, 0, (struct sockaddr *) &si_other,
					slen);
			recvfrom(sock, message, BUFLEN, 0, (struct sockaddr *) &si_other,
					&slen);
			memset(message,0, sizeof(message));
		}
		if (strcmp(message, "close") == 0) {
			return 0;
		}
	}

	close(sock);
	return 0;
}