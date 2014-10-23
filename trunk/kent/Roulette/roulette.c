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
//#include <sys/types.h>
//#include <sys/socket.h>
//include <netinet/in.h>
#include <netdb.h>
int char_to_int (char *ch)
{
	int n;
	int i;
	char q[3]="123";

	n=atoi(q);
	return n;
}
void game (int sock)
{
	int n;
	char color, line;
	int number, bet;
	char buffer[256];

	int k;
	n = write (sock,"Greetings and welcome!I want to play a game.\n",44);
	n= write (sock,"Enter your bet",14);
	bzero (buffer, 256);
	n= read (sock,buffer,255);
	bet =atoi (buffer);
	n=write(sock, "Bets are made, no more bets! Please, enter a number\n",51);
	bzero (buffer,256);
	n= read (sock, buffer, 255);
	number=atoi (buffer);
	n= write (sock, "Ok! Now enter a color",21);
	bzero (buffer, 256);
	n= read (sock, buffer, 255);
	color ="24";
	printf ("%d -bet , %d- number, %s - color\n",bet, number, color);
//	printf("%d\n", atoi(buffer));
	/*here i cant recieve bet
	bzero(buffer,256);
	n=resv (sock,buffer,256,0);
	printf("%s\n", buffer);*/
//	n=read(sock, buffer,255);
//	number = rand()% 10;
//	n=write (sock,"%d\n",number,2);
}
void doprocessing (int sock)
{
    int n;
    int rand;
    char buffer[256];
    game(sock);
    bzero(buffer,256);

    n = read(sock,buffer,255);
    if (n < 0)
    {
        perror("ERROR reading from socket");
        exit(1);
    }
    printf("Here is the message: %s\n",buffer);
    n = write(sock,"I got your message",18);
    if (n < 0)
    {
        perror("ERROR writing to socket");
        exit(1);
    }
}
int main( int argc, char *argv[] )
{
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int  n;
    pid_t pid;
    /* First call to socket() function */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("ERROR opening socket");
        exit(1);
    }
    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 9999;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
                          sizeof(serv_addr)) < 0)
    {
         perror("ERROR on binding");
         exit(1);
    }
    /* Now start listening for the clients, here
     * process will go in sleep mode and will wait
     * for the incoming connection
     */
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    while (1)
    {
        newsockfd = accept(sockfd,
                (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0)
        {
            perror("ERROR on accept");
            exit(1);
        }
        /* Create child process */
        pid = fork();
        if (pid < 0)
        {
            perror("ERROR on fork");
	    exit(1);
        }
        if (pid == 0)
        {
            /* This is the client process */
            close(sockfd);
            doprocessing(newsockfd);
            exit(0);
        }
        else
        {
            close(newsockfd);
        }
    } /* end of while */
}
