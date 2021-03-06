

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
#include <io.h>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

#define N 80

struct UserFields{
	char username[256];
	char password[256];
	char commands[256];
};
struct ArrFields{
	int size;
	struct UserFields* arr;
};
//Хеш-функция, используемая для проверки данных пользователя и пароля
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
//Отсоединение клиента
void killclient()
{
    system("killall client");
    exit(1);
}
//Считывание содержимого файла пользователей и паролей
struct ArrFields readFile()
{
	struct UserFields usr;
	struct UserFields arr[10];
	struct ArrFields arrfields;
	// Переменная, в которую будет помещен указатель на созданный
	// поток данных
	FILE *mf;
	// Переменная, в которую поочередно будут помещаться считываемые строки
	char str[50];
    //Указатель, в который будет помещен адрес массива, в который считана
	// строка, или NULL если достигнут коней файла или произошла ошибка
	char *estr;

	// Открытие файла с режимом доступа «только чтение» и привязка к нему
	// потока данных
	mf = fopen ("usersandpasswords.txt","r");

	// Проверка открытия файла
	if (mf == NULL)
	{
		printf ("ошибка\n");
	}
	int strcount = 0;
	int size = 0;
	//Чтение (построчно) данных из файла в бесконечном цикле
	while (1)
	{
	   // Чтение одной строки  из файла
	   estr = fgets (str,sizeof(str),mf);
	   //Проверка на конец файла или ошибку чтения
	   if (estr == NULL)
	   {
	      // Проверяем, что именно произошло: кончился файл
	      // или это ошибка чтения
	      if ( feof (mf) != 0)
	      {
	         //Если файл закончился, выводим сообщение о завершении
	         //чтения и выходим из бесконечного цикла
	         printf ("\nЧтение файла закончено\n");
	         break;
	      }
	      else
	      {
	         //Если при чтении произошла ошибка, выводим сообщение
	         //об ошибке и выходим из бесконечного цикла
	         printf ("\nОшибка чтения из файла\n");
	         break;
	      }
	   }
	   if(strcount == 0){
	   		  strncpy(usr.username,str,strlen(str));
	   		  usr.username[strlen(str)-1] = '\0';
	   	  }
	   else if(strcount == 1){
	   		  strncpy(usr.password,str,strlen(str));
	   		  usr.password[strlen(str)-1] = '\0';
	   	  }
	   else if(strcount == 2){
	   		  strncpy(usr.commands,str,strlen(str));
	   		  usr.commands[strlen(str)-1] = '\0';
	   	  }
	   strcount = strcount+1;
	   if(strcount == 3)
	   {
		   strcount = 0;
		   arr[size] = usr;
		   size = size+1;
	   }
	}
	arrfields.size = size;
    arrfields.arr = arr;
	// Закрываем файл
	if ( fclose (mf) == EOF)
		printf ("ошибка\n");
	else printf ("выполнено\n");
	return arrfields;
}
//Вызов и выполнение команд терминала
/*char* callterminal(char* command)
{
    FILE *fp;
    int status;
    char path[256];
    char buffer[256];
    char answer[256] = "";
    fp = popen(command, "r");
    if (fp == NULL)
		 error("Failed to execute a command in the terminal\n");
    while (fgets(path, 256, fp) != NULL)
    {
		 printf("%s", path);
		 bzero(buffer,256);
		 strncpy(buffer,path,strlen(path)-1);
		 strcat(answer,buffer);
    }
    status = pclose(fp);
    if (status == -1) {
        return "Error with executing of command";
    } else {
        return &answer;
    }
}*/
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
	//strcpy(answer,callterminal(buffer));
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
     int sockfd, newsockfd, portno;
	SOCKET sockfd = INVALID_SOCKET;
	SOCKET newsockfd = INVALID_SOCKET;
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
     if (sockfd == INVALID_SOCKET)
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,//привязка сервера к ресурсам
              sizeof(serv_addr)) == SOCKET_ERROR)
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd,//Открытие нового сокета
                 (struct sockaddr *) &cli_addr,
                 &clilen);
     if (newsockfd < 0)
          error("ERROR on accept");
     /*Аутентификация пользователя*/
     int is_auntentity_ok = 0;
     char savebuf[256];
     //do
    // {
    	 n = write(newsockfd,"Please, give me your username and password via blank:",255);
    	 if (n < 0)
    		 error("ERROR writing to socket");

    	 bzero(buffer,256);//Взятие данных от пользователя
    	 n = read(newsockfd,buffer,256);
    	 sleep(5);
    	 if (n < 0)
    		 error("ERROR reading from socket");
    	 printf("Here is the message: %s\n",buffer);
    	 char temp[256];
    	 bzero(temp,256);
    	 bzero(savebuf,256);
    	 strncpy(temp,buffer,strlen(buffer));
    	 struct ArrFields arrfields = readFile();
    	 int i = 0;
    	 int yes = 0;
    	 for(i = 0; i < arrfields.size; i++)
    	 {
    		 char usrpassw [256];
    		 bzero(usrpassw,256);
    		 strcat(usrpassw,arrfields.arr[i].username);
    		 strcat(usrpassw," ");
    		 strcat(usrpassw,arrfields.arr[i].password);
    		 if(strcmp(temp,usrpassw) == 0)
    		 {
    			 yes = 1;
    			 strncpy(savebuf,temp,strlen(temp));
    		 }
    	 }
    	 printf("%i\n",yes);
    	 if(yes == 1)
    	 {
    		 is_auntentity_ok = 1;
    		 n = write(newsockfd,"Ok!",strlen(answer)-1);
    		 if (n < 0)
    		     error("ERROR writing to socket");
    	 }
    	 else
    	 {
    		 n = write(newsockfd,"Polzovatel' and Parol' neverny!",255);
    		 if (n < 0)
    		     error("ERROR writing to socket");
    		 //close(newsockfd);
    		 //killclient();
    	 }
   //  } while(is_auntentity_ok != 1);
     is_auntentity_ok = 0;
	// do
	// {
		 unsigned int res = HashH37(savebuf);//Вычисление хэш-функции
		 printf("%i\n",res);
    	 char ans [256];
    	 bzero(ans,256);
         bzero(buffer,256);
    	 memcpy(buffer, (char*)&res, 3);
    	 strcpy(ans,buffer);
	     printf("%s\n",buffer);
    	 bzero(buffer,256);
	     n = read(newsockfd,buffer,256);
	     printf("%s\n",buffer);
	     if (n < 0)
		     error("ERROR reading from socket");
	     if (strcmp(ans,buffer) == 0)
	     {
    	     n = write(newsockfd,"Hello!",strlen(answer)-1);
    	     if (n < 0)
    		     error("ERROR writing to socket");
    	     is_auntentity_ok = 1;
   	     }
	     else
	     {
	    	 killclient();
    	     /*n = write(newsockfd,"Hash neveren",strlen(answer)-1);
    	     if (n < 0)
    		     error("ERROR writing to socket");*/
   	     }
    // } while(is_auntentity_ok != 1);
     while(1)
     {
    	 bzero(buffer,256);
    	 bzero(answer,256);
    	 n = read(newsockfd,buffer,256);
    	 if (n < 0)
    		 error("ERROR reading from socket");
    	 printf("Here is the message: %s\n",buffer);
    	 char tempbuf[256];
    	 bzero(tempbuf,256);
    	 strncpy(tempbuf,buffer,strlen(buffer)-1);
    	 printf("%s\n",tempbuf);
    	 if(strcmp(tempbuf,"end") == 0)
    	 {
        	 printf("yes\n");
        	 n = write(newsockfd,"Good Buy!",255);
        	 if (n < 0)
        		 error("ERROR writing to socket");
        	 FILE *fp;
        	 int status;
        	 char path[256];
        	 bzero(path,256);
        	 fp = popen("killall client", "r");
        	 if (fp == NULL)
        		 error("Failed to execute a command in the terminal\n");
        	 status = pclose(fp);
        	 if (status == -1) {
        		 error("Error with executing of command\n");
        	 }
        	 closesocket(newsockfd);
    	 }
    	 FILE *fp;
    	 int status;
    	 char path[256];
    	 bzero(path,256);
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
     }
     closesocket(newsockfd);
     closesocket(sockfd);
     return 0;
}
