#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <math.h>
#include <gmp.h>


long int factorial(int x) {
    return !x ? 1 : x * factorial(x - 1);
}

void calculation(char *buffer, char *ans, int n, int newsockfd) {
	int i, j;
	long int f;
	mpz_t f_g;
	double a, b, k, answer;
	char temp1[30],temp2[30];
	int size;

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
	for (j = 0; j < 255; j++) {
		if (buffer[j] == '=') {
			strncpy(temp2, buffer + i + 1, j - i - 1);
			b = atof(temp2);
			printf("b = %f\n", b);
			break;
		}
	}

	if (buffer[i] == '+'){
		answer = a+b;
		sprintf(ans, "Answer = %f\n", answer);
		write(newsockfd, ans, strlen(ans));
		printf("answer = %f\n", answer);
	}
	if (buffer[i] == '-'){
		answer = a-b;
		sprintf(ans, "Answer = %f\n", answer);
		write(newsockfd, ans, strlen(ans));
		printf("answer = %f\n", answer);
	}
	if (buffer[i] == '*'){
			answer = a*b;
			//mpz_mul (answer,a,b);
			sprintf(ans, "Answer = %f\n", answer);
			write(newsockfd, ans, strlen(ans));
			printf("answer = %f\n", answer);
	}
	if (buffer[i] == '/'){
			answer = a/b;
			sprintf(ans, "Answer = %f\n", answer);
			write(newsockfd, ans, strlen(ans));
			printf("answer = %f\n", answer);
	}
	if (buffer[i] == '#'){
			if (a<0){
				write(newsockfd,"Error, input Root >= 0 !\n",strlen("Error, input Root > 0 !\n"));
			}
			else {
				answer = sqrt(a);
				sprintf(ans, "Answer = %f\n", answer);
				write(newsockfd, ans, strlen(ans));
				printf("sqrt = %f\n", a);

			}
	}
	if (buffer[i] == '!'){
		if (f<=0){
			write(newsockfd,"Error, input Factorial > 0 !\n",strlen("Error, input Factorial > 0 !\n"));
		}
		else {
			answer = factorial(f);
			sprintf(ans, "Answer = %f\n", answer);
			write(newsockfd, ans, strlen(ans));
			printf("factorial = %d\n", factorial(f));
		}
	}
	//sprintf(ans, "Answer = %f\n", answer);
	//write(newsockfd, ans, strlen(ans));
	//printf("answer = %f\n", answer);
}

int main(int argc, char *argv[]) {
	int sockfd, newsockfd, portno, clilen;
	char buffer[256], ans[30];
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

	/* Accept actual connection from the client */

	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0) {
		perror("ERROR on accept");
		exit(1);
	}
	/* If connection is established then start communicating */
	//n = write(newsockfd,"Input expression\n",strlen("Input expression\n"));
	bzero(buffer, 256);
	n = read(newsockfd, buffer, 255);
	if (n < 0) {
		perror("ERROR reading from socket");
		exit(1);
	}

	printf("Received: %s\n", buffer);
	calculation(buffer, ans, n, newsockfd);
	//n = write(newsockfd, ans, strlen(ans));

	/* Write a response to the client */
	 if (n < 0)
	 {
		perror("ERROR writing to socket");
		exit(1);
	 }
	return 0;
}

