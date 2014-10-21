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

int doprocessing (int sock,FILE *f)
{
    char buffer[256];
    bzero(buffer,256);
    read(sock,buffer,255);
    write(sock,"I got your message",18);
    fprintf(f,"%s",buffer);
    return 0;
}

char* getFileName(int sock, char *filename)
{
    bzero(filename,256);
    read(sock,filename,255);
    write(sock,"I got your message",18);
    return filename;
}


int main( int argc, char *argv[] )
{
    int sock, bufsocket, port, clilen;
    char buffer[256];
    struct sockaddr_in serverAddr, clientAddr;
    char *name=buffer;
    int v = 10;
    pid_t pid;
    FILE *f = fopen("newfile","ab");
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("ERROR opening socket");
    }
    //Initializing socket
    bzero((char *) &serverAddr, sizeof(serverAddr));
    port = 7777;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    //Bind the host address
    if (bind(sock, (struct sockaddr *) &serverAddr,
                          sizeof(serverAddr)) < 0)
    {
         perror("ERROR on binding");
    }

    //Listening for clients
    listen(sock,5);
    clilen = sizeof(clientAddr);
    //Accept connection
    bufsocket = accept(sock, (struct sockaddr *)&clientAddr,
                                &clilen);
    if (bufsocket < 0)
    {
        //perror("ERROR on accept");
        return 1;
    }
    //create child process
    pid = fork();
    if (pid < 0)
    {
    	perror("ERROR on fork");
    }
    if (pid == 0)
    {
    	/* This is the client process */
    	close(sock);
    	getFileName(bufsocket,name);
    	puts(name);
    }
    else
    {
    	close(bufsocket);

    }
    while(v>0){
    	v--;
    	clilen = sizeof(clientAddr);
    	bufsocket = accept(sock, (struct sockaddr *)&clientAddr,
                                &clilen);
    	if (bufsocket < 0)
    	{
    		continue;
    	}
    	//create child process
    	pid = fork();
    	if (pid < 0)
    	{
    		perror("ERROR on fork");
    	}
    	if (pid == 0)
    	{
    		/* This is the client process */
    		close(sock);
    		doprocessing(bufsocket,f);
    	}
    	else
    	{
    		close(bufsocket);

        }
    }
    rename("newfile",name);
    fclose(f);
    return 0;
}
