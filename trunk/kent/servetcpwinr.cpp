/*
    Bind socket to port 8888 on localhost
*/
#include<io.h>
#include<stdio.h>
#include<winsock2.h>
#include <time.h>
#include <windows.h>
DWORD Thread1, Thread2;
#pragma comment(lib,"ws2_32.lib") //Winsock Library
void thread2(int param) {
	int recv_size;
	char buffer[256];
	char buffer_file[256];
	int N = 256;
	int n;
	int c;
	int i = 0;
	int player;
	int number, odd;
	char my_number [3];
	char my_odd_or_even;
	char my_bet [5];
	int sock = (int)param;
	
//	n = read(sock, buffer, N - 1);
	
	FILE * fo;
	
	fo = fopen("test.txt", "r");
	fgets(buffer_file, 50, fo);
	fclose(fo);

	memset(buffer, 0, sizeof (buffer));
	recv_size=recv(sock , buffer , 2000 , 0);
	 buffer[recv_size] = '\0';
	printf("Here is the message: %c\n", buffer);
	if (buffer_file[2] == '0') {
		if (buffer[2] == '1') {
			fo = fopen("test.txt", "a+");
			fputs("\n", fo);
			fputs(buffer, fo);
			fclose(fo);
			player = 1;
		//	write(sock, "You are in game, player\n", 24);
			send(sock , "You are in game, player\n" , 24 , 0);
			
		} else {
		//	write(sock, "u cant be a croup\n", 18);
		send(sock , "u cant be a croup\n" ,18 , 0);
		
		}
	} else {
		if (buffer[2] == '0') {
			fo = fopen("test.txt", "a+");

			fputs(buffer, fo);
			fclose(fo);
			player = 0;
		//	write(sock, "You are in game, croup\n", 23);
			send(sock ,"You are in game, croup\n" ,23 , 0);
		
		}
		else {
			//write(sock, "u cant be a player", 18);
			send(sock ,"u cant be a player\n" ,18 , 0);
		
		 }

	}
	memset(buffer, 0, sizeof (buffer));
	if (player == 0) {
		//write(sock, "waiting...players are making bets\n", 34);
			send(sock ,"waiting...players are making bets\n" ,34 , 0);
		
	}
	else
	//	write(sock, "make your bet", 13);
	send(sock ,"make your bet" ,13 , 0);

//	n = read(sock, buffer, N - 1);
recv_size=	recv(sock , buffer , 2000 , 0);
 buffer[recv_size] = '\0';
	printf("Here is the message: buffer[0]= %c, player= %d\n", buffer[0],
			player);
	if (player == 0) {
		if (buffer[0] == '1')
			printf("its time to show bets\n");
		fo = fopen("bets.txt", "r");
		while (fgets(buffer, 50, fo) != NULL) {
	//		printf("bets: %s\n", buffer);
		//	write(sock, buffer, strlen(buffer));
			send(sock ,buffer ,strlen(buffer) , 0);
		}
	} else {
	//	write(sock, "i got your bet", 14);
		send(sock ,"i got your bet", 14 , 0);
		fo = fopen("bets.txt", "a+");
		fputs(buffer, fo);
		fputs("\n", fo);
		fclose(fo);
		while (buffer[i]!=';') {
			my_number[i]=buffer[i];
					i++;
		}
		i++;
		my_odd_or_even=buffer[i];
		i++;
		int j=0;
		for(i=5;i <10;i++,j++) {
			my_bet[j]=buffer[i];

		}

	}

	if (player == 0) {
		memset(buffer, 0, sizeof (buffer));
//		n = read(sock, buffer, N - 1);
		recv_size=recv(sock , buffer , 2000 , 0);
		 buffer[recv_size] = '\0';
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
	int win1=5;
	int win2=5;
	if (player == 1) {
//		n = read(sock, buffer, N - 1);
	recv_size=	recv(sock , buffer , 2000 , 0);
	 buffer[recv_size] = '\0';
		if (buffer[0] == 'r') {
			printf("new check\n");

			fo = fopen("game.txt", "r");
			fgets(buffer, 50, fo);

			if (strcmp(buffer,my_number)==0)
				win1=1;

			else
				win1=0;
			fgets(buffer, 50, fo);
			if (buffer[0]==my_odd_or_even)
							win2=1;

						else
							win2=0;

			fclose(fo);
			if (win1==1||win2==1)
			//	write(sock, "Granz, you won", 14);
					send(sock ,"Gratz, you won", 14 , 0);
			else
			//	write(sock, "you loose", 9);
				send(sock ,"you loose", 9 , 0);
		}
	}

}
int main(int argc , char *argv[])
{
    WSADATA wsa;
    SOCKET s , new_socket;
    struct sockaddr_in server , client;
    int c;
    char *message;
	char buffer [256];
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
      
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
      
    //Bind
    if( bind(s ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d" , WSAGetLastError());
    }
      
    puts("Bind done");
  
    //Listen to incoming connections
    listen(s , 3);
      
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
      
 c = sizeof(struct sockaddr_in);

    new_socket = accept(s , (struct sockaddr *)&client, &c);
    if (new_socket == INVALID_SOCKET)
    {
        printf("accept failed with error code : %d" , WSAGetLastError());
    }
      
    puts("Connection accepted");

	thread2(new_socket);
  
    closesocket(s);
    WSACleanup();
      
    return 0;
}