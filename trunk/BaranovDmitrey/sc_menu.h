#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAXCLIENTS 5
#define MAXLINE 50
#define MAXBUGS 100

struct bug;
struct client;

//Server functions
void server_menu(int socket);
void client_menu(int socket);
void printusers();
void dropuser(char *username);
int adduser(char *username, int socket, int perm);


//Bug tracking functions

//Utility functions
size_t readline(int fd, void *vptr, size_t maxlen);
int Send(int socket, char *msg);