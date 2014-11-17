#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <math.h>
#include <gmp.h>
#include <gmp-i386.h>

void calculation(char *buffer, char *ans, int n, int newsockfd) {
	int i, j;
	//long int f;
	//mpf_t f;
	mpf_t a, b, k, f, answer;
	mpf_init(a);
	mpf_init(b);
	mpf_init(k);
	mpf_init(f);
	mpf_init(answer);

	//double a, b, k, answer;
	char temp1[30],temp2[30];
	int size;

	for (i = 1; i < 255; i++) {
		//sqrt
		if (buffer[i] == '#'){
			strncpy(temp1, buffer, i);
			//a = atof(temp1);
			gmp_sscanf(temp1, "%F", &a);
			gmp_printf("k = %F\n", &a);
			break;
		}
		// *, /, +,-
		if ((buffer[i] == '*') ||(buffer[i] == '/') ||(buffer[i] == '+') || (buffer[i] == '-'))  {
				bzero(temp1,30);
				strncpy(temp1, buffer, i);
				//a = atof(temp1);
				printf("temp1 = %s\n", temp1);
				gmp_sscanf(temp1, "%F", &a);
				gmp_printf("a = %F\n", a);
				break;
		}
	}
	//second number
	for (j = 0; j < 255; j++) {
		if (buffer[j] == '=') {
			strncpy(temp2, buffer + i + 1, j - i - 1);
			//b = atof(temp2);
			printf("temp2 = %s\n", temp2);
			gmp_sscanf(temp2, "%F", &b);
			gmp_printf("b = %F\n", b);
			break;
		}
	}

	if (buffer[i] == '+'){
		//answer = a+b;
		mpf_add(answer,a,b);
		sprintf(ans, "Answer = %f\n", answer);
		write(newsockfd, ans, strlen(ans));
		gmp_printf("answer = %F\n", answer);
	}
	if (buffer[i] == '-'){
		//answer = a-b;
		mpf_sub(answer,a,b);
		sprintf(ans, "Answer = %f\n", answer);
		write(newsockfd, ans, strlen(ans));
		gmp_printf("answer = %F\n", answer);
	}
	if (buffer[i] == '*'){
			//answer = a*b;
			mpf_mul(answer, a, b);
			sprintf(ans, "Answer = %f\n", answer);
			write(newsockfd, ans, strlen(ans));
			gmp_printf("answer = %F\n", answer);
	}
	if (buffer[i] == '/'){
			//answer = a/b;
			mpf_div(answer,a,b);
			sprintf(ans, "Answer = %f\n", answer);
			write(newsockfd, ans, strlen(ans));
			gmp_printf("answer = %F\n", answer);
	}
	if (buffer[i] == '#'){
			if (a<0){
				write(newsockfd,"Error, input Root >= 0 !\n",strlen("Error, input Root > 0 !\n"));
			}
			else {
				//answer = sqrt(a);
				write(newsockfd,"Wait for computation of a Root\n",strlen("Wait for computation of a Root\n"));
				mpf_sqrt(answer,a);
				sprintf(ans, "Answer = %f\n", answer);
				write(newsockfd, ans, strlen(ans));
				gmp_printf("sqrt = %F\n", a);

			}
	}
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

	 //mpf_clear (a);
	 //mpf_clear (b);
	 /*while (1) {

	         newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	         if (newsockfd < 0) {
	             perror("ERROR on accept");
	             exit(1);
	         }

	        // pthread_create(&t2, NULL, thread2, newsockfd);

	     }*/
	return 0;
}

