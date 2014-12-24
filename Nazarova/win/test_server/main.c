#include <winsock2.h>

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define DEFAULT_PORT 5001
#define SIZE_CMD 5
#define SIZE_BUF 1024
#define SIZE_CONTENT 4096
#define SIZE_STR 128
#define MAX_FILES 20
#define MAX_CONNECT 3
#define SUCCESS "000"
#define UNSUCCESS "111"
#define START_PATH "C:/Users/Kseniya/workspace/test_server/Information System/"

int port = DEFAULT_PORT;
BOOL binterface = 0;
char szAddress[SIZE_STR];

int send_content(SOCKET sock, char *dir_name);
int open_file(SOCKET sock, char *path);
void sendPath_recvReport(SOCKET sock, char *path);
void send_input_error(SOCKET sock);
void send_report(SOCKET sock, char *status);
void recv_report(SOCKET sock);
void ValidateArgs(int argc, char **argv);
void usage();
DWORD WINAPI ClientThread(LPVOID lpParam);

int main(int argc, char **argv)
{
	WSADATA       wsd;
	SOCKET        sock,
	sClient;
	int           iAddrSize;
	HANDLE        hThread;
	DWORD         dwThreadId;
	struct sockaddr_in local,
	client;

	ValidateArgs(argc, argv);
	if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
	{
		printf("Failed to load Winsock!\n");
		return 1;
	}
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (sock == SOCKET_ERROR)
	{
		printf("socket() failed: %d\n", WSAGetLastError());
		return 1;
	}
	if (binterface)
	{
		local.sin_addr.s_addr = inet_addr(szAddress);
		if (local.sin_addr.s_addr == INADDR_NONE)
		usage();
		}
	else
		local.sin_addr.s_addr = htonl(INADDR_ANY);
	local.sin_family = AF_INET;
	local.sin_port = htons(port);

	if (bind(sock, (struct sockaddr *)&local,
	sizeof(local)) == SOCKET_ERROR)
	{
		printf("bind() failed: %d\n", WSAGetLastError());
		return 1;
	}
	listen(sock, 8);

	while (1)
	{
		iAddrSize = sizeof(client);
		sClient = accept(sock, (struct sockaddr *)&client,
		&iAddrSize);
		if (sClient == INVALID_SOCKET)
		{
			printf("accept() failed: %d\n", WSAGetLastError());
			break;
		}
		printf("Accepted client: %s:%d\n",
		inet_ntoa(client.sin_addr), ntohs(client.sin_port));

		hThread = CreateThread(NULL, 0, ClientThread,
		(LPVOID)sClient, 0, &dwThreadId);
		if (hThread == NULL)
		{
			printf("CreateThread() failed: %d\n", GetLastError());
			break;
		}
		CloseHandle(hThread);
	}
	closesocket(sock);

	WSACleanup();
	return 0;
}

void send_input_error(SOCKET sock)
{
	send_report(sock, UNSUCCESS);
	recv_report(sock);
}

int send_content(SOCKET sock, char *dir_name)
{
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
		if ((msg_size = send(sock, buffer, strlen(buffer), 0)) == SOCKET_ERROR)
		{
			printf("SEND directory content error: %d\n", WSAGetLastError());
			exit(1);
		}
		printf("SEND  [%d bytes]: directory content '%s'\n", msg_size, buffer);
		memset(filename, 0,sizeof(filename));
		strcpy(filename, dir_name);
		if (!strcmp(basename(filename), "."))
			dirname(dir_name);
		else if (!strcmp(basename(filename), ".."))
			dirname(dirname(dir_name));
	}
	else
	{
		const char *err_msg = "!No such file or directory|";
		if ((msg_size = send(sock, err_msg, strlen(err_msg), 0)) == SOCKET_ERROR)
		{
			printf("SEND no file or directory error: %d\n", WSAGetLastError());
			exit(1);
		}
		printf("SEND  [%d bytes]: no file or directory message '%s'\n", msg_size, buffer);
		dirname(dir_name);
	}
	sendPath_recvReport(sock, dir_name);
	return 0;
}
DWORD WINAPI ClientThread(LPVOID lpParam)
{
	SOCKET        sock=(SOCKET)lpParam;
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
		if ((msg_size = recv(sock, buffer, sizeof(buffer), 0)) == SOCKET_ERROR)
		{
			printf("Receive :start_msg failed: %d\n", WSAGetLastError());
			exit(1);
		}
	}
	buffer[msg_size] = '\0';
	send_content(sock, START_PATH);
	strcpy(path, START_PATH);
	while(1)
	{
		if ((msg_size = send(sock, invite_msg, strlen(invite_msg), 0)) == SOCKET_ERROR)
		{
			printf("SEND invitation message error: %d\n", WSAGetLastError());
			exit(1);
		}
		printf("SEND  [%d bytes]: invitation message '%s'\n", msg_size, invite_msg);

		memset(buffer, 0, sizeof(buffer));
		if ((msg_size = recv(sock, buffer, sizeof(buffer), 0)) == SOCKET_ERROR)
		{
			printf("RECV command error: %d\n", WSAGetLastError());
			exit(1);
		}
		printf("RECV  [%d bytes]: command '%s'\n", msg_size, buffer);
		if (!strcmp(buffer, ":exit"))
		{
			if ((msg_size = send(sock, exit_msg, strlen(exit_msg), 0)) == SOCKET_ERROR)
			{
				printf("SEND directory content error: %d\n", WSAGetLastError());
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
			memset(path, 0, sizeof(path));
			if ((msg_size = recv(sock, path, sizeof(path), 0)) == SOCKET_ERROR)
			{
				printf("RECV path to file error: %d\n", WSAGetLastError());
				exit(1);
			}
			printf("RECV  [%d bytes]: path to file message '%s'\n", msg_size, path);
			open_file(sock, path);
		}
		if (!strcmp(buffer, "find"))
		{
			memset(author, 0,sizeof(author));
			if ((msg_size = recv(sock, author, sizeof(author), 0)) == SOCKET_ERROR)
			{
				printf("RECV author to find error: %d\n", WSAGetLastError());
				exit(1);
			}
			printf("RECV  [%d bytes]: author to find '%s'\n", msg_size, author);
			find_for_author(sock, path, author);
			sendPath_recvReport(sock, path);
		}
		if (!strcmp(buffer, "add"))
		{
			memset(name, 0,sizeof(name));
			if ((msg_size = recv(sock, name, sizeof(name), 0)) == SOCKET_ERROR)
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
				send_report(sock, UNSUCCESS);
				recv_report(sock);
			}
			else
			{
				send_report(sock, SUCCESS);
				memset(author,0, sizeof(author));
				if ((msg_size = recv(sock, author, sizeof(author), 0)) == SOCKET_ERROR)
				{
					printf("RECV author error: %d\n", WSAGetLastError());
					exit(1);
				}
				printf("RECV  [%d bytes]: author '%s'\n", msg_size, author);
				send_report(sock, SUCCESS);
				memset(content, 0,sizeof(content));
				if ((msg_size = recv(sock, content, sizeof(content), 0)) == SOCKET_ERROR)
				{
					printf("RECV content of file error: %d\n", WSAGetLastError());
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
	return 0;
}

int open_file(SOCKET sock, char *path)
{
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
		send_content(sock, START_PATH);
		free(tmp);
		return 0;
	}
	free(tmp);
	if ((fp = fopen(path, "r")) == NULL)
	{
		printf("Opening of file error: %d\n", WSAGetLastError());
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
		if ((msg_size = send(sock, text, strlen(text), 0)) == SOCKET_ERROR)
		{
			printf("SEND content of file error: %d\n", WSAGetLastError());
			exit(1);
		}
		printf("SEND  [%d bytes]: content of file \n%s\n\n", msg_size, text);

		dirname(path);
		sendPath_recvReport(sock, path);

	}
	else
		send_content(sock, path);
	fclose(fp);
	return 0;

}

void sendPath_recvReport(SOCKET sock, char *path)
{
	int msg_size;
	recv_report(sock);
	if (path[strlen(path) - 1] != '/')
		strcat(path, "/");
	if ((msg_size = send(sock, path, strlen(path), 0)) == SOCKET_ERROR)
	{
		printf("SEND current path error: %d\n", WSAGetLastError());
		exit(1);
	}
	printf("SEND  [%d bytes]: current path '%s'\n", msg_size, path);
	recv_report(sock);
}

void send_report(SOCKET sock, char *status)
{
	int msg_size;
	if ((msg_size = send(sock, status, strlen(status), 0)) == SOCKET_ERROR)
	{
		printf("SEND report message error: %d\n", WSAGetLastError());
		exit(1);
	}
	printf("SEND  [%d bytes]: report message '%s'\n", msg_size, status);
}


void recv_report(SOCKET sock)
{
	char status[SIZE_CMD];
	int msg_size;
	memset(status, 0, sizeof(status));
	if ((msg_size = recv(sock, status, sizeof(status), 0)) == SOCKET_ERROR)
	{
		printf("RECV report message failed: %d\n", WSAGetLastError());
		exit(1);
	}
	printf("RECV  [%d bytes]: report message  '%s'\n", msg_size, status);
}

void usage()
{
printf("usage: server [-p:x] [-i:IP]\n\n");
printf("-p:x      Port number to listen on\n");
printf("-i:str    Interface to listen on\n");
ExitProcess(1);
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
