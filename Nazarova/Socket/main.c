#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in server;
	char buffer[1024];

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
	// Binding of socket
	if (bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("Socket is not bound");
		exit(1);
	}

	// Listening mode
	if (listen(sock, 2) < 0)
	{
		perror("Listening error");
		exit(1);
	}
	while(1)
	{
		sock = accept(sock, NULL, NULL);
		if(sock < 0)
		{
			perror("Accepting error");
			exit(1);
		}

		while(1)
		{
			bzero(buffer, 1024);
			// Receiving
			if (recv(sock, buffer, 1024, 0) <= 0)
			{
				perror("Receiving error");
				exit(1);
			}
			printf("Input Message: %s \n", buffer);

			// Sending
			if (send(sock, "Thanks \n", sizeof("Thanks \n"), 0) < 0)
			{
				perror("Sending error");
				exit(1);
			}
		}

		close(sock);
	}

	return 0;

}
