/*
    Create a TCP socket
*/
 
#include<stdio.h>
#include<winsock2.h>
 
#pragma comment(lib,"ws2_32.lib") //Winsock Library
 
int main(int argc , char *argv[])
{
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;
    char *message , server_reply[2000];
    int recv_size;
	char buffer[256];
    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return 1;
    }
     
    printf("Initialised.\n");
     
    //Create a socket
    if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d" , WSAGetLastError());
    }
 
    printf("Socket created.\n");
     
     
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);
 
       //Connect to remote server
    if (connect(s , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("connect error");
        return 1;
    }
     
    puts("Connected");
     
    //Send some data

   	memset(buffer, 0, sizeof (buffer));
    scanf ("%s", buffer);
	
  //  n=write (sockfd, buffer, strlen(buffer));
	send(s ,buffer ,strlen(buffer) , 0);
	getchar ();
    memset(buffer, 0, sizeof (buffer));
 //   n=read (sockfd, buffer,255);
	recv_size=recv(s , buffer , 2000 , 0);
	buffer[recv_size] = '\0';
    printf ("%s\n",buffer);


   
    /* Now read server response */
    memset(buffer, 0, sizeof (buffer));
 //   n = read(sockfd,buffer,255);
recv_size=	recv(s , buffer , 2000 , 0);
  buffer[recv_size] = '\0';
    printf("%s\n",buffer);
     memset(buffer, 0, sizeof (buffer));
    scanf ("%s", buffer);
  //  n=write (sockfd, buffer, strlen(buffer));
	send(s ,buffer ,strlen(buffer) , 0);
	getchar ();
    //start from croup


      memset(buffer, 0, sizeof (buffer));
 //   n = read(sockfd,buffer,255);
	recv_size=  recv(s , buffer , 2000 , 0);
	  buffer[recv_size] = '\0';
    printf("%s\n",buffer);
    memset(buffer, 0, sizeof (buffer));
        scanf ("%s", buffer);
        send(s ,buffer ,strlen(buffer) , 0);
		getchar ();
      memset(buffer, 0, sizeof (buffer));
   recv_size= recv(s , buffer , 2000 , 0);
     buffer[recv_size] = '\0';
    printf("%s\n",buffer);
    return 0;
}