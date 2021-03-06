#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
    if (argc < 3) {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(0);
    }
    portno = atoi(argv[2]);
    /* Create a socket point */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("ERROR opening socket");
        exit(1);
    }
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,&serv_addr,sizeof(serv_addr)) < 0)
    {
         perror("ERROR connecting");
         exit(1);
    }


while(1)
{
	bzero(buffer,256);
	    n = read(sockfd,buffer,255);
	        if (n < 0)
	        {
	             perror("ERROR reading from socket");
	             exit(1);
	        }
	    printf("%s\n",buffer);
    bzero(buffer,256);
	printf("Print login\n");
    fgets(buffer,255,stdin);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0)
    {
         perror("ERROR writing to socket");
         exit(1);
    }

    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    //printf("----%s----\n",buffer);

    bzero(buffer,256);
	printf("Print password\n");
    fgets(buffer,255,stdin);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0)
    {
         perror("ERROR writing to socket");
         exit(1);
    }
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if((!strncmp(buffer,"next",4)))
    {
    	n = write(sockfd,"OK",2);
    	break;
    }

}
    while(1)
    {
    	bzero(buffer,256);
    	n = read(sockfd,buffer,255);
    	//printf("Reading\n");
    	if((strncmp(buffer,"OK",2)))
    	{
    		if((strncmp(buffer,"next",4)))
    			printf("%s\n",buffer);
    	}

    	//printf("Writing\n");
    	if(!(strncmp(buffer,"next",4)))
    	{
			bzero(buffer,256);
			fgets(buffer,255,stdin);
			n = write(sockfd,buffer,strlen(buffer));
			if (n < 0)
			{
			   perror("ERROR writing to socket");
			   exit(1);
			}
    	}
    	else
    	{
    		n = write(sockfd,"OK",2);
    		if (n < 0)
    		{
    		   perror("ERROR writing to socket");
    		   exit(1);
    		}
    	}
    }

}


