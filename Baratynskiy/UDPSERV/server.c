#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUFLEN 1024  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data

struct mySocket {
	struct sockaddr_in si_me, si_other;
	int sock, slen, recv_len;
	char buf[BUFLEN];

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
	if (sendto(s.sock, s.buf, s.recv_len, 0, (struct sockaddr*) &s.si_other,
			s.slen) == -1) {
		die("sendto()");
	}
	return s.buf;
}

int main(void) {
	struct mySocket s;
	s.slen = sizeof(s.si_other);
	char *command = s.buf;
	if ((s.sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		die("socket");
	}

	memset((char *) &s.si_me, 0, sizeof(s.si_me));

	s.si_me.sin_family = AF_INET;
	s.si_me.sin_port = htons(PORT);
	s.si_me.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(s.sock, (struct sockaddr*) &s.si_me, sizeof(s.si_me)) == -1) {
		die("bind");
	}
	while (1) {
		bzero(s.buf, sizeof(s.buf));
		command = getFirstMessage(s);
		if (strcmp(command, "transfer") == 0) {
			puts("transfer");
			recvfrom(s.sock, s.buf, BUFLEN, 0, (struct sockaddr *) &s.si_other,
					&s.slen);
			sendto(s.sock, s.buf, BUFLEN, 0, (struct sockaddr*) &s.si_other,
					s.slen);
			printf("%s\n", s.buf);
			FILE *f = fopen(s.buf, "ab");
			bzero(s.buf, sizeof(s.buf));
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
			system("ls>ls");
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
				bzero(lsStr, sizeof(lsStr));
			}
			sendto(s.sock, "&", BUFLEN, 0, (struct sockaddr*) &s.si_other,
					s.slen);
			recvfrom(s.sock, s.buf, BUFLEN, 0, (struct sockaddr *) &s.si_other,
					&s.slen);
			fclose(ls);
			system("rm ls");
		} else if (strcmp(command, "get") == 0) {
			recvfrom(s.sock, s.buf, BUFLEN, 0, (struct sockaddr *) &s.si_other,
					&s.slen);
			sendto(s.sock, s.buf, BUFLEN, 0, (struct sockaddr*) &s.si_other,
					s.slen);
			FILE *f = fopen(s.buf, "r+");
			while (!feof(f)) {
				bzero(s.buf,BUFLEN);
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
			bzero(s.buf,BUFLEN);
			fclose(f);
		} else if (strcmp(command, "close") == 0) {
			exit(1);
		}

	}
	close(s.sock);
	return 0;
}
