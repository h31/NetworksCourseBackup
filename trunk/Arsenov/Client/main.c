

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <string.h>
#include <errno.h>

ssize_t                         /*cbnsdf. */
readn(int fd, void *vptr, size_t n)
{
    size_t  nleft;
    ssize_t nread;
    char   *ptr;

    ptr = vptr;
    nleft = n;
    while (nleft > 0) {
        if ( (nread = read(fd, ptr, nleft)) < 0) {
            if (errno == EINTR)
                nread = 0;      /* and call read() again */
            else
                return (-1);
        } else if (nread == 0)
            break;              /* EOF */

        nleft -= nread;
        ptr += nread;
    }
    return (n - nleft);         /* return >= 0 */
}

/*
 *
 */
int main(int argc, char** argv) {
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];

    const int portno = 5000;
    /* Create a socket point */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("ERROR opening socket");
        exit(1);
    }
    server = gethostbyname("localhost");
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
           (char *)&serv_addr.sin_addr.s_addr,
                server->h_length);
    serv_addr.sin_port = htons(portno);

    /* Now connect to the server */
    if (connect(sockfd,&serv_addr,sizeof(serv_addr)) < 0)
    {
         perror("ERROR connecting");
         exit(1);
    }

    while (1) {
        /* Now ask for a message from the user, this message
         * will be read by server
         */
        printf("Please enter the command: ");
        bzero(buffer, 256);
        fgets(buffer, 255, stdin);
        int buffer_len = strlen(buffer);

        if (buffer_len == 0) {
            perror("Empty message, try again");
            break;
        }
        // Remove EOL
        buffer[strlen(buffer) - 1] = '\0';
        buffer_len--;
        /* Send length to the server */
        write(sockfd, &buffer_len, sizeof(int));
        /* Send message to the server */
        n = write(sockfd, buffer, buffer_len);
        if (n < 0) {
            perror("ERROR writing to socket");
            exit(1);
        }
        /* Now read server response */
        read(sockfd, &buffer_len, sizeof(int));
        bzero(buffer, 256);
        n = readn(sockfd, buffer, buffer_len);
        if (n < 0) {
            perror("ERROR reading from socket");
            exit(1);
        }
        printf("%s\n", buffer);
    }
    return 0;

}



