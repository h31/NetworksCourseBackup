
/* Sample TCP server */

/*Create by Anton Kiselyov*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg)//Обработка сообщения от сервера
{
    perror(msg);
    exit(1);
}

char* callterminal(char* command)
{
	FILE *fp;
	int status;
	char path[16536] = "";
	char answer[16536] = "";
	fp = popen(command, "r");
	if (fp == NULL)
		/* Handle error */;
	while (fgets(path, 16536, fp) != NULL)
	{
		printf("%s", path);
	}
	strcat(answer,path);
	status = pclose(fp);
	if (status == -1) {
		/* Error reported by pclose() */
		return "Error with executing of command";
	} else {
    /* Use macros described under wait() to inspect `status' in order
       to determine success/failure of command executed by popen() */
		char *p = answer;
		return p;
	}

}
void doprocessing (int sock)
{
    int n;
    char buffer[256];

    bzero(buffer,256);

    n = read(sock,buffer,255);
    if (n < 0)
    {
        perror("ERROR reading from socket");
        exit(1);
    }
    printf("Here is the message: %s\n",buffer);
	char answer[16536];
	strcpy(answer,callterminal(buffer));
    n = write(sock,answer,18);
    if (n < 0)
    {
        perror("ERROR writing to socket");
        exit(1);
    }
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     pid_t pid;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {//указать порт
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,//привязка сервера к ресурсам
              sizeof(serv_addr)) < 0)
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd,//Открытие нового сокета
                 (struct sockaddr *) &cli_addr,
                 &clilen);
     if (newsockfd < 0)
          error("ERROR on accept");
     while(1)
     {
          newsockfd = accept(sockfd,
                 (struct sockaddr *) &cli_addr, &clilen);
         if (newsockfd < 0)
         {
             perror("ERROR on accept");
             exit(1);
         }
         /* Create child process */
         pid = fork();
         if (pid < 0)
         {
             perror("ERROR on fork");
             exit(1);
         }
         if (pid == 0)
         {
             /* This is the client process */
             close(sockfd);
             doprocessing(newsockfd);
             exit(0);
         }
         else
         {
             close(newsockfd);
         }

    	/* bzero(buffer,256);
    	 n = read(newsockfd,buffer,256);
    	 if (n < 0) error("ERROR reading from socket");
    	 printf("Here is the message: %s\n",buffer);
    	 char answer[16536];
    	 strcpy(answer,callterminal(buffer));

    	 n = write(newsockfd,answer,18);
    	 if (n < 0) error("ERROR writing to socket");*/
     }
     close(newsockfd);
     close(sockfd);
     return 0;
}
