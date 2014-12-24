/*
 * main.c
 *
 *  Created on: Nov 7, 2014
 *      Author: user
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <libgen.h>
#include <pthread.h>
#include <errno.h>
#include <stdbool.h>
#include "article.h"


#define DEFAULT_PORT 5001
#define SIZE_CMD 5
#define SIZE_BUF 1024
#define SIZE_CONTENT 4096
#define SIZE_STR 128
#define MAX_CONNECT 3
#define SUCCESS "000"
#define UNSUCCESS "111"
#define START_PATH "/home/ks/workspace/InformationSystem/Information System/"

pthread_t t1;
int port = DEFAULT_PORT;
bool interface = 0;
char szAddress[SIZE_STR];

int send_content(int sock, char *dir_name);
int open_file(int sock, char *path);
void sendPath_recvReport(int sock, char *path);
void *pthread_handler(void *sock);
void send_input_error(int sock);
void send_report(int sock, char *status);
void recv_report(int sock);
void validateArgs(int argc, char **argv);
void usage();

int main(int argc, char **argv)
{
	const int on = 1;
	int sock, newsock, client;
	struct sockaddr_in server, client_addr;
	validateArgs(argc, argv);

	if (interface)
	{
		server.sin_addr.s_addr = inet_addr(szAddress);
		if (server.sin_addr.s_addr == INADDR_NONE)
			usage();
	}
	else
		server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Socket is not created");
		exit(1);
	}
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	if (bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("Socket is not bound");
		exit(1);
	}

	if (listen(sock, 5) < 0)
	{
		perror("Listening error");
		exit(1);
	}

	int i = 0;
	while(1)
	{
		client = sizeof(client_addr);
		if((newsock = accept(sock, (struct sockaddr *)&client_addr, (socklen_t *)&client)) < 0)
		{
			perror("Accepting error");
			exit(1);
		}
		pthread_create(&t1, NULL, pthread_handler, (void *)newsock);
		i++;
	}

	for(; i > 0; i--)
		pthread_join(t1, NULL);
	close(sock);
	return 0;
}

void *pthread_handler(void *newsock)
{
	int sock = (int) newsock, msg_size;
	const char *invite_msg = ">   ";
	const char *exit_msg = "Bye-bye!!!|";
	char path[SIZE_BUF];
	char name[SIZE_STR];
	char buffer[SIZE_BUF];
	char author[SIZE_STR];
	char content[SIZE_CONTENT];

	while(strcmp(buffer, ":start"))
	{
		bzero(buffer, sizeof(buffer));
		if ((msg_size = recv(sock, buffer, sizeof(buffer), 0)) < 0)
		{
			perror("RECV start message error");
			exit(1);
		}
	}
	printf("RECV  [%d bytes]: start message '%s'\n", msg_size, buffer);
	send_content(sock, START_PATH);
	strcpy(path, START_PATH);

	while(1)
	{
		if ((msg_size = send(sock, invite_msg, strlen(invite_msg), 0)) < 0)
		{
			perror("SEND invitation message error");
			exit(1);
		}
		printf("SEND  [%d bytes]: invitation message '%s'\n", msg_size, invite_msg);
		bzero(buffer, sizeof(buffer));
		if ((msg_size = recv(sock, buffer, sizeof(buffer), 0)) < 0)
		{
			perror("RECV command error");
			exit(1);
		}
		printf("RECV  [%d bytes]: command '%s'\n", msg_size, buffer);

		if (!strcmp(buffer, ":exit"))
		{
			if ((msg_size = send(sock, exit_msg, strlen(exit_msg), 0)) < 0)
			{
				perror("SEND directory content error");
				exit(1);
			}
			printf("SEND  [%d bytes]: directory content '%s'\n", msg_size, exit_msg);
			break;
		}
		if (strcmp(buffer, "find") && strcmp(buffer, "open") && strcmp(buffer, "add"))
		{
			send_input_error(sock);
			send_content(sock, path);
			continue;
		}
		send_report(sock, SUCCESS);

		if (!strcmp(buffer, "open"))
		{
			bzero(path, sizeof(path));
			if ((msg_size = recv(sock, path, sizeof(path), 0)) < 0)
			{
				perror("RECV path to file error");
				exit(1);
			}
			printf("RECV  [%d bytes]: path to file message '%s'\n", msg_size, path);
			open_file(sock, path);
		}

		if (!strcmp(buffer, "find"))
		{
			bzero(author, sizeof(author));
			if ((msg_size = recv(sock, author, sizeof(author), 0)) < 0)
			{
				perror("RECV author to find error");
				exit(1);
			}
			printf("RECV  [%d bytes]: author to find '%s'\n", msg_size, author);

			find_for_author(sock, path, author);
			sendPath_recvReport(sock, path);
		}

		if (!strcmp(buffer, "add"))
		{
			bzero(name, sizeof(name));
			if ((msg_size = recv(sock, name, sizeof(name), 0)) < 0)
			{
				perror("RECV name error");
				exit(1);
			}
			printf("RECV  [%d bytes]: name'%s'\n", msg_size, name);
			char *dir = strdup(path);
			strcat(path, name);
			strcat(path, ".txt");
			if (check_file_existence(path) < 0)
			{
				send_report(sock, UNSUCCESS);
				recv_report(sock);
			}
			else
			{
				send_report(sock, SUCCESS);
				bzero(author, sizeof(author));
				if ((msg_size = recv(sock, author, sizeof(author), 0)) < 0)
				{
					perror("RECV author error");
					exit(1);
				}
				printf("RECV  [%d bytes]: author '%s'\n", msg_size, author);
				send_report(sock, SUCCESS);
				bzero(content, sizeof(content));
				if ((msg_size = recv(sock, content, sizeof(content), 0)) < 0)
				{
					perror("RECV content of file error");
					exit(1);
				}
				printf("RECV  [%d bytes]: content of file '%s'\n", msg_size, content);
				strcat(name, "\n");
				strcat(author, "\n\n");
				if (add_article(path, name, author, content) < 0)
					send_report(sock, UNSUCCESS);
				else
					send_report(sock, SUCCESS);
				recv_report(sock);
			}
			send_content(sock, dir);
			dirname(path);
			if (path[strlen(path) - 1] != '/')
				strcat(path, "/");
		}
	}
	pthread_exit(0);
	return 0;
}

void validateArgs(int argc, char **argv)
{
	int i;

	for(i = 1; i < argc; i++)
	{
		if ((argv[i][0] == '-') || (argv[i][0] == '/'))
		{
			switch (tolower(argv[i][1]))
			{
			case 'p':
				port = atoi(&argv[i][3]);
				break;
			case 'i':
				interface = 1;
				if (strlen(argv[i]) > 3)
					strcpy(szAddress, &argv[i][3]);
				break;
			default:
				usage();
				break;
			}
		}
	}
}

void usage()
{
	printf("usage: server [-p:x] [-i:IP]\n\n");
	printf(" -p:x Port number to listen on\n");
	printf(" -i:str Interface to listen on\n");
}

void send_input_error(int sock)
{
	send_report(sock, UNSUCCESS);
	recv_report(sock);
}

int send_content(int sock, char *dir_name)
{
	char buffer[SIZE_BUF];
	char *filename;
	const char *delimiter = "----------------------|";
	int msg_size;
	DIR *dir = opendir(dir_name);

	bzero(buffer,sizeof(buffer));
	if(dir)
	{
		struct dirent *ent;
		strcat(buffer, delimiter);
		while((ent = readdir(dir)) != NULL)
		{
			filename = ent->d_name;
			if (strcmp(filename, ".")==0)
				continue;
			strcat(filename, "|");
			strcat(buffer, filename);
		}
		closedir(dir);
		strcat(buffer, delimiter);
		if ((msg_size = send(sock, buffer, strlen(buffer), 0)) < 0)
		{
			perror("SEND directory content error");
			exit(1);
		}
		printf("SEND  [%d bytes]: directory content '%s'\n", msg_size, buffer);
		bzero(filename,sizeof(filename));
		strcpy(filename, dir_name);
		if (!strcmp(basename(filename), "."))
			dirname(dir_name);
		else if (!strcmp(basename(filename), ".."))
			dirname(dirname(dir_name));
	}
	else
	{
		const char *err_msg = "!No such file or directory|";
		if ((msg_size = send(sock, err_msg, strlen(err_msg), 0)) < 0)
		{
			perror("SEND no file or directory error");
			exit(1);
		}
		printf("SEND  [%d bytes]: no file or directory message '%s'\n", msg_size, buffer);
		dirname(dir_name);
	}

	sendPath_recvReport(sock, dir_name);
	return 0;
}

int open_file(int sock, char *path)
{
	FILE *fp;
	int msg_size;
	struct stat about_file;

	char buffer[SIZE_STR], text[SIZE_CONTENT];
	bzero(text, sizeof(text));
	bzero(buffer, sizeof(buffer));

	char *tmp = malloc(SIZE_BUF);
	strcpy(tmp, START_PATH);
	if (!strcmp(path, strcat(tmp, "..")))
	{
		send_content(sock, START_PATH);
		free(tmp);
		return 0;
	}
	free(tmp);
	if ((fp = fopen(path, "r")) == NULL)
	{
		perror("Opening of file error");
		send_content(sock, path);
		return -1;
	}

	fstat(fileno(fp), &about_file);
	if ((about_file.st_mode & S_IFMT) != S_IFDIR)
	{
		int ch, i;
		for (i = 0; i < (sizeof(text) - sizeof(char)) && (ch = getc(fp)) != EOF; i++)
		{
			if (ch == '\n')
				ch = '|';
			text[i] = ch;
		}
		strcat(text, "|");
		if ((msg_size = send(sock, text, strlen(text), 0)) < 0)
		{
			perror("SEND content of file error");
			exit(1);
		}
		printf("SEND  [%d bytes]: content of file\n", msg_size);

		dirname(path);
		sendPath_recvReport(sock, path);

	}
	else
		send_content(sock, path);
	fclose(fp);
	return 0;

}

void sendPath_recvReport(int sock, char *path)
{
	int msg_size;
	recv_report(sock);
	if (path[strlen(path) - 1] != '/')
		strcat(path, "/");
	if ((msg_size = send(sock, path, strlen(path), 0)) < 0)
	{
		perror("SEND current path error");
		exit(1);
	}
	printf("SEND  [%d bytes]: current path '%s'\n", msg_size, path);
	recv_report(sock);
}

void send_report(int sock, char *status)
{
	int msg_size;
	if ((msg_size = send(sock, status, strlen(status), 0)) < 0)
	{
		perror("SEND report message error");
		exit(1);
	}
	printf("SEND  [%d bytes]: report message '%s'\n", msg_size, status);
}


void recv_report(int sock)
{
	char status[SIZE_CMD];
	int msg_size;
	bzero(status, sizeof(status));
	if ((msg_size = recv(sock, status, sizeof(status), 0)) < 0)
	{
		perror("RECV report message failed");
		exit(1);
	}
	printf("RECV  [%d bytes]: report message  '%s'\n", msg_size, status);
}
