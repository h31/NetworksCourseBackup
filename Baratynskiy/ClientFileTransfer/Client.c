#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define bufSize 1024

int main(int argc, char *argv[]) {
	int sock, port, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	long size;
	int b;
	char fileName[30], user[30];
	char commandLine[15];
	char buffer[bufSize];
	char charSize[bufSize];
	int i = 0;
	int firstTime = 1;

	if (argc < 3) {
		fprintf(stderr, "usage %s hostname port\n", argv[0]);
		exit(0);
	}
	port = atoi(argv[2]);
	printf("User:");
	scanf("%s", user);
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("ERROR opening socket");
		exit(1);
	}
	server = gethostbyname(argv[1]);
	if (server == NULL) {
		fprintf(stderr, "ERROR, no such host\n");
		exit(0);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	if (connect(sock, &serv_addr, sizeof(serv_addr)) < 0) {
		perror("ERROR connecting");
		exit(1);
	}
	write(sock, user, sizeof(user));
	int ch = chdir(user);
	if (ch == -1) {
		mkdir(user, S_IRWXU);
		chdir(user);
	}
	bzero(user, sizeof(user));
	read(sock, buffer, bufSize - 1);
	bzero(buffer, bufSize);
	while (1) {
		scanf("%s", commandLine);
		if (strcmp(commandLine, "transfer") == 0) {
			firstTime = 1;
			printf("Enter file name: ");
			scanf("%s", fileName);
			FILE *file = fopen(fileName, "r+");
			fseek(file, 0, SEEK_END);
			size = ftell(file);
			sprintf(charSize, "%d", size);
			fseek(file, 0, SEEK_SET);
			/* Create a socket point */
			while (!feof(file)) {
				if (firstTime == 0) {
					b = fread(buffer, 1, sizeof(buffer), file);
				}
				if (firstTime == 1) {
					write(sock, fileName, strlen(fileName));
					firstTime = 2;
					bzero(buffer, bufSize);
					n = read(sock, buffer, bufSize - 1);
					printf("%s\n", buffer);
					if (n < 0) {
						perror("ERROR reading from socket");
						exit(1);
					}
					continue;
				} else if (firstTime == 2) {
					write(sock, charSize, strlen(charSize));
					firstTime = 0;
					bzero(buffer, bufSize);
					n = read(sock, buffer, bufSize - 1);
					printf("%s\n", buffer);
					if (n < 0) {
						perror("ERROR reading from socket");
						exit(1);
					}
					continue;

				}
				n = write(sock, buffer, strlen(buffer));
				if (n < 0) {
					perror("ERROR writing to socket");
					exit(1);
				}

				/* Now read server response */
				bzero(buffer, bufSize);
				n = read(sock, buffer, bufSize - 1);
				if (n < 0) {
					perror("ERROR reading from socket");
					exit(1);
				}
				printf("%s\n", buffer);
			}
			fclose(file);
		} else if (strcmp(commandLine, "ls") == 0) {
			write(sock, "ls", 2);
			bzero(buffer, bufSize);
			read(sock, buffer, bufSize - 1);
			while (1) {
				bzero(buffer, bufSize);
				read(sock, buffer, bufSize - 1);
				if (strcmp(buffer, "&") == 0) {
					break;
				}
				printf("%s", buffer);
				write(sock, "o", 1);
			}

		} else if (strcmp(commandLine, "get") == 0) {
			char filename[30];
			printf("Get ");
			scanf("%s", filename);
			write(sock, "get", 3);
			bzero(buffer, bufSize);
			read(sock, buffer, bufSize - 1);
			write(sock, filename, sizeof(filename));
			FILE *f = fopen(filename, "ab");
			bzero(buffer, bufSize);
			read(sock, buffer, bufSize - 1);
			while (1) {
				bzero(buffer, bufSize);
				read(sock, buffer, bufSize - 1);
				if (strcmp(buffer, "&") == 0) {
					fclose(f);
					break;
				} else {
					fprintf(f, "%s", buffer);
					write(sock, "m", 1);
				}
			}

		} else if (strcmp(commandLine, "mkdir") == 0) {
			char dirname[30];
			printf("mkdir ");
			scanf("%s", dirname);
			write(sock, "mkdir", 5);
			bzero(buffer, bufSize);
			read(sock, buffer, bufSize - 1);
			write(sock, dirname, sizeof(dirname));
			bzero(buffer, bufSize);
			read(sock, buffer, bufSize - 1);
			bzero(buffer, bufSize);

		} else if (strcmp(commandLine, "cd") == 0) {
			char dirname[30];
			printf("cd ");
			scanf("%s", dirname);
			write(sock, "cd", 2);
			bzero(buffer, bufSize);
			read(sock, buffer, bufSize - 1);
			write(sock, dirname, sizeof(dirname));
			bzero(buffer, bufSize);
			read(sock, buffer, bufSize - 1);
			bzero(buffer, bufSize);
		} else if (strcmp(commandLine, "close") == 0) {
			return 0;
		}
	}
	return 0;
}
