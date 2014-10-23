#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "writeStruct.h"

int main( int argc, char *argv[] )
{
	int testSize=sizeFile();
	struct Line x[testSize];
	char str[50];
	FILE *file;
	file = fopen("/home/user/workspace/server/file.txt", "r");
	int i;
	for(i=0;fgets(str,sizeof(str),file);i++){
		 writeSize(&x[i], &str);
	}
	fclose(file);

	int sockfd, newsockfd, portno, clilen;
    const int on = 1;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int  n;


    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("ERROR opening socket");
        exit(1);
    }
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 5001;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);


    if (bind(sockfd, (struct sockaddr *) &serv_addr,
                          sizeof(serv_addr)) < 0)
    {
         perror("ERROR on binding");
         exit(1);
    }
    listen(sockfd,5);
    clilen = sizeof(cli_addr);


    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr,
                                &clilen);
    if (newsockfd < 0)
    {
        perror("ERROR on accept");
        exit(1);
    }

    char r='0';
    while(r!='!'){
    bzero(buffer,256);
    n = read( newsockfd,buffer,255 );
    if (n < 0) {
            perror("ERROR reading from socket");
            exit(1);
        }
    r=buffer[0];
    }

    for(i=0;i<testSize;i++)
    {
    char *stringOut=writeToClient(&x[i]);
    n = write(newsockfd, stringOut,strlen(stringOut));
    printf("%d",strlen(stringOut));
    bzero(buffer,256);
    n = read( newsockfd,buffer,255 );
    if (n < 0)
    {
        perror("ERROR reading from socket");
        exit(1);
    }
    printf("Answer: %s\n",buffer);
        if(buffer[0]==x[i].trueAnswer)
        	n = write(newsockfd,"Right\n",6);
        else
        	n = write(newsockfd,"Wrong\n",6);
    if (n < 0)
    {
        perror("ERROR writing to socket");
        exit(1);
}}
    for(i=0;i<testSize;i++)
        freeLine(&x[i]) ;
    return 0;
}
