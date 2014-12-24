#include <winsock2.h>

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "article.h"

#define DEFAULT_PORT 5001
#define MAX_CONNECT  3
#define SIZE_CMD     5
#define SIZE_BUF     1024
#define SIZE_CONTENT 4096
#define SIZE_STR     128
#define SUCCESS      "000"
#define UNSUCCESS    "111"
#define START_PATH "C:/Users/Kseniya/workspace/server_udp/Information System/"

int port = DEFAULT_PORT;
bool binterface = 0;
char szAddress[SIZE_STR];

int send_content(SOCKET sock, char *dir_name, struct sockaddr_in *ptr_addr);
int open_file(SOCKET sock, char *path, struct sockaddr_in *ptr_addr);
void sendPath_recvReport(SOCKET sock, char *path, struct sockaddr_in *ptr_addr);
void send_input_error(SOCKET sock, struct sockaddr_in *ptr_addr);
void send_report(SOCKET sock, char *status, struct sockaddr_in *ptr_addr);
void recv_report(SOCKET sock, struct sockaddr_in *ptr_addr);
void validateArgs(int argc, char **argv);
DWORD WINAPI ClientThread(LPVOID lpParam);
void usage();

typedef struct
{
	SOCKET socket_fd;
	struct sockaddr_in *ptr_addr;
} P_socket;

int main(int argc, char **argv)
{
	WSADATA       wsd;
	SOCKET        sock, i;
	HANDLE        hThread;
	DWORD         dwThreadId;
	struct sockaddr_in server, addr[MAX_CONNECT];
	P_socket p_sock[MAX_CONNECT];
	const int on = 1;

	validateArgs(argc, argv);
	if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
	{
		printf("Failed to load Winsock!\n");
		return 1;
	}

	if (binterface)
	{
		server.sin_addr.s_addr = inet_addr(szAddress);
		if (server.sin_addr.s_addr == INADDR_NONE)
			usage();
	}
	else
		server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_family = AF_INET;
	server.sin_port = htons(port);


	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == SOCKET_ERROR)
	{
		printf("socket() failed: %d\n", WSAGetLastError());
		exit(1);
	}
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	for (i = 0; i < MAX_CONNECT; i++)
	{
		memset(&addr[i], 0, sizeof(addr[i]));
		p_sock[i].socket_fd = sock;
		p_sock[i].ptr_addr = &addr[i];
	}
	if (bind(sock, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("bind() failed: %d\n", WSAGetLastError());
		return 1;
	}

	for(i = 0;i < MAX_CONNECT;i++)
	{
		printf("Accepted client: %s:%d\n",	inet_ntoa(server.sin_addr), ntohs(addr[i].sin_port));
		if ((hThread = CreateThread(NULL, 0, ClientThread, (LPVOID)&p_sock[i], 0, &dwThreadId)) == NULL)
		{
			printf("CreateThread() failed: %d\n", GetLastError());
			break;
		}
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
	}
	closesocket(sock);

	WSACleanup();
	return 0;
}
DWORD WINAPI ClientThread(LPVOID lpParam)
{
	P_socket *data;
	data = (P_socket *) lpParam;
	struct sockaddr_in addr = *(data->ptr_addr);
    SOCKET sock = data->socket_fd;
	int len = sizeof(addr);
	int msg_size;
	const char *invite_msg = ">   ";
	const char *exit_msg = "Bye-bye!!!|";
	char path[SIZE_BUF];
	char name[SIZE_STR];
	char buffer[SIZE_BUF];
	char author[SIZE_STR];
	char content[SIZE_CONTENT];

	while(strcmp(buffer, ":start"))
	{
		memset(buffer, 0, sizeof(buffer));
		if ((msg_size = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&addr,  &len))== SOCKET_ERROR)
		{
			printf("Receive :start_msg failed: %d\n", WSAGetLastError());
			exit(1);
		}
	}
	buffer[msg_size] = '\0';
	send_content(sock, START_PATH, &addr);
	strcpy(path, START_PATH);

	while(1)
	{
		if ((msg_size = sendto(sock, invite_msg, strlen(invite_msg), 0,  (struct sockaddr*)&addr, sizeof(addr))) == SOCKET_ERROR)
		{
			printf("SEND invitation message error: %d\n", WSAGetLastError());
			exit(1);
		}
		printf("SEND  [%d bytes]: invitation message '%s'\n", msg_size, invite_msg);

		memset(buffer, 0, sizeof(buffer));
		if ((msg_size = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&addr, &len)) == SOCKET_ERROR)
		{
			printf("RECV command error: %d\n", WSAGetLastError());
			exit(1);
		}
		printf("RECV  [%d bytes]: command '%s'\n", msg_size, buffer);

		if (!strcmp(buffer, ":exit"))
		{
			if ((msg_size = sendto(sock, exit_msg, strlen(exit_msg), 0,  (struct sockaddr*)&addr, sizeof(addr))) == SOCKET_ERROR)
			{
				printf("SEND directory content error: %d\n", WSAGetLastError());
				exit(1);
			}
			printf("SEND  [%d bytes]: directory content '%s'\n", msg_size, exit_msg);
			break;
		}

		if (strcmp(buffer, "find") && strcmp(buffer, "open") && strcmp(buffer, "add"))
		{
			send_input_error(sock, &addr);
			send_content(sock, path, &addr);
			continue;
		}
		send_report(sock, SUCCESS, &addr);

		if (!strcmp(buffer, "open"))
		{
			memset(path, 0, sizeof(path));
			if ((msg_size = recvfrom(sock, path, sizeof(path), 0, (struct sockaddr*)&addr,  &len)) == SOCKET_ERROR)
			{
				printf("RECV path to file error: %d\n", WSAGetLastError());
				exit(1);
			}
			printf("RECV  [%d bytes]: path to file message '%s'\n", msg_size, path);
			open_file(sock, path, &addr);
		}

		if (!strcmp(buffer, "find"))
		{
			memset(author, 0,sizeof(author));
			if ((msg_size = recvfrom(sock, author, sizeof(author), 0, (struct sockaddr*)&addr, &len)) == SOCKET_ERROR)
			{
				printf("RECV author to find error: %d\n", WSAGetLastError());
				exit(1);
			}
			printf("RECV  [%d bytes]: author to find '%s'\n", msg_size, author);
			find_for_author(sock, path, author, &addr);
			sendPath_recvReport(sock, path, &addr);
		}

		if (!strcmp(buffer, "add"))
		{
			memset(name, 0,sizeof(name));
			if ((msg_size = recvfrom(sock, name, sizeof(name), 0, (struct sockaddr*)&addr,  &len)) == SOCKET_ERROR)
			{
				printf("RECV name error: %d\n", WSAGetLastError());
				exit(1);
			}
			printf("RECV  [%d bytes]: name'%s'\n", msg_size, name);
			char *dir = strdup(path);
			strcat(path, name);
			strcat(path, ".txt");
			if (check_file_existence(path) < 0)
			{
				send_report(sock, UNSUCCESS, &addr);
				recv_report(sock, &addr);
			}
			else
			{
				send_report(sock, SUCCESS, &addr);
				memset(author,0, sizeof(author));
				if ((msg_size = recvfrom(sock, author, sizeof(author), 0, (struct sockaddr*)&addr,  &len)) == SOCKET_ERROR)
				{
					printf("RECV author error: %d\n", WSAGetLastError());
					exit(1);
				}
				printf("RECV  [%d bytes]: author '%s'\n", msg_size, author);
				send_report(sock, SUCCESS, &addr);
				memset(content, 0,sizeof(content));
				if ((msg_size = recvfrom(sock, content, sizeof(content), 0, (struct sockaddr*)&addr,  &len)) == SOCKET_ERROR)
				{
					printf("RECV content of file error: %d\n", WSAGetLastError());
					exit(1);
				}
				printf("RECV  [%d bytes]: content of file '%s'\n", msg_size, content);

				strcat(name, "\n");
				strcat(author, "\n\n");
				if (add_article(path, name, author, content) < 0)
					send_report(sock, UNSUCCESS, &addr);
				else
					send_report(sock, SUCCESS, &addr);
				recv_report(sock, &addr);
				}
				send_content(sock, dir, &addr);
				dirname(path);
				if (path[strlen(path) - 1] != '/')
					strcat(path, "/");
		}
	}
	return 0;
}

void send_input_error(SOCKET sock, struct sockaddr_in *ptr_addr)
{
	send_report(sock, UNSUCCESS, ptr_addr);
	recv_report(sock, ptr_addr);
}

int send_content(SOCKET sock, char *dir_name, struct sockaddr_in *ptr_addr)
{
	struct sockaddr_in addr;
	addr = *ptr_addr;
	char buffer[SIZE_BUF];
	char *filename;
	const char *delimiter = "----------------------|";
	int msg_size;
	DIR *dir = opendir(dir_name);
	memset(buffer, 0, sizeof(buffer));
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
		if ((msg_size = sendto(sock, buffer, strlen(buffer), 0,  (struct sockaddr*)&addr, sizeof(addr))) == SOCKET_ERROR)
		{
			printf("SEND directory content error: %d\n", WSAGetLastError());
			exit(1);
		}
		printf("SEND  [%d bytes]: directory content '%s'\n", msg_size, buffer);
		memset(filename, 0, sizeof(filename));
		strcpy(filename, dir_name);
		if (!strcmp(basename(filename), "."))
			dirname(dir_name);
		else if (!strcmp(basename(filename), ".."))
			dirname(dirname(dir_name));
	}
	else
	{
		const char *err_msg = "!No such file or directory|";
		if ((msg_size = sendto(sock, err_msg, strlen(err_msg), 0,  (struct sockaddr*)&addr, sizeof(addr))) == SOCKET_ERROR)
		{
			printf("SEND no file or directory error: %d\n", WSAGetLastError());
			exit(1);
		}
		printf("SEND  [%d bytes]: no file or directory message '%s'\n", msg_size, buffer);
		dirname(dir_name);
	}

	sendPath_recvReport(sock, dir_name, &addr);
	return 0;
}

int open_file(SOCKET sock, char *path, struct sockaddr_in *ptr_addr)
{
	struct sockaddr_in addr;
	addr = *ptr_addr;
	FILE *fp;
	int msg_size;
	struct stat about_file;

	char buffer[SIZE_STR], text[SIZE_CONTENT];
	memset(text, 0, sizeof(text));
	memset(buffer, 0, sizeof(buffer));

	char *tmp = malloc(SIZE_BUF);
	strcpy(tmp, START_PATH);
	if (!strcmp(path, strcat(tmp, "..")))
	{
		send_content(sock, START_PATH, &addr);
		free(tmp);
		return 0;
	}
	free(tmp);
	if ((fp = fopen(path, "r")) == NULL)
	{
		printf("Opening of file error: %d\n", WSAGetLastError());
		send_content(sock, path, &addr);
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
		if ((msg_size = sendto(sock, text, strlen(text), 0,  (struct sockaddr*)&addr, sizeof(addr))) == SOCKET_ERROR)
		{
			printf("SEND content of file error: %d\n", WSAGetLastError());
			exit(1);
		}

		dirname(path);
		sendPath_recvReport(sock, path, &addr);

	}
	else
		send_content(sock, path, &addr);
	fclose(fp);
	return 0;

}

void sendPath_recvReport(SOCKET sock, char *path, struct sockaddr_in *ptr_addr)
{
	struct sockaddr_in addr;
	addr = *ptr_addr;
	int msg_size;
	recv_report(sock, ptr_addr);
	if (path[strlen(path) - 1] != '/')
		strcat(path, "/");
	if ((msg_size = sendto(sock, path, strlen(path), 0,  (struct sockaddr*)&addr, sizeof(addr))) == SOCKET_ERROR)
	{
		printf("SEND current path error: %d\n", WSAGetLastError());
		exit(1);
	}
	printf("SEND  [%d bytes]: current path '%s'\n", msg_size, path);
	recv_report(sock, ptr_addr);
}

void send_report(SOCKET sock, char *status, struct sockaddr_in *ptr_addr)
{
	struct sockaddr_in addr;
	addr = *ptr_addr;
	int msg_size;
	if ((msg_size = sendto(sock, status, strlen(status), 0,  (struct sockaddr*)&addr, sizeof(addr))) == SOCKET_ERROR)
	{
		printf("SEND report message error: %d\n", WSAGetLastError());
		exit(1);
	}
	printf("SEND  [%d bytes]: report message '%s'\n", msg_size, status);
}


void recv_report(SOCKET sock, struct sockaddr_in *ptr_addr)
{
	struct sockaddr_in addr;
	addr = *ptr_addr;
	int len =  sizeof(addr);
	char status[SIZE_CMD];
	int msg_size;
	memset(status, 0, sizeof(status));
	if ((msg_size = recvfrom(sock, status, sizeof(status), 0, (struct sockaddr*)&addr,  &len)) == SOCKET_ERROR)
	{
		printf("RECV report message failed: %d\n", WSAGetLastError());
		exit(1);
	}
	printf("RECV  [%d bytes]: report message  '%s'\n", msg_size, status);
}

void usage()
{
	printf("usage: server [-p:x] [-i:IP]\n\n");
	printf(" -p:x Port number to listen on\n");
	printf(" -i:str Interface to listen on\n");
	ExitProcess(1);
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
