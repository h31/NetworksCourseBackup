/*
 * main.c
 *
 *  Created on: Oct 16, 2014
 *      Author: user
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

struct item {
	char* name;
	unsigned int price;
	unsigned int count;
};

int main( int argc, char *argv[] )
{
    int server_socket, conn_socket, portno, clilen;
    char recv_buff[256];
    char send_buff[255];
    struct sockaddr_in serv_addr, cli_addr;
    int  n;
    unsigned int i;
    unsigned int req_item;
    unsigned int sent_bytes;
    char stock[] = "s";

    const int items_num = 2;
    struct item items[items_num];

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
    portno = 5010;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

//    const int on = 1;
//    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );

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
		if (conn_socket < 0)
		{
			perror("ERROR on accept");
			exit(1);
		}
		/* If connection is established then start communicating */
		for (i=0; i < items_num; i++) {
			snprintf(send_buff, 255, "%d %s %d %d\n", //
					i, //
					items[i].name, //
					items[i].price, //
					items[i].count); //

			n = write(conn_socket,send_buff,strlen(send_buff));
			if (n < 0)
			{
				perror("ERROR writing to socket");
				exit(1);
			}
		}


		while (1) {
			bzero(recv_buff,256);
			n = read( conn_socket,recv_buff,255 );
			if (n < 0)
			{
				perror("ERROR reading from socket");
				exit(1);
			}
			printf("Received: %s\n",recv_buff);
			if (strcmp(recv_buff, "close\n") == 0) { //
				close(conn_socket); //
				break; //
			}
			// if (strcmp(recv_buff, "close_all") == 0) { //
			// 	exit(0); //
			// } //

			req_item = atoi(recv_buff); //

			snprintf(send_buff, 255, "Item %s purchased\n", items[req_item].name); //

			n = write(conn_socket, send_buff, strlen(send_buff));
						if (n < 0)
						{
							perror("ERROR writing to socket");
							exit(1);
						}


			items[req_item].count--;

		}
    }
    return 0;
}
