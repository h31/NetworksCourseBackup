#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <libgen.h>

int send_content(int sock, char *dir_name);
int open_file(int sock, char *path);

int main(int argc, char *argv[])
{
	const int on = 1;
	int sock, client;
	struct sockaddr_in server, client_addr;
	char buffer[1024];
	char *str;

	server.sin_family = AF_INET;
	server.sin_port = htons(5001);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	//Creating of socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		perror("Socket is not created");
		exit(1);
	}
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );

	// Binding of socket
	if (bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("Socket is not bound");
		exit(1);
	}

	// Listening mode
	if (listen(sock, 5) < 0)
	{
		perror("Listening error");
		exit(1);
	}
	client = sizeof(client_addr);
	while(1)
	{
	sock = accept(sock, (struct sockaddr *)&client_addr, &client);
	if(sock < 0)
	{
		perror("Accepting error");
		exit(1);
	}
	bzero(buffer, 1024);

	send_content(sock, "Information System/");

	while(1)
	{
		bzero(buffer, 1024);
		// Receiving
		if (recv(sock, buffer, 1024, 0) <= 0)
		{
			perror("Receiving error");
			exit(1);
		}

		open_file(sock, buffer);

		// Sending


	}
	if (send(sock, "---	Thanks \n", sizeof("---	Thanks \n"), 0) < 0)
		{
			perror("Sending error");
			exit(1);
		}

	close(sock);
	}
	return 0;
}

int send_content(int sock, char *dir_name)
{
	char *filename;
	DIR *dir = opendir(dir_name);
	puts(dir_name);
	if(dir)
	{
	struct dirent *ent;
	while((ent = readdir(dir)) != NULL)
		{
		filename = ent->d_name;
		puts(filename);
		strcat(filename, "\n");
		if (send(sock, filename, strlen(filename), 0) < 0)
			{
				perror("Sending error");
				exit(1);
			}
		}
	}
	else
		{
		char err_msg[] = "---	Error opening directory\n";
		perror("q");
		if (send(sock, err_msg, sizeof(err_msg), 0) < 0)
		{
			perror("Sending error");
			exit(1);
		}
		return -1;
		}
	return 0;
}

int open_file(int sock, char *path)
{
	int file;
	char *buff;
	struct stat about_file;

	static char home_dir[100] = "Information System/";
	strncat(home_dir, path, strlen(path)-2);
	puts(home_dir);

	fstat(file = open(home_dir, O_RDONLY), &about_file);
	if ((about_file.st_mode & S_IFMT) != S_IFDIR)
	{
		if (file >= 0)
		{
			bzero(buff, 0);
			int ret;
			char ch;
			int i = 0;
			while (ret = read(file, &ch, 1) > 0)
			{
				putchar (ch);
				buff[i] = ch;
				i++;
			}
			if (send(sock, buff, i, 0) < 0)
			{
				perror("Sending error");
				exit(1);
			}
			chdir(dirname(home_dir));
			strcat(home_dir, "/");
			puts(home_dir);
			close(file);
			return 0;
		}
		else
		{
			char err_msg[] = "---	No such file\n";
			if (send(sock, err_msg, sizeof(err_msg), 0) < 0)
			{
				perror("Sending error");
				exit(1);
			}

			chdir(dirname(home_dir));
			strcat(home_dir, "/");
			puts(home_dir);
			return -1;
		}
		//free(buff);
	}
	else
	{
		send_content(sock, strcat(home_dir, "/")); //strncat(home_dir, path, strlen(path)-2));
	}
	return 0;

}
