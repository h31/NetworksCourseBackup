/*
 * Server.c
 *
 *  Created on: Oct 16, 2014
 *      Author: Baratynskiy
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

int getFileSize(int sock, int size)
{
	char buffer[256],newbuffer[256];
    bzero(buffer,256);
    read(sock,buffer,255);
    write(sock,"I got your message",18);
    sscanf(buffer,"%d",&size);
    return size;
}

int main( int argc, char *argv[] )
{
    int sock, bufsocket, port, clilen;
    const int on = 1;
    char buffer[256];
    struct sockaddr_in serverAddr, clientAddr;
    char *name=buffer;
    int fs;
    while(1)
    {
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
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );
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
    getFileName(bufsocket,name);
    puts(name);
    clilen = sizeof(clientAddr);
    //Accept connection
    bufsocket = accept(sock, (struct sockaddr *)&clientAddr,
                                    &clilen);
    if (bufsocket < 0)
    {
    	//perror("ERROR on accept");
    	return 1;
    }
    fs = getFileSize(bufsocket,fs);
    if (fs%256 > 0)
    {
    	fs = fs/256+1;
    }
    else
    {
    	fs = fs/256;
    }
    while(fs>0)
    {
        fs--;
        clilen = sizeof(clientAddr);
        bufsocket = accept(sock, (struct sockaddr *)&clientAddr,
                                &clilen);
        if (bufsocket < 0)
        {
            continue;
        }
        doprocessing(bufsocket,f);
    }
    close(bufsocket);
    close(sock);
    fclose(f);
    rename("newfile",name);
    }
    return 0;
}
