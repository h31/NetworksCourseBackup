#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <math.h>
#include <gmp.h>
#define max_conn 10
#define max_delays_num 100
#define BUFLEN 1000

long int fact_args[max_delays_num];
pthread_t fact_thread[max_delays_num];
long int fact_res[max_delays_num];

double sqrt_args[max_delays_num];
pthread_t sqrt_thread[max_delays_num];
double sqrt_res[max_delays_num];

void* threads(void* arg) {
	int newsockfd = *(int*)arg;
	char buffer[BUFLEN], ans[BUFLEN];
	int n;
	while (1){
		bzero(buffer, BUFLEN);
		n = read(newsockfd, buffer, BUFLEN-1);
		calculation(buffer, ans, n, newsockfd);
	}
}

long int factorial(long int x) {
	return !x ? 1 : x * factorial(x - 1);
}

void run_fact(void* args) {
	int* sockfd_p = (int*) args;
	int sockfd = *sockfd_p;
	fact_res[sockfd] = factorial(fact_args[sockfd]);
}

void run_sqrt(void* args) {
	int* sockfd_p = (int*) args;
	int sockfd = *sockfd_p;
	sqrt_res[sockfd] = sqrt(sqrt_args[sockfd]);
}

void calculation(char *buffer, char *ans, int n, int newsockfd) {
	int i, j;
	long int f,ansf;
	double a, b, k, answer;
	char temp1[BUFLEN],temp2[BUFLEN];
	int size;
	printf("Received: %s\n", buffer);

	bzero(temp1, BUFLEN);
	bzero(temp2, BUFLEN);

	if (strncmp(buffer, "CHECK_F", 7) == 0) {
		if (fact_res[newsockfd] != 0) {
			sprintf(temp1, "Factorial = %ld\n", fact_res[newsockfd]);
			write(newsockfd, temp1, strlen(temp1));
			printf("factorial = %ld\n", fact_res[newsockfd]);
			fact_res[newsockfd] = 0;
		}
		else {
			write(newsockfd, "Not yet", strlen("Not yet"));
		}
		return;
	}

	if (strncmp(buffer, "CHECK_S", 7) == 0) {
		if (sqrt_res[newsockfd] != 0) {
			sprintf(temp1, "Sqrt = %lf\n", sqrt_res[newsockfd]);
			write(newsockfd, temp1, strlen(temp1));
			printf("sqrt = %lf\n", sqrt_res[newsockfd]);
			sqrt_res[newsockfd] = 0;
		}
		else {
			write(newsockfd, "Not yet", strlen("Not yet"));
		}
		return;
	}

	for (i = 1; i < (BUFLEN-1); i++) {
		//factorial
		if (buffer[i] == '!'){
			strncpy(temp1, buffer, i);
			f = atoi(temp1);
			break;
		}
		//sqrt
		if (buffer[i] == '#'){
			strncpy(temp1, buffer, i);
			a = atof(temp1);
			break;
		}
		// *, /, +,-
		if ((buffer[i] == '*') ||(buffer[i] == '/') ||(buffer[i] == '+') || (buffer[i] == '-'))  {
			strncpy(temp1, buffer, i);
			a = atof(temp1);
			break;
		}
	}
	//end expression
	for (j = 0; j < (BUFLEN-1); j++) {
		if (buffer[j] == '=') {
			strncpy(temp2, buffer + i + 1, j - i - 1);
			b = atof(temp2);
			break;
		}
	}

	if (buffer[i] == '+'){
		answer = a+b;
		sprintf(ans, "Answer = %lf\n", answer);
		write(newsockfd, ans, strlen(ans));
	}
	if (buffer[i] == '-'){
		answer = a-b;
		sprintf(ans, "Answer = %lf\n", answer);
		write(newsockfd, ans, strlen(ans));
	}
	if (buffer[i] == '*'){
		answer = a*b;
		//mpz_mul (answer,a,b);
		sprintf(ans, "Answer = %lf\n", answer);
		write(newsockfd, ans, strlen(ans));
	}
	if (buffer[i] == '/'){
		if (b==0){
			write(newsockfd,"Error, input divider != 0 !\n",strlen("Error, input divider != 0 !\n"));
		}
		else {
			answer = a/b;
			sprintf(ans, "Answer = %lf\n", answer);
			write(newsockfd, ans, strlen(ans));
		}
	}
	if (buffer[i] == '#'){
		if (a<0){
			write(newsockfd,"Error, input Root >= 0 !\n",strlen("Error, input Root > 0 !\n"));
		}
		else {
			sqrt_args[newsockfd] = a;
			sqrt_res[newsockfd] = 0;
			pthread_create(&(sqrt_thread[newsockfd]),
					NULL,
					run_sqrt,
					(void*) &newsockfd);
			write(newsockfd, "Wait\n", strlen("Wait\n"));
		}
	}
	if (buffer[i] == '!'){
		if (f<=0){
			write(newsockfd,"Error, input Factorial > 0 !\n",strlen("Error, input Factorial > 0 !\n"));
		}
		else {
			fact_args[newsockfd] = f;
			fact_res[newsockfd] = 0;
			pthread_create(&(fact_thread[newsockfd]),
					NULL,
					run_fact,
					(void*) &newsockfd);
			write(newsockfd, "Wait\n", strlen("Wait\n"));
		}
	}
}

int main(int argc, char *argv[]) {
	int sockfd, newsockfd, portno, clilen;
	char buffer[BUFLEN], ans[BUFLEN];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	pthread_t worker_thread[max_conn];
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
	/* Now bind the host address using bind	char buffer[256], ans[100];
	 * () call.*/
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("ERROR on binding");
		exit(1);
	}

	/* Now start listening for the clients, here process will
	 * go in sleep mode and will wait for the incoming connection
	 */
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);


	/* If connection is established then start communicating */
	while (1){
		int worker_count = 0;
		int worker_socket[max_conn];
		int i;
		while (worker_count < max_conn) {
			printf("Waiting\n");
			/* Accept actual connection from the client */
			newsockfd = accept(sockfd, (struct sockaddr *) & cli_addr, &clilen);
			if (newsockfd <= 0) {
				perror("ERROR on accept");
				break;
			}
			printf("Connection %d\n", newsockfd-3);
			pthread_create(&(worker_thread[worker_count]),
					NULL,
					threads,
					(void*) &newsockfd);
			worker_count++;
			/*for (i = 0; i < worker_count; i++) {
				pthread_join(worker_thread[i], NULL);
			}*/
		}

	}
	return 0;
}
