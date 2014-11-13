#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <unistd.h>
#include <string.h>
#include <time.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
pthread_t t1, t2, t3;


void* thread2(int sock) {
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
	bzero(buffer, N);

	n = read(sock, buffer, N - 1);
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
			write(sock, "You are in game, player\n", 24);
		} else
			write(sock, "u cant be a croup\n", 18);
	} else {
		if (buffer[2] == '0') {
			fo = fopen("test.txt", "a+");

			fputs(buffer, fo);
			fclose(fo);
			player = 0;
			write(sock, "You are in game, croup\n", 23);
		} else {
			write(sock, "u cant be a player", 18);
		}

	}
	bzero(buffer, N);
	if (player == 0)
		write(sock, "waiting...players are making bets\n", 34);
	else
		write(sock, "make your bet", 13);

	n = read(sock, buffer, N - 1);
	printf("Here is the message: buffer[0]= %c, player= %d\n", buffer[0],
			player);
	if (player == 0) {
		if (buffer[0] == "1")
			printf("its time to show bets\n");
		fo = fopen("bets.txt", "r");
		while (fgets(buffer, 50, fo) != NULL) {
			printf("bets: %s\n", buffer);
		}
	} else {
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
		bzero(buffer, N);
		n = read(sock, buffer, N - 1);
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
		n = read(sock, buffer, N - 1);
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
				write(sock, "Granz, you won", 14);
			else
				write(sock, "you loose", 9);
		}
	}

}
void* thread1(int sock) {
	write(sock, "its thread1", 11);
//	printf("its thread\n");
}
int main(int argc, char *argv[]) {
	int sockfd, newsockfd, portno, clilen, client;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;

	/* First call to socket() function */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("ERROR opening socket");
		exit(1);
	}
	/* Initialize socket structure */
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = 5001;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	const int on = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	/* Now bind the host address using bind() call.*/
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("ERROR on binding");
		exit(1);
	}

	/* Now start listening for the clients, here process will
	 * go in sleep mode and will wait for the incoming connection
	 */

	listen(sockfd, 5);
	clilen = sizeof(cli_addr);
	bzero(buffer, 256);

	while (1) {

		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0) {
			perror("ERROR on accept");
			exit(1);
		}

		pthread_create(&t2, NULL, thread2, newsockfd);

	}
	return 0;
}
