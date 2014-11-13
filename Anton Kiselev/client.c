/*
 * client.c
 *
 *  Created on: 05.11.2014
 *      Author: anton
 */


/* Sample TCP client */
/*Created by Anton*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
//Хэш-функция, используемая для проверки данных пользователя и пароля
unsigned int HashH37(const char * str)
{
	unsigned int hash = 0;
	for(;*str;str++)
		hash = (hash * 1664525) + (unsigned char)(*str) + 1013904223;
	return hash;
}
//Вывод ошибок клиента
void error(const char *msg)
{
    perror(msg);//Открыть сообщение с выводом информации на консоль
    exit(0);
}

int main(int argc, char *argv[])
{
	int pid = getpid();
	 int sockfd, portno, n;//Дескриптор сокета
	 struct sockaddr_in serv_addr;//Адрес сервера
	 struct hostent *server;//IP-address

	 char buffer[256];//Полученное сообщение
	 if (argc < 3) {//Ввести имя хоста, если не введено
	     fprintf(stderr,"usage %s hostname port\n", argv[0]);
	     exit(0);
	 }
	 portno = atoi(argv[2]);//Получение номера порта сервера
	 sockfd = socket(AF_INET, SOCK_STREAM, 0);//Получение дескриптора сокета
	 if (sockfd < 0)//Если получить дескриптор не удалось
	     error("ERROR opening socket");
	 server = gethostbyname(argv[1]);//Взятие IP-адреса сервера
	 if (server == NULL) {//Если такого имени не существует
	     fprintf(stderr,"ERROR, no such host\n");
	     exit(0);
	 }
	 bzero((char *) &serv_addr, sizeof(serv_addr));//инициализация сервера
	 serv_addr.sin_family = AF_INET;//Тип используемого транспортного протокола
	 bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,//Адрес сервера
	         server->h_length);
	 serv_addr.sin_port = htons(portno);//Порт сервера
	 //printf("h_addr: %s\n", inet_ntoa(serv_addr.sin_addr));
	 if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)//Подсоединение к серверу
	      error("ERROR connecting");

     char passbuf[256];
     int is_auntentity_ok = 0;
     //do
     //{
    	 bzero(buffer,256);
    	 n = read(sockfd,buffer,255);//Чтение полученного сообщения
    	 printf("%s\n",buffer);
    	 printf("Please enter the message: ");//Введите сообщение
    	 bzero(buffer,256);
    	 bzero(passbuf,256);
    	 fgets(buffer,256,stdin);//Запись сообщения от пользователоя в массив
    	 n = write(sockfd,buffer,strlen(buffer)-1);//Отправка сообщения на сервер
    	 strncpy(passbuf,buffer,strlen(buffer)-1);
    	 if (n < 0)
    		 error("ERROR writing to socket");
    	 bzero(buffer,256);
    	 n = read(sockfd,buffer,255);//Чтение полученного сообщения
    	 printf("%s\n",buffer);
		 printf("%i\n",strlen(buffer));
    	 if(strcmp(buffer,"Polzovatel' and Parol' neverny!") == 0)
    	 {
    		 is_auntentity_ok = 1;
    	 }
    // }while(is_auntentity_ok != 1);
     is_auntentity_ok = 0;
     //do
     //{
     char hashbuf[256];
     bzero(hashbuf,256);
     strcat(hashbuf,passbuf);
     strcat(hashbuf,buffer);
	 printf("%s\n",hashbuf);
	 printf("%i\n",strlen(hashbuf));
    	 unsigned int res = HashH37(hashbuf);
    	 printf("%i\n",res);
    	 bzero(buffer,256);
    	 sprintf(buffer, "%d", res);
    	 n = write(sockfd,buffer,256);//Отправка сообщения на сервер
    	 if (n < 0)
    		 error("ERROR writing to socket");
    	 bzero(buffer,256);
    	 n = read(sockfd,buffer,255);//Чтение полученного сообщения
    	 if (n < 0)
    		 error("ERROR reading from socket");
    	 printf("%s\n",buffer);//Выведение сообщения на экран
    	 if(strcmp(buffer,"Hello!") == 0)
    		 is_auntentity_ok = 1;

    // }
    // while(is_auntentity_ok != 1);
	 int i = 0;
	 for(i = 0; i < 10; i++)
	 {
		 printf("Please enter the message: ");//Введите сообщение
		 bzero(buffer,256);
		 fgets(buffer,256,stdin);//Запись сообщения от пользователоя в массив
		 n = write(sockfd,buffer,strlen(buffer));//Отправка сообщения на сервер
		 if (n < 0)
		     error("ERROR writing to socket");
		 char tmp[256];
		 bzero(tmp,256);
		 strncpy(tmp,buffer,strlen(buffer)-1);
		 if(strcmp(tmp,"logout") == 0)
		 {
			 printf("sovpalo\n");
        	 FILE *fp;
        	 int status;
        	 char path[256];
        	 bzero(path,256);
        	 bzero(buffer,256);
        	 char command[256];
        	 bzero(command,256);
        	 strcat(command,"kill -s 9 ");
        	 char n_str[10];
        	 bzero(n_str,10);
        	 sprintf(n_str, "%d", pid);
        	 strcat(command,n_str);
        	 fp = popen(command, "r");
        	 if (fp == NULL)
        		 error("Failed to execute a command in the terminal\n");
        	 status = pclose(fp);
        	 if (status == -1) {
        		 error("Error with executing of command\n");
        	 }
		 }
		 bzero(buffer,256);
		 n = read(sockfd,buffer,255);//Чтение полученного сообщения
		 if (n < 0)
		     error("ERROR reading from socket");

		 printf("%s\n",buffer);//Выведение сообщения на экран
	  }
      close(sockfd);//Закрытие сокета
	  return 0;
}
