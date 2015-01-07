
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <conio.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define BUFLEN 40  //Max length of buffer
#define PORT 5001   //The port on which to listen for incoming data
#define snprintf _snprintf

#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)  
#define bcopy(b1,b2,len) (memmove((b2), (b1), (len)), (void) 0)

struct connection {
	int recv;
	char data[255];
	struct sockaddr_in addr;
	int socket, addrlen;
};

struct connection conns[20];
int conn_num = 0;
DWORD dwThreadId[5];

void my_read(int conn, char* data, int length) {
	while (!conns[conn].recv) Sleep(1);
	bcopy(conns[conn].data, data, length);
	conns[conn].recv = 0;
}

int my_write(int conn, char* data, int length) {
	int n;
	printf("%d\n", conns[conn].socket);
	n=sendto(conns[conn].socket, data, length, 0,
		(struct sockaddr*) &conns[conn].addr, conns[conn].addrlen);
	if(n==SOCKET_ERROR)
		printf("Error\n");
	printf("%d \n",n);
	return n;
}

long int factorial(long int x) {
	return !x ? 1 : x * factorial(x - 1);
}
void calculation(char *buffer,char *answer) {
	int i, j;
	long int f;
	double a, b, k;
	char temp1[BUFLEN],temp2[BUFLEN];
	int size;
	printf("Received: %s\n", buffer);
	bzero(temp1, BUFLEN);
	bzero(temp2, BUFLEN);
	for (i = 1; i < 255; i++) {
		//factorial
		if (buffer[i] == '!'){
			strncpy(temp1, buffer, i);
			f = atoi(temp1);
			printf("f = %d\n", f);
			break;
		}
		//sqrt
		if (buffer[i] == '#'){
			strncpy(temp1, buffer, i);
			a = atof(temp1);
			printf("k = %f\n", a);
			break;
		}
		// *, /, +,-
		if ((buffer[i] == '*') ||(buffer[i] == '/') ||(buffer[i] == '+') || (buffer[i] == '-'))  {
			strncpy(temp1, buffer, i);
			a = atof(temp1);
			printf("a = %f\n", a);
			break;
		}
	}
	//end expression
	for (j = 0; j < BUFLEN-1; j++) {
		if (buffer[j] == '=') {
			strncpy(temp2, buffer + i + 1, j - i - 1);
			b = atof(temp2);
			printf("b = %f\n", b);
			break;
		}
	}
	if (buffer[i] == '+'){
		Sleep(10000);
		sprintf(answer, "Answer = %f", a+b);
	}
	if (buffer[i] == '-'){
		sprintf(answer, "Answer = %f", a-b);
	}
	if (buffer[i] == '*'){
		sprintf(answer, "Answer = %f", a*b);
	}
	if (buffer[i] == '/'){
		if (b==0){
			sprintf(answer, "Error, input divider != 0 !\n");
		}
		else {
			sprintf(answer, "Answer = %f", a/b);
		}
	}
	if (buffer[i] == '#'){
		if (a<0){
			sprintf(answer, "Error, input Root >= 0 !\n");
		}
		else {
			sprintf(answer, "Sqrt = %f", sqrt(a));
		}
	}
	if (buffer[i] == '!'){
		if (f<=0){
			sprintf(answer, "Error, input Factorial > 0 !\n");
		}
		else {
			sprintf(answer, "Factorial = %d", factorial(f));
		}
	}
}

void die(char *s)
{
	perror(s);
	exit(1);
}

DWORD WINAPI startThread(LPVOID lpParam) {
	int sock = *(int*)lpParam;
	char mesg[BUFLEN];
	while(1)
	{
		char answer[BUFLEN];
		bzero(mesg, sizeof(mesg));
		my_read(sock, mesg, BUFLEN);
		calculation(mesg,answer);
		my_write(sock, answer, strlen(answer));
		printf("%s\n",answer);
	}
}

int main() {
	//struct mySocket s;
	int n;
	int num=0; //количество циклов для shutdown
	struct sockaddr_in servaddr, cliaddr,portno;
	int sock;
	socklen_t len;
	len = sizeof(cliaddr);
	char mesg[BUFLEN];
	HANDLE thread[5],mainthread;

	// Initialize Winsock
	WSADATA wsaData;
	n = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (n != 0) {
		printf("WSAStartup failed with error: %d\n", n);
		return -1;
	}

	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock == INVALID_SOCKET)
	{
		die("socket");
	}

	memset((char *) &servaddr, 0, sizeof(servaddr));
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	if( bind(sock , (struct sockaddr*)&servaddr, sizeof(servaddr) ) == -1)
	{
		die("bind");
	}
	while(1)
	{

		/* Accept actual connection from the client */

		/* If connection is established then start communicating */
		int n, i, num;
		char buff[256];
		bzero(buff, 255);

		struct sockaddr_in client_addr;
		socklen_t addrlen = sizeof(struct sockaddr_in);
		n = recvfrom(sock, buff, 255, 0, (struct sockaddr*) &client_addr, &addrlen);
		printf("Res %s \n",buff);
		num = -1;
		for (i = 0; i < conn_num; i++) {
			if ((conns[i].addr.sin_addr.s_addr == client_addr.sin_addr.s_addr) &&
				(conns[i].addr.sin_port == client_addr.sin_port)) {
					num = i;
					break;
			}
		}
		if (num == -1) {
			num = conn_num;
			conns[num].addr.sin_addr = client_addr.sin_addr;
			conns[num].addr.sin_port = client_addr.sin_port;
			conns[num].addr.sin_family = client_addr.sin_family;
			//conns[num].addr.sin_zero = client_addr.sin_zero;
			conns[num].socket = sock;
			conns[num].addrlen = addrlen;
			conns[num].recv = 0;
			thread[num]= CreateThread(NULL, 0, startThread, (LPVOID)&num, 0, &dwThreadId[i]);
			conn_num++;
		}
		printf("Client %d\n", num);

		bcopy(buff, conns[num].data, 255);
		conns[num].recv = 1;
	}
	closesocket(sock);
	return 0;

}
