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

#define BUFLEN 1024  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data

struct mySocket {
	struct sockaddr_in si_me, si_other;
	SOCKET sock; 
	int slen, recv_len;
	char buf[BUFLEN];
	WSADATA wsa;
};

void die(char *s) {
	perror(s);
	exit(1);
}

char* getFirstMessage(struct mySocket s) {
	if ((s.recv_len = recvfrom(s.sock, s.buf, BUFLEN, 0,
			(struct sockaddr *) &s.si_other, &s.slen)) == -1) {
		die("recvfrom()");
	}
	printf("Received packet from %s:%d\n", inet_ntoa(s.si_other.sin_addr), ntohs(s.si_other.sin_port));
	if (sendto(s.sock, s.buf, s.recv_len, 0, (struct sockaddr*) &s.si_other,
			s.slen) == -1) {
		die("sendto()");
	}
	return s.buf;
}

int main(void) {
	struct mySocket s;
	int i;
	   if (WSAStartup(MAKEWORD(2,2),&s.wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        exit(EXIT_FAILURE);
    }
	s.slen = sizeof(s.si_other);
	char *command = s.buf;

	if ((s.sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET) {
		 printf("Could not create socket : %d" , WSAGetLastError());
	}
	
			_mkdir("SERVER");
			_chdir("SERVER");

	memset((char *) &s.si_me, 0, sizeof(s.si_me));

	s.si_me.sin_family = AF_INET;
	s.si_me.sin_port = htons(PORT);
	s.si_me.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(s.sock, (struct sockaddr*) &s.si_me, sizeof(s.si_me)) == SOCKET_ERROR) {
		die("bind");
	}
	while (1) {
		memset(s.buf, 0, sizeof(s.buf));
		command = getFirstMessage(s);
		if (strcmp(command, "transfer") == 0) {
			puts("transfer");
			recvfrom(s.sock, s.buf, BUFLEN, 0, (struct sockaddr *) &s.si_other,
					&s.slen);
			sendto(s.sock, s.buf, BUFLEN, 0, (struct sockaddr*) &s.si_other,
					s.slen);
			printf("%s\n", s.buf);
			FILE *f = fopen(s.buf, "ab");
			memset(s.buf,0, sizeof(s.buf));
			while (strcmp(s.buf, "&") != 0) {
				recvfrom(s.sock, s.buf, BUFLEN, 0,
						(struct sockaddr *) &s.si_other, &s.slen);
				sendto(s.sock, s.buf, BUFLEN, 0, (struct sockaddr*) &s.si_other,
						s.slen);
				if (strcmp(s.buf, "&") != 0) {
					fprintf(f, "%s", s.buf);
				}
			}
			fclose(f);
		} else if (strcmp(command, "ls") == 0) {
			system("dir>ls");
			FILE *ls = fopen("ls", "r");
			char charSize[30];
			char lsStr[30], buffer[30];
			while (!feof(ls)) {
				fgets(lsStr, sizeof(lsStr), ls);
				if (strcmp(lsStr, "ls\n") == 0) {
					continue;
				}
				recvfrom(s.sock, s.buf, BUFLEN, 0,
						(struct sockaddr *) &s.si_other, &s.slen);
				sendto(s.sock, lsStr, BUFLEN, 0, (struct sockaddr*) &s.si_other,
						s.slen);
				recvfrom(s.sock, s.buf, BUFLEN, 0,
						(struct sockaddr *) &s.si_other, &s.slen);
				sendto(s.sock, lsStr, BUFLEN, 0, (struct sockaddr*) &s.si_other,
						s.slen);
				memset(lsStr,0, sizeof(lsStr));
			}
			sendto(s.sock, "&", BUFLEN, 0, (struct sockaddr*) &s.si_other,
					s.slen);
			recvfrom(s.sock, s.buf, BUFLEN, 0, (struct sockaddr *) &s.si_other,
					&s.slen);
			fclose(ls);
			system("del ls");
		} else if (strcmp(command, "get") == 0) {
			recvfrom(s.sock, s.buf, BUFLEN, 0, (struct sockaddr *) &s.si_other,
					&s.slen);
			sendto(s.sock, s.buf, BUFLEN, 0, (struct sockaddr*) &s.si_other,
					s.slen);
			FILE *f = fopen(s.buf, "r+");
			while (!feof(f)) {
				memset(s.buf,0,BUFLEN);
				recvfrom(s.sock, s.buf, BUFLEN, 0,
						(struct sockaddr *) &s.si_other, &s.slen);
				fread(s.buf,1, sizeof(s.buf), f);
				sendto(s.sock, s.buf, BUFLEN, 0, (struct sockaddr*) &s.si_other,
						s.slen);
			}
			recvfrom(s.sock, s.buf, BUFLEN, 0, (struct sockaddr *) &s.si_other,
					&s.slen);
			sendto(s.sock, "&", BUFLEN, 0, (struct sockaddr*) &s.si_other,
					s.slen);
			memset(s.buf,0,BUFLEN);
			fclose(f);
		} else if (strcmp(command, "cd") == 0) {
			recvfrom(s.sock, s.buf, BUFLEN, 0, (struct sockaddr *) &s.si_other,
								&s.slen);
			sendto(s.sock, s.buf, BUFLEN, 0, (struct sockaddr*) &s.si_other,
								s.slen);
			chdir(s.buf);
			memset(s.buf,0,BUFLEN);
		} else if (strcmp(command, "mkdir") == 0) {
			recvfrom(s.sock, s.buf, BUFLEN, 0, (struct sockaddr *) &s.si_other,
								&s.slen);
			sendto(s.sock, s.buf, BUFLEN, 0, (struct sockaddr*) &s.si_other,
								s.slen);
			printf("Directory %s was created\n", s.buf);
			_mkdir(s.buf);
			memset(s.buf,0,BUFLEN);
		} else if (strcmp(command, "close") == 0) {
			exit(1);
		}

	}
	close(s.sock);
	return 0;
}
