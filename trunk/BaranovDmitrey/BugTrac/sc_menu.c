#include "sc_menu.h"

struct bug
{
    int id;
    char dev[MAXLINE];
    char project[MAXLINE];
    //int accepted;
    int solved;
    char info[MAXLINE];
};

struct client
{
    char username[MAXLINE];
    int perm;
    int client_socket;
};

void addBug(struct bug bugs[], int bugsize, int id, char *username, char *msg);
void printDevBugs(int socket, struct bug bugs[], char *username, int bugsize);
void printBugs(int socket, struct bug bugs[], int bugsize);
void solvebug(struct bug bugs[], int bugsize, int id, int perm);
void printSolvedBugs(int socket, struct bug bugs[], int bugsize);


struct client users[MAXCLIENTS];
struct bug bugs[MAXBUGS];

void server_menu(int socket)
{
    printf("Server started!\n Enter command(? for help):");


    char choose[MAXLINE];

    while(1)
    {
        scanf("%s", choose);
        if(strcmp(choose, "list")==0) printusers();
        else if(strcmp(choose, "exit")==0) exit(2);
        else if(strcmp(choose, "?")==0)
        {
            printf("list - print all users\n");
            printf("drop <username> - drops \'username\'\n");
            printf("reload - for reload data\n");
            printf("exit - close server\n");
        }
        else if(strcmp(choose, "drop")==0)
        {
            char username[MAXLINE];
            scanf("%s", username);
            dropuser(username);
            printf("%s dropped.\n", username);
        }
        else if(strcmp(choose, "reload")==0)reload();
        else printf("No such command!\n");
    }

}

void client_menu(int socket)
{
    int bytes;
    int logged = 0;
    char buf[1024];
    int perm = 2;
    int id;
    char *username;
    username = malloc(MAXLINE * sizeof(char));
    strcpy(username, " ");
    char *dev;
    char *info;
    char *token;

    while(1)
    {
		Send(socket, "Welcome to the BugTracServ!\n");
		Send(socket, "Now you can use next command:\n <login> - for login\n <add>   - add the bug\n <show>  - to show the bugs\n <solve> - to solve the bug\n");
        if((bytes = readline(socket, buf, 1024)) <= 0) break;
        token = strtok(buf, " ");
        if(strcmp(token, "login") == 0)
        {
            if(logged == 1)
            {
                Send(socket, "Already logged in!\r\n");
                continue;
            }
            if((token = strtok(NULL, " ")) == NULL)
            {
                Send(socket, "Not enouth args. Please use \"login <username> <permission>\" format!\r\n");
                continue;
            }
            strcpy(username, token);                                                             //username
            if((token = strtok(NULL, " ")) == NULL)
            {
                Send(socket, "Not enouth args. Please use \"login <username> <permission>\" format!\r\n");
                continue;
            }
            perm = atoi(token);                                                                        //permissions
            adduser(username, socket, perm);
			Send(socket, "User has logged!\n");
            logged = 1;                                                                                          //in system?
        }
        else if(strcmp(token, "add") == 0 )                                           //add bug
        {
            if(perm == 0)
            {
                if((token = strtok(NULL, " ")) == NULL)
                {
                    Send(socket, "Not enouth args. Please use \"add <bug_id> <dev_for> <bug_info>\" format!\r\n");
                    continue;
                }
                id = atoi(token);

                if((token = strtok(NULL, " ")) == NULL)
                {
                    Send(socket, "Not enouth args. Please use \"add <bug_id> <dev_for> <bug_info>\" format!\r\n");
                    continue;
                }
                dev = token;

                if((token = strtok(NULL, " ")) == NULL)
                {
                    Send(socket, "Not enouth args. Please use \"add <bug_id> <dev_for> <bug_info>\" format!\r\n");
                    continue;
                }
                info = token;

                addBug(bugs, MAXBUGS, id, dev, info);
				Send(socket, "Bug has added!\n");
            }
            else
            {
                Send(socket, "This options for testers only.\r\n");
                continue;
            }
        }
        else if(strcmp(token, "show") == 0 ) //show the bugs
        {
            if(strcmp(username, " ") == 0)
            {
                Send(socket, "You must be logged in to see the bugs.\r\n");
                continue;
            }

            if(perm == 1)
            {
                printDevBugs(socket, bugs, username, MAXBUGS);
            }
            else
            {
                if((token = strtok(NULL, " ")) == NULL)
                {
                    printBugs(socket, bugs, MAXBUGS);
                    continue;
                }

                if(strcmp(token, "s\r\n") == 0) printSolvedBugs(socket, bugs, MAXBUGS);
            }
        }
        else if(strcmp(token, "solve") == 0)                                    //def bug_status
        {
            if(perm == 1)     //for the dev
            {
                if((token = strtok(NULL, " ")) == NULL)
                {
                    Send(socket, "Not enouth args. Please use \"solve <bug_id>\" format!\r\n");
                    continue;
                }
                id = atoi(token);

                solvebug(bugs, MAXBUGS, id, perm);
            }
            else             //for the tester
            {
                if((token = strtok(NULL, " ")) == NULL)
                {
                    Send(socket, "Not enouth args. Please use \"solve <bug_id>\" format!\r\n");
                    continue;
                }
                id = atoi(token);

                solvebug(bugs, MAXBUGS, id, perm);
            }
        }
        else if(strcmp(token, "exit") == 0)
        {
            dropuser(username);
        }
        else
        {
            Send(socket, "No such command!\r\n");
        }
    }
}

void printusers()
{
    for(int count=0;count<MAXCLIENTS;count++)
    {
        printf("username : %s\n", users[count].username);
        printf("permissions : %i\n\n", users[count].perm);
    }
}

void dropuser(char *username)
{
    for(int count=0;count<MAXCLIENTS;count++)
    {
        if(strcmp(users[count].username, username) == 0)
        {
            send(users[count].client_socket, "kicked from server!", 18, 0);
            shutdown(users[count].client_socket, 2);
            close(users[count].client_socket);
            strcpy(users[count].username, "");
            users[count].perm = 0;
            users[count].client_socket = 0;
        }
    }
}
int adduser(char *username, int socket, int perm)
{
    for(int count=0;count<MAXCLIENTS;count++)
    {
        if(strcmp(users[count].username, username) == 0) return 0;
        else
            if(strcmp(users[count].username,"") == 0)
            {
                strcpy(users[count].username, username);
                users[count].perm = perm;
                users[count].client_socket = socket;
                return 1;
            }
    }
}

void addBug(struct bug bugs[], int bugsize, int id, char *username, char *msg)
{
    for(int count=0;count<bugsize;count++)
    {
        if(bugs[count].id == 0)
        {
            bugs[count].id = id;
            strcpy(bugs[count].dev, username);
            strcpy(bugs[count].info, msg);
            break;
        }
    }
}

void printDevBugs(int socket, struct bug bugs[], char *username, int bugsize)
{
    char ID[10];
    char *msg;
    msg = malloc(sizeof(char));
    for(int count=0;count<bugsize;count++)
    {
        if(strcmp(bugs[count].dev, username)==0)
        {
            if(bugs[count].solved == 0)
            {
                sprintf(ID, "%d", bugs[count].id);
                strcat(msg, ID);
                strcat(msg, " ");
                strcat(msg, bugs[count].info);
                Send(socket, msg);
                *msg = 0;
            }
        }
    }
    Send(socket, ".\r\n");
}

void loadBugBase(struct bug bugs[])
{
	FILE *fp;
	char buffer[20];
	char *token;
	fp = fopen("/home/rimskii/Desktop/bugs_file.txt", "r");
	fscanf(fp, "%s", buffer);

	for(int i=0; i<20; i++)
	{
		token = strtok(buffer, ":");

	}

	//printf("Read Buffer: %s\n", %buffer );
	fclose(fp);
}

void printBugs(int socket, struct bug bugs[], int bugsize)
{
    char ID[10];
    char *msg;
    msg = malloc(sizeof(char));
    for(int count = 0; count < bugsize; count++)
    {
        if(bugs[count].id != 0)
        {
            sprintf(ID, "%d", bugs[count].id);
            strcat(msg, ID);
            strcat(msg, " ");
            strcat(msg, bugs[count].dev);
            strcat(msg, " ");
            strcat(msg, bugs[count].info);
            Send(socket, msg);
            *msg = 0;
        }
    }
    Send(socket, ".\r\n");
}

void reload()
{
    //do some
}

void printSolvedBugs(int socket, struct bug bugs[], int bugsize)
{
    char ID[10];
    char *msg;
    msg = malloc(sizeof(char));
    for(int count = 0; count < bugsize; count++)
    {
        if(bugs[count].id != 0)
        {
            if(bugs[count].solved == 1)
            {
                sprintf(ID, "%d", bugs[count].id);
                strcat(msg, ID);
                strcat(msg, " ");
                strcat(msg, bugs[count].dev);
                strcat(msg, " ");
                strcat(msg, bugs[count].info);
                Send(socket, msg);
                *msg = 0;
            }
        }

    }
    Send(socket, ".\r\n");
}

void solvebug(struct bug bugs[], int bugsize, int id, int perm)
{
    for(int count=0;count<bugsize;count++)
    {
        if(bugs[count].id == id)
        {
            if(perm == 1)
            {
                bugs[count].solved = 1;
            }
            else
            {
                bugs[count].id = 0;
                strcpy(bugs[count].dev, "");
            }
        }
    }
}

size_t readline(int fd, void *vptr, size_t maxlen)
{
    size_t n, rc;
    char c, *ptr;

    ptr = vptr;
    for(n=0;n<maxlen;n++)
    {
        again:
        if((rc = read(fd, &c, 1)) == 1)
        {
            *ptr++=c;
            if(c == '\n') break;
        }
        else if(rc == 0)
        {
            if(n==1) return 0;
            else break;
        }
        else
        {
            if(errno == EINTR) goto again;
            return -1;
        }
    }

    *ptr = 0;
    return n;
}

int Send(int socket, char *msg)
{
    int bytes;
    if((bytes = write(socket, msg, strlen(msg))) <0) return -1;

    return bytes;
}
