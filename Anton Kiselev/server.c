
/* Sample TCP server */

/*Create by Anton Kiselyov*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
//Хеш-функция, используемая для проверки данных польховатля и пароля
unsigned int HashH37(const char * str)
{
	unsigned int hash = 0;
	for( ; *str; str++)
		hash = (hash * 1664525) + (unsigned char)(*str) + 1013904223;
	return hash;
}
//Вывод сообщения об ошибке
void error(const char *msg)
{
    perror(msg);
    exit(1);
}
//Вызов и выполнение команд терминала
char* callterminal(char* command)
{
    FILE *fp;
    int status;
    char path[256];
    char answer[256];
    fp = popen(command, "r");
    if (fp == NULL)
        /* Handle error */;
    while (fgets(path, 256, fp) != NULL)
    {
        //printf("%s", path);
    }
    status = pclose(fp);
    if (status == -1) {
        /* Error reported by pclose() */
        return "Error with executing of command";
    } else {
    /* Use macros described under wait() to inspect `status' in order
       to determine success/failure of command executed by popen() */
        char *p = answer;
        return p;
    }

}
//Для выполнения клиентских команд
void doprocessing (int socket)
{
    int n;
    char buffer[256];
    bzero(buffer,256);
    n = read(socket,buffer,255);
    printf("%s\n",buffer);
    if (n < 0)
    {
        perror("ERROR reading from socket");
        exit(1);
    }
    printf("Here is the message: %s\n",buffer);
	char answer[256];
	strcpy(answer,callterminal(buffer));
    n = write(socket,answer,18);
    if (n < 0)
    {
        perror("ERROR writing to socket");
        exit(1);
    }
}
//Запуск сервера
int main(int argc, char *argv[])
{
    /* int sockfd, newsockfd, portno;
     socklen_t clilen;
     pid_t pid;
     char answer[1024];
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {//указать порт
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,//привязка сервера к ресурсам
              sizeof(serv_addr)) < 0)
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd,//Открытие нового сокета
                 (struct sockaddr *) &cli_addr,
                 &clilen);
     if (newsockfd < 0)
          error("ERROR on accept");
     bzero(buffer,256);
     n = read(newsockfd,buffer,256);
     if (n < 0)
    	 error("ERROR reading from socket");
     printf("Here is the message: %s\n",buffer);

     FILE *fp;
     int status;
     char path[256];
     fp = popen(buffer, "r");
     if (fp == NULL)
    	 error("Failed to execute a command in the terminal\n");
     while (fgets(path, 256, fp) != NULL)
     {
         printf("%s", path);
         bzero(buffer,256);
         strncpy(buffer,path,strlen(path)-1);
         strcat(answer,buffer);
     }
     n = write(newsockfd,answer,strlen(answer)-1);
     if (n < 0)
    	 error("ERROR writing to socket");

     status = pclose(fp);
     if (status == -1) {
         error("Error with executing of command\n");
     }
     close(newsockfd);
     close(sockfd);
     return 0;*/

		FILE *in,*out;
		char c;

		in=fopen("UsersAndPassword","r");//Тут еще нужно бы было проверять возвращаемое значение при открытии файлов

		while ( (c=fgetc(in) ) != EOF)//считываем символ из файла data в переменную c до символа EOF.
		{
			printf("%c",c);
		}
		printf("\n");
		fclose(in);
}
