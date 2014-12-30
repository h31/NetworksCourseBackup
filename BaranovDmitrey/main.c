#include <netinet/in.h>
#include "sc_menu.h"

int main(int argc, char** argv)
{
    int listener;
    int sock;
    struct sockaddr_in addr;
    pthread_t ppid, spid;

    listener = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(listener, (struct sockaddr_in *)&addr, sizeof(addr));

    listen(listener, MAXCLIENTS);
    pthread_create(&ppid, NULL, server_menu, listener);
    while(1)
    {
        sock = accept(listener, NULL, NULL);
        pthread_create(&spid, NULL, client_menu, sock);
    }

    return 0;
}