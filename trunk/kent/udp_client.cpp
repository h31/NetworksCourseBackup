/*
    Simple udp client
    Silver Moon (m00n.silv3r@gmail.com)
*/
#include<stdio.h>
#include<winsock2.h>
 
#pragma comment(lib,"ws2_32.lib") //Winsock Library
 
#define SERVER "127.0.0.1"  //ip address of udp server
#define BUFLEN 256  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data
 
int main(void)
{
    struct sockaddr_in si_other;
    int s, slen=sizeof(si_other);
    char buf[BUFLEN];
    char message[BUFLEN];
    WSADATA wsa;
	int n;
	int N=256;
    //Initialise winsock
    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    printf("Initialised.\n");
     
    //create socket
    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
    {
        printf("socket() failed with error code : %d" , WSAGetLastError());
        exit(EXIT_FAILURE);
    }
     
    //setup address structure
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
    si_other.sin_addr.S_un.S_addr = inet_addr(SERVER);
     
    //start communication
    memset(buf,'\0', BUFLEN);
	scanf("%s", buf);
	//getchar();
	sendto(s,buf, strlen(buf) , 0 , (struct sockaddr *) &si_other, slen);
	memset(buf,'\0', BUFLEN);
	recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen);
	printf("%s", buf);
	
	memset(buf,'\0', BUFLEN);
	recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen);
	printf("%s", buf);

	scanf("%s", buf);
	//getchar();
	sendto(s,buf, strlen(buf) , 0 , (struct sockaddr *) &si_other, slen);

	memset(buf,'\0', BUFLEN);
	recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen);
	printf("%s", buf);

	scanf("%s", buf);
	//getchar();
	sendto(s,buf, strlen(buf) , 0 , (struct sockaddr *) &si_other, slen);

	memset(buf,'\0', BUFLEN);
	recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen);
	printf("%s", buf);
    closesocket(s);
    WSACleanup();
 
 
    return 0;
}