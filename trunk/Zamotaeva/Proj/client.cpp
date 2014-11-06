#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "protocol.h"

unsigned int id = 0;
char sendline [1000];
char recvline [1000];

char * msgbuf = sendline + sizeof(CMD) + sizeof(MSG);

unsigned int ucount;
USER_INFO * users = 0;

int main(int argc, char**argv)
{
    int sockfd,n;
    struct sockaddr_in servaddr,cliaddr;

    if (argc != 2)
    {
        printf("usage:  client <IP address>\n");
        return 1;
    }

    sockfd=socket(AF_INET,SOCK_STREAM,0);

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=inet_addr(argv[1]);
    servaddr.sin_port=htons(8888);

    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    n = recvfrom(sockfd,recvline,10000,0,NULL,NULL);
    recvline[n]='\0';
    fputs(recvline,stdout);

    n = recvfrom(sockfd,recvline,10000,0,NULL,NULL);
    recvline[n]='\0';
    fputs(recvline,stdout);

    CMD * cmd_in = (CMD*)sendline; //ykaz na struct cmd ykaz na buffer
    memset(cmd_in, 0, sizeof(CMD));
    cmd_in->cmd = CMD_USERLIST; //spisok polz client servery

    sendto(sockfd, cmd_in, sizeof(CMD), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));

    CMD * cmd_out = (CMD*)recvline; //client poluch ot serv  , recvline ykaz na buffer v kotorom commanda USERLIST

    n = recvfrom(sockfd, cmd_out, sizeof(CMD), 0, NULL, NULL);

    if (n != sizeof(CMD))
    {
        printf("wrong CMD struct size\n");
        return 1;
    }

    if (cmd_out->cmd != CMD_USERLIST) //cmd out ykaz na buff , pole tam dolgna buti kommanda
    {
        printf("wrong command\n");
        return 1;
    }

    USER_LIST * userlist = (USER_LIST*)recvline; //chit userlist
    n = recvfrom(sockfd, userlist, sizeof(USER_LIST), 0, NULL, NULL);

    if (n != sizeof(USER_LIST))
    {
        printf("wrong UL struct size\n");
        return 1;
    }

    ucount = userlist->count;//kolvo polz

    printf("%u users online\n", ucount);

    if (ucount > 0)
    {
        if (users)
            delete [] users;

        users = new USER_INFO[ucount];

        for (unsigned int i = 0; i < ucount; i++)
        {
            n=recvfrom(sockfd,&users[i],sizeof(USER_INFO),0,NULL,NULL); //poluc spisok polz

            if (n != sizeof(USER_INFO))
            {
                printf("wrong UI struct size\n");
                return 1;
            }

            printf("%u: user id: %u\n", i, users[i].user_id);
        }
    }

    if (ucount > 0)
    {
        while (fgets(msgbuf, 10000,stdin) != NULL)// chit msg s konsoli
        {
            printf("Type your message:\n");

            memset(cmd_in, 0, sizeof(CMD)); //otrpav msg servery
            cmd_in->cmd = CMD_MSG;
            MSG * msg = (MSG*)(sendline + sizeof(CMD)); //soobh na otprav
            MSG * msg_recv = (MSG*)(recvline + sizeof(CMD)); //poluch soobch
            char* msg_recv_text = (char*)(recvline + sizeof(CMD) + sizeof(MSG));// text soobch
            memset(msg, 0, sizeof(MSG));
            msg->length = strlen(msgbuf) + 1;

            srand(time(0));

            msg->daddr = users[rand() % ucount].user_id;// sluch vub komy pisat

            printf("Message for %d\n", msg->daddr);

            sendto(sockfd, cmd_in, sizeof(CMD) + sizeof(MSG) + msg->length, 0, (struct sockaddr *)&servaddr, sizeof(servaddr)); // otpavl msg

            recvfrom(sockfd, recvline, sizeof(recvline) ,0, NULL, NULL); //chit msg

            printf("REcv message: %.*s", msg_recv->length, msg_recv_text);

        }
    }
    else
    {
        while (1)
        {

        MSG * msg_recv = (MSG*)(recvline + sizeof(CMD)); //poluch soobch
        char* msg_recv_text = (char*)(recvline + sizeof(CMD) + sizeof(MSG));// text soobch

        recvfrom(sockfd, recvline, sizeof(recvline) ,0, NULL, NULL); //chit msg

        printf("REcv message: %.*s", msg_recv->length, msg_recv_text);
        }

    }
}
