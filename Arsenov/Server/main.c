//*


#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <pthread.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <errno.h>

struct item {
    char* name;
    unsigned int price;
    unsigned int count;
};

void my_write(int conn_socket, const void* send_buff, unsigned int size) {
    int n1, n2;
    n1 = write(conn_socket, &size, sizeof(unsigned int));
    n2 = write(conn_socket, send_buff, size);
    if (n1 < 0 || n2 < 0)
    {
        perror("ERROR writing to socket");
        exit(1);
    }
}

void show(int items_num, struct item* items, int conn_socket) {
    unsigned int i;
    char send_buff[256];
    unsigned int buff_size = 0;

    for (i=0; i < items_num; i++) {
        int written = snprintf(send_buff + buff_size, 255 - buff_size,
                              "%d %s %d %d\n", //
                              i, //
                              items[i].name, //
                              items[i].price, //
                              items[i].count); //
        buff_size += written;
    }

    my_write(conn_socket, send_buff, buff_size);
}

#define max_conn 100
#define max_items 20

int items_num = 2;
struct item items[max_items];

int worker_count = 0;
int worker_socket[max_conn];

ssize_t                         /* Read "n" bytes from a descriptor. */
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

static void* worker(void* arg) {
    int conn_socket = *(int*)arg;
    printf("Worker for %d is up\n", conn_socket);
    char recv_buffer[255];
    char send_buffer[255];
    int recv_msg_len;
    int  n;
    int i;

    const int max_cart = 20;
    int cart_item_num = 0;
    struct item cart[max_cart];

    while (1) {

        n = read(conn_socket, &recv_msg_len, sizeof(int));
       // sleep(10);
        if (n < 0) {
            perror("ERROR getting message length");
            return 0;
        }
        printf("Receiving message with length %d\n", recv_msg_len);

        if (recv_msg_len == 0) {
            my_write(conn_socket, " ", 1);
            continue;
        }

        bzero(recv_buffer, 255);
        n = readn(conn_socket, recv_buffer, recv_msg_len);
        if (n < 0) {
            perror("ERROR reading message");
            return 0;
        }

        printf("Received %s\n", recv_buffer);

        if (strcmp(recv_buffer, "close") == 0) { //
            printf("Close\n");
            break; //
        }

        if (strcmp(recv_buffer, "show index") == 0) {
            sleep(10);
        	show(items_num, items, conn_socket);
            continue;
        }

        if (strcmp(recv_buffer, "show cart") == 0) {
            show(cart_item_num, cart, conn_socket);
            continue;
        }

        unsigned int req_item;

        if (sscanf(recv_buffer, "buy %d", &req_item) == 1) {
            if (req_item >= items_num) {
                my_write(conn_socket, "No such item\n",
                        strlen("No such item\n"));
            } else {
                snprintf(send_buffer, 255, "Item %s purchased\n",
                        items[req_item].name);

                my_write(conn_socket, send_buffer, strlen(send_buffer));

                items[req_item].count--;

                int already_in_cart = 0;

                for (i = 0; i < cart_item_num; i++) {
                    if (cart[i].name == items[req_item].name) {
                        cart[i].count++;
                        already_in_cart = 1;
                        break;
                    }
                }

                if (!already_in_cart) {
                    cart[cart_item_num] = items[req_item];
                    cart[cart_item_num].count = 1;
                    cart_item_num++;
                }
            }
            continue;
        }

        char* name = malloc(21);
        unsigned int price;
        unsigned int count;

        if (sscanf(recv_buffer, "add %s %d %d", name, //
                &price, //
                &count) == 3) {
            if (strlen(name) > 20) {
                my_write(conn_socket, "Too long name\n",
                                            strlen("Too long name\n"));
            } else {
                snprintf(send_buffer, 255, "Item %s added\n", name);

                my_write(conn_socket, send_buffer, strlen(send_buffer));

                struct item new_item = { name, price, count };
                items[items_num] = new_item;
                items_num++;
            }
            continue;
        }

        snprintf(send_buffer, 255, "Incorrect command");

        /* Write a response to the client */
        my_write(conn_socket, send_buffer, strlen(send_buffer));
    }

    shutdown(conn_socket, 2);
    close(conn_socket);
}

void* accept_loop(void* arg) {
    int accept_socket = *(int*)arg;

    pthread_t worker_thread[max_conn];

    int newsockfd;
    int clilen;
    struct sockaddr_in cli_addr;

    int i;

    clilen = sizeof(cli_addr);

    while (worker_count < max_conn) {
        printf("Waiting\n");
        /* Accept actual connection from the client */
        newsockfd = accept(accept_socket, (struct sockaddr *) & cli_addr,
                &clilen);
        if (newsockfd <= 0) {
            perror("ERROR on accept");
            break;
        }
        printf("Connection %d\n", newsockfd);

        worker_socket[worker_count] = newsockfd;

        int* sock = &(worker_socket[worker_count]);

        pthread_create(&(worker_thread[worker_count]),
                NULL,
                worker,
                (void*) sock);

        worker_count++;
    }

    printf("Closed accept socket\n");
    for (i = 0; i < worker_count; i++) {
        shutdown(worker_socket[i], 2);
        close(worker_socket[i]);
    }

    for (i = 0; i < worker_count; i++) {
        pthread_join(worker_thread[i], NULL);
    }
}

/*
 *
 */
int main(int argc, char** argv) {
    int sockfd, portno, clilen;

    struct sockaddr_in serv_addr, cli_addr;
    int  n;
    int optval;

    items[0].name = "Pizza";
    items[0].price = 100;
    items[0].count = 10;
    items[1].name = "iPhone";
    items[1].price = 100500;
    items[1].count = 15;


    char command;

    pthread_t accept_thread;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("ERROR opening socket");
        exit(1);
    }
    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 5000;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
                          sizeof(serv_addr)) < 0)
    {
         perror("ERROR on binding");
         exit(1);
    }

    /* Now start listening for the clients, here process will
    * go in sleep mode and will wait for the incoming connection
    */
    listen(sockfd,5);

    pthread_create(&(accept_thread),
            NULL,
            accept_loop,
            (void*) &sockfd);

    while (1) {
        command = getchar();
        if (command == 'q') {
            break;
        } else if (command == 'd') {
            int client;
            scanf("%d", &client);
            shutdown(worker_socket[client], 2);
            close(worker_socket[client]);
        }
    }

    printf("Exit...\n");
    shutdown(sockfd, 2);
    close(sockfd);
    //pthread_join(accept_thread, NULL);

    printf("Done\n");

    return 0;
}
