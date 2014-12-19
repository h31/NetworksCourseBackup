/* Sample UDP server */

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <pthread.h>
#include <netdb.h>
pthread_t t1, t2, t3;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
struct sockaddr_in servaddr,cliaddr;
 void thread2(int sock) {
 char buffer[256];
 int N = 256;
 int n;
 char mesg[256];

 char buffer_file[256];

 //	int n;
 int c;
 int i = 0;
 int player;
 int number, odd;
 char my_number[3];
 char my_odd_or_even;
 char my_bet[5];
 bzero(buffer, N);
 socklen_t len;
 len = sizeof(cliaddr);
 /*      n = recvfrom(sock,buffer,N,0,(struct sockaddr *)&cliaddr,&len);
 sendto(sock,buffer,N,0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
 printf("-------------------------------------------------------\n");
 //  buffer[n] = 0;
 printf("Received the following:\n");
 printf("%s",buffer);
 printf("-------------------------------------------------------\n");*/
n = recvfrom(sock, buffer, N, 0, (struct sockaddr *) &cliaddr, &len);
 printf("Here is the message: %s\n", buffer);
 FILE * fo;
 fo = fopen("test.txt", "r");
 bzero(buffer_file, N);
 fgets(buffer_file, 50, fo);

 fclose(fo);

 if (buffer_file[2] == '0') {
 if (buffer[2] == '1') {
 fo = fopen("test.txt", "a+");
 fputs("\n", fo);
 fputs(buffer, fo);
 fclose(fo);
 player = 1;
 //	write(sock, "You are in game, player\n", 24);
 sendto(sock, "You are in game, player\n", N, 0,
 (struct sockaddr *) &cliaddr, sizeof(cliaddr));
 } else
 //		write(sock, "u cant be a croup\n", 18);
 sendto(sock, "u cant be a croup\n", N, 0,
 (struct sockaddr *) &cliaddr, sizeof(cliaddr));
 } else {
 if (buffer[2] == '0') {
 fo = fopen("test.txt", "a+");

 fputs(buffer, fo);
 fclose(fo);
 player = 0;
 //write(sock, "You are in game, croup\n", 23);
 sendto(sock, "You are in game, croup\n", N, 0,
 (struct sockaddr *) &cliaddr, sizeof(cliaddr));
 } else {
 //	write(sock, "u cant be a player", 18);
 sendto(sock, "u cant be a player", N, 0,
 (struct sockaddr *) &cliaddr, sizeof(cliaddr));
 }

 }
 bzero(buffer, N);
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
 if (buffer[0] == "1")
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
 bzero(buffer, N);
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
struct sockaddr_in servaddr,
cliaddr;
/*static void test(void* arg) {
	pthread_mutex_lock(&mutex);

	int sock = *(int*) arg;
	char mesg[1000];
	socklen_t len;
	int n;
	char buffer[256];
	int N = 256;

	char buffer_file[256];

	//	int n;
	int c;
	int i = 0;
	int player;
	int number, odd;
	char my_number[3];
	char my_odd_or_even;
	char my_bet[5];
	for (;;) {
		/*		 len = sizeof(cliaddr);
		 n= recvfrom(sockfd,mesg,1000,0,(struct sockaddr *)&cliaddr,&len);
		 mesg[n] = 0;
		 printf("%s",mesg);
		 sendto(sockfd,"hekko",10,0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));*/
/*
		bzero(buffer, N);

		n = recvfrom(sock, buffer, N, 0, (struct sockaddr *) &cliaddr, &len);
		printf("Here is the message: %s\n", buffer);
		FILE * fo;
		fo = fopen("test.txt", "r");
		bzero(buffer_file, N);
		fgets(buffer_file, 50, fo);

		fclose(fo);

		if (buffer_file[2] == '0') {
			if (buffer[2] == '1') {
				fo = fopen("test.txt", "a+");
				fputs("\n", fo);
				fputs(buffer, fo);
				fclose(fo);
				player = 1;
				//	write(sock, "You are in game, player\n", 24);
				sendto(sock, "You are in game, player\n", N, 0,
						(struct sockaddr *) &cliaddr, sizeof(cliaddr));
			} else
				//		write(sock, "u cant be a croup\n", 18);
				sendto(sock, "u cant be a croup\n", N, 0,
						(struct sockaddr *) &cliaddr, sizeof(cliaddr));
		} else {
			if (buffer[2] == '0') {
				fo = fopen("test.txt", "a+");

				fputs(buffer, fo);
				fclose(fo);
				player = 0;
				//write(sock, "You are in game, croup\n", 23);
				sendto(sock, "You are in game, croup\n", N, 0,
						(struct sockaddr *) &cliaddr, sizeof(cliaddr));
			} else {
				//	write(sock, "u cant be a player", 18);
				sendto(sock, "u cant be a player", N, 0,
						(struct sockaddr *) &cliaddr, sizeof(cliaddr));
			}

		}
		bzero(buffer, N);
		if (player == 0)
			sendto(sock, "waiting...players are making bets\n", N, 0,
					(struct sockaddr *) &cliaddr, sizeof(cliaddr));
		else
			sendto(sock, "make your bet", N, 0, (struct sockaddr *) &cliaddr,
					sizeof(cliaddr));
		n = recvfrom(sock, buffer, N, 0, (struct sockaddr *) &cliaddr, &len);
		printf("Here is the message: buffer[0]= %c, player= %d\n", buffer[0],
				player);

		if (player == 0) {
			if (buffer[0] == "1")
				printf("its time to show bets\n");
			fo = fopen("bets.txt", "r");
			while (fgets(buffer, 50, fo) != NULL) {
				//		printf("bets: %s\n", buffer);
				//	write(sock, buffer, strlen(buffer));
				sendto(sock, "bets made", N, 0, (struct sockaddr *) &cliaddr,
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
	}

	pthread_mutex_unlock(&mutex);
	shutdown(sock, 2);
	close(sock);
}*/

int main(int argc, char**argv) {
	int sockfd, n;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	pthread_t accept_thread;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(32000);
	int optval = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
	bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	thread2(sockfd);
}
