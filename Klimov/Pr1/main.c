

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <sys/stat.h>
#include <unistd.h>
//#include "tinyxml/tinyxml.h"

int main( int argc, char *argv[] )
{
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    char name[256];
    struct stat st = {0};
    struct sockaddr_in serv_addr, cli_addr;
    int  n;

    /* First call to socket() function */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("ERROR opening socket");
        exit(1);
    }
    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 5001;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
                          sizeof(serv_addr)) < 0)
    {
         perror("ERROR on binding");
         exit(1);
    }

    /* Now start listening for the clients, here process will
    * go in sleep mode and will wait for the incoming connection
    */
    listen(sockfd,5);
    clilen = sizeof(cli_addr);

    /* Accept actual connection from the client */
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr,
                                &clilen);
    if (newsockfd < 0)
    {
        perror("ERROR on accept");
        exit(1);
    }
    /* If connection is established then start communicating */

    int i, k = 0, m = 0;
    FILE *fp;

    n = write(newsockfd, "Please type ur e-mail address\n", 30);
    bzero(buffer,256);
    n = read( newsockfd,buffer,255 );
    if (n < 0)
    {
    	perror("ERROR reading from socket");
        exit(1);
    }

    fp = fopen(buffer, "rb");
    if(fp){
    	//already in base
    	printf("E-mail in base: %s\n", buffer);
    }
    strncpy(name, buffer, 256);
    bzero(buffer,256);
    n = write(newsockfd, "Please type the e-mail address of the recipient\n", 48);

    bzero(buffer,256);
    n = read( newsockfd,buffer,255 );
    if (n < 0)
    {
        perror("ERROR reading from socket");
        exit(1);
    }

    printf("E-mail: %s\n", buffer);



    for(i = 0; i < 256; i++)
    	if(buffer[i] == '@')
    		k++;
    if(k > 0){
    	fp = fopen(buffer, "ab+");


    	fprintf(fp, "<%s>\n", name);
    	n = write(newsockfd,"OK\nPlease, write a message:\n",29);
    	bzero(buffer,256);
    	n = read( newsockfd,buffer,255 );
    	fprintf(fp, "%s\n", buffer);
    	fprintf(fp, "</%s>\n", name);
    	printf("Message: %s\n", buffer);

    }
    else
    	n = write(newsockfd,"Wrong e-mail address\n",21);

    if (n < 0)
    {
        perror("ERROR writing to socket");
        exit(1);
    }
    return 0;
}


