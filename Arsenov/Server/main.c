/*
 * main.c
 *
 *  Created on: Oct 16, 2014
 *      Author: user
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

struct item {
    char* name;
    unsigned int price;
    unsigned int count;
};

void my_write(int conn_socket, const void* send_buff, size_t size) {
    int n;
    n = write(conn_socket, send_buff, size);
    if (n < 0)
    {
        perror("ERROR writing to socket");
        exit(1);
    }
}

void show(int items_num, struct item* items, int conn_socket) {
    unsigned int i;
    char send_buff[256];

    for (i=0; i < items_num; i++) {
        snprintf(send_buff, 255, "%d %s %d %d\n", //
                i, //
                items[i].name, //
                items[i].price, //
                items[i].count); //

        my_write(conn_socket,send_buff,strlen(send_buff));
    }
}

int main( int argc, char *argv[] )
{
    int server_socket, conn_socket, portno, clilen;
    char recv_buff[256];
    char send_buff[256];
    struct sockaddr_in serv_addr, cli_addr;
    int  n;
    unsigned int i;
    unsigned int req_item;
    unsigned int sent_bytes;

    int items_num = 2;
    const int max_items = 20;
    struct item items[max_items];

    items[0].name = "Pizza";
    items[0].price = 100;
    items[0].count = 10;
    items[1].name = "iPhone";
    items[1].price = 100500;
    items[1].count = 15;

    /* First call to socket() function */
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        perror("ERROR opening socket");
        exit(1);
    }
    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 5002;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    const int on = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );

    /* Now bind the host address using bind() call.*/
    if (bind(server_socket, (struct sockaddr *) &serv_addr,
                          sizeof(serv_addr)) < 0)
    {
         perror("ERROR on binding");
         exit(1);
    }

    /* Now start listening for the clients, here process will
    * go in sleep mode and will wait for the incoming connection
    */
    listen(server_socket,5);
    clilen = sizeof(cli_addr);

    while(1) {
        /* Accept actual connection from the client */
        conn_socket = accept(server_socket, (struct sockaddr *)&cli_addr,
                                    &clilen);
        const int max_cart = 20;
        int cart_item_num = 0;
        struct item cart[max_cart];

        if (conn_socket < 0)
        {
            perror("ERROR on accept");
            exit(1);
        }
        /* If connection is established then start communicating */

        while (1) {
            bzero(recv_buff,256);
            n = read( conn_socket,recv_buff,255 );
            if (n <= 0)
            {
                perror("ERROR reading from socket");
                exit(1);
            }
            printf("Received: %s\n",recv_buff);
            if (strcmp(recv_buff, "close\n") == 0) { //
                printf("Close\n");
                close(conn_socket); //
                break; //
            }

            if (strcmp(recv_buff, "show index\n") == 0) {
                show(items_num, items, conn_socket);
                continue;
            }

            if (strcmp(recv_buff, "show cart\n") == 0) {
                show(cart_item_num, cart, conn_socket);
                continue;
            }

            if (sscanf(recv_buff, "buy %d\n", &req_item) == 1) {
                if (req_item >= items_num) {
                    my_write(conn_socket, "No such item\n",
                            strlen("No such item\n"));
                } else {
                    snprintf(send_buff, 255, "Item %s purchased\n",
                            items[req_item].name);

                    my_write(conn_socket, send_buff, strlen(send_buff));

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

            if (sscanf(recv_buff, "add %s %d %d", name, //
                    &price, //
                    &count) == 3) {
                if (strlen(name) > 20) {
                    my_write(conn_socket, "Too long name\n",
                                                strlen("Too long name\n"));
                } else {
                    snprintf(send_buff, 255, "Item %s added\n", name);

                    my_write(conn_socket, send_buff, strlen(send_buff));

                    struct item new_item = { name, price, count };
                    items[items_num] = new_item;
                    items_num++;
                }
                continue;
            }
        }
    }
    return 0;
}
