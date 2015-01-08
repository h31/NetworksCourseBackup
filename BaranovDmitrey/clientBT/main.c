#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#define SIZE 12

int conct();
int Send(int socket, char *msg);

int main(void)
{
    int s = conct();
    char buf[SIZE];
    
    send(s, "Hello SERV!", SIZE, 0);
    recv(s, buf, SIZE, 0);

    int i;	
    for(i = 0; i < SIZE; i++)
    {printf("%c", buf[i]);}   
    
    return 0;
}


int Send(int socket, char *msg)
{
    int bytes;
    if((bytes = write(socket, msg, strlen(msg))) <0) return -1;

    return bytes;
}

int conct()
{
    struct sockaddr_in soka;
    int s; //socket descriptor
    int rc; //
        
    char adr[15];
    int prt;
    
    //printf("enter <adr> <port>: ");
    //scanf("%s %d", &adr, &prt);
    prt=4000; adr="127.0.0.1";
    //if(htons(4000) == htons(prt)) {printf("port good!\n");}
    //if(inet_addr("127.0.0.1") == inet_addr(adr)) {printf("adress good!\n");}
    soka.sin_family = AF_INET;
    soka.sin_port = htons(prt);
    soka.sin_addr.s_addr = inet_addr(adr); 

    s = socket(AF_INET, SOCK_STREAM, 0);
    connect(s, (struct sockaddr *)&soka, sizeof(soka));
        
    return s;
}