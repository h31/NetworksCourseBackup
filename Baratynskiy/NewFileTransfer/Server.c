/*
 * Server.c
 *
 *  Created on: Oct 16, 2014
 *      Author: user
 */


#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main( int argc, char *argv[] )
{
    int sock, bufsocket, port, clilen;
    char buffer[256];
    struct sockaddr_in serverAddr, clientAddr;
    int  n;
    FILE *file = fopen("file.txt","ab");
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("ERROR opening socket");
        exit(1);
    }
    //Initializing socket
    bzero((char *) &serverAddr, sizeof(serverAddr));
    port = 5003;
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

    //Accept connection
    bufsocket = accept(sock, (struct sockaddr *)&clientAddr,
                                &clilen);
    if (bufsocket < 0)
    {
        perror("ERROR on accept");
        exit(1);
    }
    //Communication
    bzero(buffer,256);
    n = read( bufsocket,buffer,255 );
    if (n < 0)
    {
        perror("ERROR reading from socket");
        exit(1);
    }
    fprintf(file,"%s",buffer);
    printf("The file was received: \n");

    fclose(file);
    //Response to client
    n = write(bufsocket,"I got your file\n",18);
    if (n < 0)
    {
        perror("ERROR writing to socket");
        exit(1);
    }
    return 0;
}
