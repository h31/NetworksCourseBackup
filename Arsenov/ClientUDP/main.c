
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <string.h>
#include <assert.h>

int send_msg_num = 0;
int recv_msg_num = 0;
int session_id = -1;

void send_thread(void* arg) {
	int sockfd = *(int*)arg;
	char buffer[256];
	char buffer_backup[256];
	char* text_buffer = buffer + 8;
	int n;

	while (1) {
        /* Now ask for a message from the user, this message
         * will be read by server
         */
        // printf("Please enter the command: ");
        bzero(text_buffer, 256);
        fgets(text_buffer, 255 - 8, stdin);

		printf(">> %d %d\n", send_msg_num, recv_msg_num);

		if ((send_msg_num != 0) && (send_msg_num > recv_msg_num+1)) {
			write(sockfd, buffer_backup, strlen(buffer_backup + 8) + 8);
		}

        int buffer_len = strlen(text_buffer);

        if (buffer_len == 0) {
            perror("Empty message, try again");
            break;
        }
        // Remove EOL
        text_buffer[buffer_len - 1] = '\0';
        buffer_len--;

		/* Send length to the server */
        *(int*)buffer = send_msg_num;
        *(int*)(buffer + 4) = session_id;

		bcopy(buffer, buffer_backup, 256);

		if (strncmp(text_buffer, "NO_REQ", strlen(text_buffer)) == 0) {
			printf("Imitate a request loss\n");
			send_msg_num++;
			continue;
		}
		if (strncmp(text_buffer, "NO_RESP", strlen(text_buffer)) == 0) {
			printf("Imitate a response loss\n");
			*(int*)buffer = send_msg_num-1;
			recv_msg_num--;
		}
        /* Send message to the server */
        n = write(sockfd, buffer, strlen(text_buffer) + 8);
        if (n < 0) {
            perror("ERROR writing to socket");
            exit(1);
        }
		send_msg_num++;
	}
}

void recv_thread(void* arg) {
	int sockfd = *(int*)arg;
	char buffer[256];
	char* text_buffer = buffer + 8;
	int new_session_id = -1;
	int n;

	while (1) {
        /* Now read server response */
        bzero(buffer, 256);
        n = read(sockfd, buffer, 255);
        if (n < 0) {
            perror("ERROR reading from socket");
            exit(1);
        }
        // printf("Receiving message length %d\n", n);

        recv_msg_num = *(int*)buffer;
        new_session_id = *(int*)(buffer + 4);
        printf("<< Received message with num %d session %d\n", recv_msg_num, new_session_id);
        if (new_session_id == session_id) {
            // assert(recv_num == msg_num);
        }
        session_id = new_session_id;
        printf("<< %s\n", text_buffer);
    }
}

/*
 * 
 */
int main(int argc, char** argv) {
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
	pthread_t recv_pth, send_pth;

    const int portno = 5000;
    /* Create a socket point */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("ERROR opening socket");
        exit(1);
    }
    server = gethostbyname("192.168.56.1");
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

	pthread_create(&recv_pth,
            NULL,
            recv_thread,
            (void*) &sockfd);

	pthread_create(&send_pth,
            NULL,
            send_thread,
            (void*) &sockfd);
	pthread_join(send_pth, NULL);
    
    return 0;

}
