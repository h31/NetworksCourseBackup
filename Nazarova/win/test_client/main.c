// Module Name: Client.c
//
// Description:
//    This sample is the echo client. It connects to the TCP server,
//    sends data, and reads data back from the server.
//
// Compile:
//    cl -o Client Client.c ws2_32.lib
//
// Command Line Options:
//    client [-p:x] [-s:IP] [-n:x] [-o]
//           -p:x      Remote port to send to
//           -s:IP     Server's IP address or hostname
//           -n:x      Number of times to send message
//           -o        Send messages only; don't receive
//
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <libgen.h>

#define SIZE_CMD 5
#define SIZE_ARG 50
#define SIZE_STR 128
#define SIZE_BUF 1024
#define SUCCESS "000"
#define UNSUCCESS "111"
#define SIZE_CONTENT 4096
#define DEFAULT_PORT 5001

int port = DEFAULT_PORT;
bool binterface = 0;
char szAddress[SIZE_STR];

int recv_report(SOCKET sock);
void send_report(SOCKET sock, char *status);
void ValidateArgs(int argc, char **argv);
void usage();
void output(char *buffer);


int main(int argc, char **argv)
{
	WSADATA wsd;
	SOCKET sock;
	int msg_size;
	char name[SIZE_STR];
	char path[SIZE_BUF];
	char author[SIZE_STR];
	char buffer[SIZE_BUF];
	char command[SIZE_CMD];
	char content[SIZE_CONTENT];
	struct sockaddr_in client;

	ValidateArgs(argc, argv);
	if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
	{
		printf("Failed to load Winsock library!\n");
		return 1;
	}
	if (binterface)
		{
		client.sin_addr.s_addr = inet_addr(szAddress);
			if (client.sin_addr.s_addr == INADDR_NONE)
				usage();
		}
		else
			client.sin_addr.s_addr = htonl(INADDR_ANY);
	client.sin_family = AF_INET;
	client.sin_port = htons(port);

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
	{
		printf("Create socket failed: %d\n", WSAGetLastError());
		exit(1);
	}
	if (connect(sock, (struct sockaddr *)&client, sizeof(client))  == SOCKET_ERROR)
	{
		printf("Connect failed: %d\n", WSAGetLastError());
		exit(1);
	}
	memset(buffer, 0, sizeof(buffer));
	while(strcmp(buffer, ":start"))
	{

		fgets(buffer, sizeof(buffer), stdin);
		if (buffer[strlen(buffer) - 1] == '\n')
			buffer[strlen(buffer) - 1] = '\0';
		if ((msg_size = send(sock, buffer, strlen(buffer), 0)) == SOCKET_ERROR)
		{
			printf("SEND start message failed: %d\n", WSAGetLastError());
			exit(1);
		}
	}
	//printf("SEND  [%d bytes]: start message '%s'\n", msg_size, buffer);

	memset(buffer, 0, sizeof(buffer));
	if ((msg_size = recv(sock, buffer, sizeof(buffer), 0)) == SOCKET_ERROR)
	{
		printf("RECV directory content failed: %d\n", WSAGetLastError());
		exit(1);
	}
	//printf("RECV  [%d bytes]: directory content\n", msg_size);
	output(buffer);
	send_report(sock, SUCCESS);

	while(1)
	{
		memset(path, 0, sizeof(path));
		if ((msg_size = recv(sock, path, sizeof(path), 0)) == SOCKET_ERROR)
		{
			printf("RECV current path failed: %d\n", WSAGetLastError());
			exit(1);
		}
		//printf("RECV  [%d bytes]: current path '%s'\n", msg_size, path);

		send_report(sock, SUCCESS);

		memset(buffer, 0, sizeof(buffer));
		if ((msg_size = recv(sock, buffer, sizeof(buffer), 0)) == SOCKET_ERROR)
		{
			printf("RECV invitation message failed: %d\n", WSAGetLastError());
			exit(1);
		}
		//printf("RECV  [%d bytes]: invitation message\n", msg_size);
		output(buffer);
		char space;
		memset(name, 0, sizeof(name));
		memset(buffer, 0, sizeof(buffer));
		memset(author, 0, sizeof(author));
		memset(command, 0, sizeof(command));
		memset(content, 0, sizeof(content));
		scanf("%5s%1c", command, &space);
		if ((msg_size = send(sock, command, strlen(command), 0)) == SOCKET_ERROR)
		{
			printf("SEND command failed: %d\n", WSAGetLastError());
			exit(1);
		}
		//printf("SEND  [%d bytes]: command '%s'\n", msg_size, path);

		if (!strcmp(command, ":exit"))
		{
			memset(buffer, 0, sizeof(buffer));
			if ((msg_size = recv(sock, buffer, sizeof(buffer), 0)) == SOCKET_ERROR)	// Receive the content of file
			{
				printf("RECV file or directory content failed: %d\n", WSAGetLastError());
				exit(1);
			}
			//printf("RECV  [%d bytes]: file or directory content\n", msg_size);
			output(buffer);
			break;
		}

		if (recv_report(sock) < 0)
		{
			puts("!No such command");
			send_report(sock, SUCCESS);
		}

		if (!strcmp(command, "add"))
		{
			char str[SIZE_ARG];
			fgets(name, sizeof(name), stdin);
			if (name[strlen(name) - 1] == '\n')
				name[strlen(name) - 1] = '\0';
			if ((msg_size = send(sock, name, strlen(name), 0)) == SOCKET_ERROR)
			{
				printf("SEND command failed: %d\n", WSAGetLastError());
				exit(1);
			}
			//printf("SEND  [%d bytes]: title of article '%s'\n", msg_size, name);
			if (recv_report(sock) < 0)
			{
				puts("!Such file already exist");
				send_report(sock, SUCCESS);
			}
			else
			{
				int length = sizeof(content) - sizeof(author) - sizeof(name);
				printf("Input author: ");
				fgets(author, sizeof(author), stdin);
				if (author[strlen(author) - 1] == '\n')
					author[strlen(author) - 1] = '\0';
				printf("name's read: %s [%d bytes]\n", name, msg_size);
				printf("author's read: %s [%d bytes]\n", author, msg_size);

				puts("Put content:");
				printf("[%d of %d]  ", (strlen(content)+strlen(str)), length);
				while (fgets(str, sizeof(str), stdin) != NULL)
				{
					if (!strncmp(":end", str, strlen(":end")))
						break;
					if ((strlen(content)+strlen(str)) > length)
					{
						puts("!Text size will not allow");
						memset(str, 0, strlen(str));
						printf("[%d of %d]  ",(strlen(content)+strlen(str)), length );
					}
					strcat(content, str);
					memset(str, 0, strlen(str));
				}
				if ((msg_size = send(sock, author, strlen(author), 0)) == SOCKET_ERROR)
				{
					printf("SEND author of article failed: %d\n", WSAGetLastError());
					exit(1);
				}
				//printf("SEND  [%d bytes]: author of article '%s'\n", msg_size, author);
				recv_report(sock);

				if ((msg_size = send(sock, content, strlen(content), 0)) == SOCKET_ERROR)
				{
					printf("SEND file content failed: %d\n", WSAGetLastError());
					exit(1);
				}
				//printf("SEND  [%d bytes]: file content '%s'\n", msg_size, content);
				if (recv_report(sock) < 0)
					puts("!Such file already exist");
				send_report(sock, SUCCESS);
			}
		}

			gets(buffer);
			if (!strcmp(command, "open"))
			{
				strcat(path, buffer);
				if ((msg_size = send(sock, path, strlen(path), 0)) == SOCKET_ERROR)
				{
					printf("SEND full path to file failed: %d\n", WSAGetLastError());
					exit(1);
				}
				//printf("SEND  [%d bytes]: full path to file '%s'\n", msg_size, path);
			}
			else if (!strcmp(command, "find"))
			{
				if ((msg_size = send(sock, buffer, strlen(buffer), 0))== SOCKET_ERROR)
				{
					printf("SEND author to find failed: %d\n", WSAGetLastError());
					exit(1);
				}
				//printf("SEND  [%d bytes]: author to find '%s'\n", msg_size, buffer);
			}

			memset(content, 0, sizeof(content));
			if ((msg_size = recv(sock, content, sizeof(content), 0)) == SOCKET_ERROR)	// Receive the content of file
			{
				printf("RECV file or directory content failed: %d\n", WSAGetLastError());
				exit(1);
			}
			//printf("RECV  [%d bytes]: file or directory content\n", msg_size);
			output(content);

			send_report(sock, SUCCESS);

		}

	closesocket(sock);

	WSACleanup();
	return 0;
}

void ValidateArgs(int argc, char **argv)
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
				binterface = 1;
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

void output(char *buffer)
{
	int i;
	for (i = 0; i < strlen(buffer); i++)
		if (buffer[i] != '|')
			printf("%c", buffer[i]);
		else
			printf("\n");
	if (buffer[strlen(buffer) - 1] == '\n')
		buffer[strlen(buffer) - 1] = '\0';
}

int recv_report(SOCKET sock)
{
	char status[SIZE_CMD];
	int msg_size;
	memset(status, 0, sizeof(status));
	if ((msg_size = recv(sock, status, sizeof(status), 0)) == SOCKET_ERROR)
	{
		perror("RECV report message failed");
		printf("connect() failed: %d\n", WSAGetLastError());
		exit(1);
	}
	//printf("RECV  [%d bytes]: report message  '%s'\n", msg_size, status);
	return (!strcmp(status, SUCCESS) ? 0 : -1);
}

void send_report(SOCKET sock, char *status)
{
	int msg_size;
	if ((msg_size = send(sock, status, sizeof(status), 0)) == SOCKET_ERROR)
	{
		perror("SEND report message failed");
		printf("connect() failed: %d\n", WSAGetLastError());
		exit(1);
	}
	//printf("SEND  [%d bytes]: report message  '%s'\n", msg_size, status);

}
