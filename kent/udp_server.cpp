// udp_client.cpp: определяет точку входа для консольного приложения.
//




#include "stdafx.h"
#include <io.h>
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <locale.h>
#include <sys/types.h>
#include <limits.h>
#include <sys/stat.h>
#include <time.h>
#pragma comment(lib, "WS2_32.lib")

//struct sockaddr_in servaddr,
//cliaddr;
void thread2(int sock) {
	char buffer[256];
	int N = 256;
	int n;
	char mesg[256];

	char buffer_file[256];
	 struct sockaddr_in cliaddr,si_other;
	//	int n;
	int c;
	int i = 0;
	int player;
	int number, odd;
	char my_number[3];
	char my_odd_or_even;
	char my_bet[5];
	memset(buffer,'\0',N);
	int len;
	len = sizeof(cliaddr);




	//n = recvfrom(sock, buffer, N, 0, (struct sockaddr *) &cliaddr, &len);
//	buffer[n]='\0';
//	printf("Here is the message: %s\n", buffer);

//	sendto(sock, "hi", 2, 0, (struct sockaddr*) &cliaddr, len);


	   /*   n = recvfrom(sock,buffer,N,0,(struct sockaddr *)&cliaddr,&len);
	 sendto(sock,buffer,N,0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
	 printf("-------------------------------------------------------\n");
	 //  buffer[n] = 0;
	 printf("Received the following:\n");
	 printf("%s",buffer);
	 printf("-------------------------------------------------------\n");*/
	n = recvfrom(sock, buffer, N, 0, (struct sockaddr *) &cliaddr, &len);
//	buffer[n]='\0';
	printf("Here is the message: %s\n", buffer);
	FILE * fo;
	fo = fopen("test.txt", "r");

	fgets(buffer_file, 50, fo);
	//memset(buffer, 0, sizeof (buffer));
	fclose(fo);
	printf("Here is the message: %s\n", buffer_file);
	if (buffer_file[2] == '0') {
		if (buffer[2] == '1') {
			fo = fopen("test.txt", "a+");
			fputs("\n", fo);
			fputs(buffer, fo);
			fclose(fo);
			player = 1;
			//	write(sock, "You are in game, player\n", 24);
			sendto(sock, "You are in game, player\n",23, 0,(struct sockaddr*) &cliaddr, len);
		} else
			//		write(sock, "u cant be a croup\n", 18);
			sendto(sock, "u cant be a croup\n", 17, 0,(struct sockaddr*) &cliaddr, len);
	} else {
		if (buffer[2] == '0') {
			fo = fopen("test.txt", "a+");

			fputs(buffer, fo);
			fclose(fo);
			player = 0;
			//write(sock, "You are in game, croup\n", 23);
			sendto(sock, "You are in game, croup\n", N, 0,(struct sockaddr*) &cliaddr, len);
		} else {
			//	write(sock, "u cant be a player", 18);
			sendto(sock, "u cant be a player", N, 0,(struct sockaddr*) &cliaddr, len);
		}

	}
	memset(buffer,'\0',N);
	if (player == 0)
		sendto(sock, "waiting...players are making bets\n", N, 0,
				(struct sockaddr *) &cliaddr, sizeof(cliaddr));
	else
		sendto(sock, "make your bet", N, 0, (struct sockaddr *) &cliaddr,
				sizeof(cliaddr));
	//n = read(sock, buffer, N - 1);
	n = recvfrom(sock, buffer, N, 0, (struct sockaddr *) &cliaddr, &len);
	printf("Here is the message: buffer[0]= %c, player= %d\n", buffer[0],
			player);
	if (player == 0) {
		if (buffer[0] == '1')
			printf("its time to show bets\n");
		fo = fopen("bets.txt", "r");
		while (fgets(buffer, 50, fo) != NULL) {
			//		printf("bets: %s\n", buffer);
			//	write(sock, buffer, strlen(buffer));
			sendto(sock, buffer, N, 0, (struct sockaddr *) &cliaddr,
					sizeof(cliaddr));
		}
	} else {
		//write(sock, "i got your bet", 14);
		sendto(sock, "i got your bet", N, 0, (struct sockaddr *) &cliaddr,
				sizeof(cliaddr));
		fo = fopen("bets.txt", "a+");
		fputs(buffer, fo);
		fputs("\n", fo);
		fclose(fo);
		while (buffer[i] != ';') {
			my_number[i] = buffer[i];
			i++;
		}
		i++;
		my_odd_or_even = buffer[i];
		i++;
		int j = 0;
		for (i = 5; i < 10; i++, j++) {
			my_bet[j] = buffer[i];

		}

	}

	if (player == 0) {
		memset(buffer,'\0',N);
		//n = read(sock, buffer, N - 1);
		n = recvfrom(sock, buffer, N, 0, (struct sockaddr *) &cliaddr, &len);
		if (buffer[0] == 's') {
			srand(time(NULL));
			number = rand() % 100;
			odd = rand() % 2;
			fo = fopen("game.txt", "a+");
			fprintf(fo, "%d", number);
			fputs("\n", fo);
			fprintf(fo, "%d", odd);
			fclose(fo);
		}

	}
	int win1 = 5;
	int win2 = 5;
	if (player == 1) {
		//n = read(sock, buffer, N - 1);
		n = recvfrom(sock, buffer, N, 0, (struct sockaddr *) &cliaddr, &len);
		if (buffer[0] == 'r') {
			printf("new check\n");

			fo = fopen("game.txt", "r");
			fgets(buffer, 50, fo);

			if (strcmp(buffer, my_number) == 0)
				win1 = 1;

			else
				win1 = 0;
			fgets(buffer, 50, fo);
			if (buffer[0] == my_odd_or_even)
				win2 = 1;

			else
				win2 = 0;

			fclose(fo);
			if (win1 == 1 || win2 == 1)
				//	write(sock, "Granz, you won", 14);
				sendto(sock, "Granz, you won", N, 0,
						(struct sockaddr *) &cliaddr, sizeof(cliaddr));
			else
				//	write(sock, "you loose", 9);
				sendto(sock, "you loose", N, 0, (struct sockaddr *) &cliaddr,
						sizeof(cliaddr));
		}
	}
}

#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data
 
int main()
{
    SOCKET s;
    struct sockaddr_in server, si_other;
    int slen , recv_len;
    char buf[BUFLEN];
    WSADATA wsa;
 
    slen = sizeof(si_other) ;
     
    //Initialise winsock
    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    printf("Initialised.\n");
     
    //Create a socket
    if((s = socket(AF_INET , SOCK_DGRAM , 0 )) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d" , WSAGetLastError());
    }
    printf("Socket created.\n");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( PORT );
     
    //Bind
    if( bind(s ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d" , WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    puts("Bind done");
 
    //keep listening for data
 thread2(s);
	
 
    closesocket(s);
    WSACleanup();
     
    return 0;
}