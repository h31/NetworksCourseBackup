/*
 * Server.c
 *
 *  Created on: Oct 16, 2014
 *      Author: user
 */


#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>

void getFileName(FILE *f,int sock, int bufsocket,struct sockaddr_in clientAddr,int clilen,pid_t pid)
{
	bufsocket = accept(sock, (struct sockaddr *)&clientAddr,
	                                &clilen);
	    if (bufsocket < 0)
	    {
	        perror("ERROR on accept");
	        exit(1);
	    }
	    //create child process
	    pid = fork();
	            if (pid < 0)
	            {
	                perror("ERROR on fork");
	    	    exit(1);
	            }
	            if (pid == 0)
	            {
	                /* This is the client process */
	                close(sock);
	                doprocessing(bufsocket,f);
	                fprintf(f,"%s\n","");
	                exit(0);
	            }
	            else
	            {
	                close(bufsocket);
	            }
}

void doprocessing (int sock,FILE *f)
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
    fprintf(f,"%s",buffer);
    if (n < 0)
    {
        perror("ERROR writing to socket");
        exit(1);
    }
}

int main( int argc, char *argv[] )
{
    int sock, bufsocket, port, clilen;
    char buffer[256];
    struct sockaddr_in serverAddr, clientAddr;
    int  n,b;
    int size;
    int i=0;
    pid_t pid;
    FILE *f = fopen("file.txt","ab");
    struct stat buff;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("ERROR opening socket");
        exit(1);
    }
    //Initializing socket
    bzero((char *) &serverAddr, sizeof(serverAddr));
    port = 9999;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    //Bind the host address
    if (bind(sock, (struct sockaddr *) &serverAddr,
                          sizeof(serverAddr)) < 0)
    {
         perror("ERROR on binding");
         exit(1);
    }

    //Listening for clients
    listen(sock,5);
    clilen = sizeof(clientAddr);
    getFileName(f,sock,bufsocket,clientAddr,clilen,pid);
    //Accept connection
    while(1){
    bufsocket = accept(sock, (struct sockaddr *)&clientAddr,
                                &clilen);
    if (bufsocket < 0)
    {
        perror("ERROR on accept");
        exit(1);
    }
    //create child process
    pid = fork();
            if (pid < 0)
            {
                perror("ERROR on fork");
    	    exit(1);
            }
            if (pid == 0)
            {
                /* This is the client process */
                close(sock);
                doprocessing(bufsocket,f);
                exit(0);
            }
            else
            {
                close(bufsocket);
            }
    }
    //fprintf(file,"%s",buffer);
 //   printf("The file was received: \n");

    fclose(f);

    return 0;
}
