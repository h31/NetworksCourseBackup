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
    char end;
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
   /* bcopy((char *)server->h_addr,
           (char *)&serv_addr.sin_addr.s_addr,
                server->h_length);*/
    serv_addr.sin_port = htons(portno);

    /* Now connect to the server */
    if (connect(sockfd,&serv_addr,sizeof(serv_addr)) < 0)
    {
         perror("ERROR connecting");
         exit(1);
    }
    /* Now ask for a message from the user, this message
    * will be read by server
    */
    //Start connect
    int i;
    for(i=0;i<2;i++){
    strcpy(buffer,"ERROR");
    while(strcmp(buffer,"ERROR")==0){
    	if(i==0)
    		printf("Please enter '!' to connect :");
    	else
    		printf("Please enter number of test :");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0)
    {
         perror("ERROR writing to socket");
         exit(1);
    }
    bzero(buffer,256);
       n = read(sockfd,buffer,255);
           if (n < 0)
           {
                perror("ERROR reading from socket");
                exit(1);
           }
    }}
    /*//Registration
    while(strcmp(buffer,"Login")==0){
    	printf("%s\n",buffer);
        bzero(buffer,256);
        fgets(buffer,255,stdin);
        n = write(sockfd,buffer,strlen(buffer));
        bzero(buffer,256);
        n = read(sockfd,buffer,255);
        }
    //Enter password
        while(strcmp(buffer,"Parol")==0){
        	printf("%s\n",buffer);
            bzero(buffer,256);
            fgets(buffer,255,stdin);
            n = write(sockfd,buffer,strlen(buffer));
            bzero(buffer,256);
            n = read(sockfd,buffer,255);
            }*/
    /*bzero(buffer,256);
    n = read(sockfd,buffer,255);
    printf("%s\n",buffer);*/
    while(1){
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
        if (n < 0)
        {
             perror("ERROR reading from socket");
             exit(1);
        }
    printf("%s\n",buffer);
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0)
    {
         perror("ERROR writing to socket");
         exit(1);
    }
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0)
    {
         perror("ERROR reading from socket");
         exit(1);
    }
    printf("%s\n",buffer);
    if(buffer[strlen(buffer)-2]=='!')
    	close(sockfd);
    }
    return 0;
}


