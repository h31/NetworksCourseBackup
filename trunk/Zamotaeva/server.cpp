/*
    C socket server example, handles multiple clients using threads
    Compile
    gcc server.c -lpthread -o server
*/

#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread

#include "protocol.h"

USER_INFO uinfo[3];
unsigned int ucount = 0;

//the thread function
void *connection_handler(void *);

int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c;
    struct sockaddr_in server , client;

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc , 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);


    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    pthread_t thread_id;

    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");

        uinfo[ucount].user_id = client_sock; // id polz eto nomer socketa
        ucount++; //uvelich polz

        if( pthread_create( &thread_id , NULL ,  connection_handler , (void*) &client_sock) < 0) //sozdaem potoki s id
        {
            perror("could not create thread");
            return 1;
        }

        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( thread_id , NULL);
        puts("Handler assigned");
    }

    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }

    return 0;
}

/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char *message ,client_message_in[1000], client_message_out[1000];

    CMD * cmd_in = (CMD*)client_message_in; //ykaz na struct cmd, pod struct vudel mesto client_message_in
    CMD * cmd_out = (CMD*)client_message_out;
    USER_LIST *list_out = (USER_LIST*)(client_message_out + sizeof(CMD));// ukaz na buffer nachalo za cmd - skolko polz (cmd_in na cmd)
    USER_INFO *ui_out = (USER_INFO*)(client_message_out + sizeof(CMD) + sizeof(USER_LIST)); // buffer ykaz na spisok polz

    MSG * msg_in = (MSG*)(client_message_in + sizeof(CMD)); // ykaz na msg, posle cmd - komy
    char * msg_in_text = (client_message_in + sizeof(CMD) + sizeof(MSG)); //text soobch


    //Send some messages to the client
    message = "Greetings! I am your connection handler\n";
    write(sock , message , strlen(message));

    message = "Now type something and i shall repeat what you type \n";
    write(sock , message , strlen(message));

    //Receive a message from client
    while( (read_size = recv(sock , cmd_in , sizeof(CMD) , 0)) > 0 )
    {
        if (read_size != sizeof(CMD))
        {
            printf("error cmd recv\n");
            continue;
        }

        memcpy(cmd_out, cmd_in, sizeof(CMD)); //cmd_in serv pol ot klienta, cmd_out otpr klienty

        switch(cmd_in->cmd)
        {
        case CMD_USERLIST:
        {
            memset(list_out, 0, sizeof(USER_LIST));
            list_out->count = ucount - 1; //- ceba

            int usersent = 0;
            for (int i = 0; i < ucount; i++)
            {
                if (uinfo[i].user_id != sock)// ego id sovp c coboi -  ne berem sam on
                {
                    memcpy(&ui_out[usersent], &uinfo[i], sizeof(USER_LIST)); // kopir iz user info v nego
                    usersent++;
                }
            }

            send(sock, client_message_out, sizeof(CMD) + sizeof(USER_LIST) + sizeof(USER_INFO) * ucount - 1, 0);// otprav polz
        }
            break;
        case CMD_MSG:
            recv(sock , msg_in , sizeof(MSG) , 0);
            printf("message for %d, length: %d\n", msg_in->daddr, msg_in->length);
            recv(sock, msg_in_text, msg_in->length, 0);
            printf("Message: %.*s\n", msg_in->length, msg_in_text);

            send(msg_in->daddr, client_message_in, sizeof(CMD) + sizeof(MSG) + msg_in->length, 0); //server otpravl klienty iz buffera client_in
        }
    };

    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }

    return 0;
}
