/*
    Simple udp client
    Silver Moon (m00n.silv3r@gmail.com)
*/

/* Sample UDP client */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include<stdio.h>
#include<winsock2.h>
 
#pragma comment(lib,"ws2_32.lib") //Winsock Library
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

int main(int argc, char**argv)
{
   int sockfd,n;
   struct sockaddr_in servaddr,cliaddr;
   char sendline[256];
    WSADATA wsa;
 
    //Initialise winsock
    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    printf("Initialised.\n");
     
   if (argc != 2)
   {
      printf("usage:  udpcli <IP address>\n");
      exit(1);
   }
   sockfd=socket(AF_INET,SOCK_DGRAM,0);

   memset(&servaddr,0,sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr=inet_addr(argv[1]);
   servaddr.sin_port=htons(32000);

   //Инициализация соединения от клиента
   //Передача адреса, считывается с консоли
    fgets(sendline, 256,stdin);
    memset(sendline,0,256);
   	strcpy(sendline,"new");
    sendto(sockfd, sendline, 256,0,(struct sockaddr*) &servaddr,sizeof(servaddr));
    char buffer[256];
	char passbuf[256];
    int is_auntentity_ok = 0;
   	 memset(buffer,0,256);
   	 memset(buffer,0,256);
   	 memset(passbuf,0,256);
   	 //Отправка кода операций
   	 sendto(sockfd,"My data",256,0,
   	             (struct sockaddr *)&servaddr,sizeof(servaddr));

   	 n = recvfrom(sockfd,buffer,256,0,NULL,NULL);
   	 printf("%s\n",buffer);

     //Далее происходит этап аутентифкации
   	 printf("Please enter the message: \n");//Введите сообщение
   	 memset(buffer,0,256);
   	 memset(passbuf,0,256);
   	 fgets(buffer,256,stdin);//Запись сообщения от пользователоя в массив
	 buffer[strlen(buffer)] = '\0';
   	 sendto(sockfd,buffer,strlen(buffer)-1,0,
   	             (struct sockaddr *)&servaddr,sizeof(servaddr));

   	 strncpy_s(passbuf,buffer,strlen(buffer)-1);
   	 memset(buffer,0,256);
   	 n = recvfrom(sockfd,buffer,255,0,NULL,NULL);

   	 printf("%s\n",buffer);
	 printf("%i\n",strlen(buffer));
   	 if(strcmp(buffer,"Polzovatel' and Parol' neverny!") == 0)
   	 {
   		 is_auntentity_ok = 1;
   	 }

     is_auntentity_ok = 0;

     char hashbuf[256];
     memset(hashbuf,0,256);
     strcat_s(hashbuf,passbuf);
     strcat_s(hashbuf,buffer);
	 printf("%s\n",hashbuf);
	 printf("%i\n",strlen(hashbuf));
   	 unsigned int res = HashH37(hashbuf);
   	 printf("%i\n",res);
   	 memset(buffer,0,256);
   	 sprintf(buffer, "%d", res);
   	 sendto(sockfd,buffer,256,0,
   	   	             (struct sockaddr *)&servaddr,sizeof(servaddr));
   	 memset(buffer,0,256);
   	 n = recvfrom(sockfd,buffer,256,0,NULL,NULL);

   	 if (n < 0)
   		 error("ERROR reading from socket");
   	 printf("%s\n",buffer);//Выведение сообщения на экран
   	 if(strcmp(buffer,"Hello!") == 0)
   		 is_auntentity_ok = 1;

   	 int i = 0;
	 for(i = 0; i < 10; i++)
	 {
		 printf("Please enter the message: ");//Введите сообщение

		 memset(buffer,0,256);
		 fgets(buffer,256,stdin);//Запись сообщения от пользователоя в массив

		 sendto(sockfd,"commands",strlen("commands"),0,
		 	   	             (struct sockaddr *)&servaddr,sizeof(servaddr));
		 Sleep(2);
		 sendto(sockfd,buffer,strlen(buffer),0,
	   	   	             (struct sockaddr *)&servaddr,sizeof(servaddr));
		 char tmp[256];
		 memset(tmp,0,256);
		 strncpy_s(tmp,buffer,strlen(buffer)-1);
		 if(strcmp(tmp,"logout") == 0)
		 {
			 printf("sovpalo\n");
			 FILE *fp;
			 int status;
			/* char path[256];
			 memset(path,0,256);
			 memset(buffer,0,256);
			 char command[256];
			 memset(command,0,256);
			 strcat_s(command,"kill -s 9 ");
			 char n_str[10];
			 memset(n_str,0,10);
			 sprintf(n_str, "%d", pid);
			 strcat_s(command,n_str);
			 fp = _popen(command, "r");
			 if (fp == NULL)
				 error("Failed to execute a command in the terminal\n");
			 status = _pclose(fp);
			 if (status == -1) {
				 error("Error with executing of command\n");
			 }*/
		 }
		 memset(buffer,0,256);
	   	 n = recvfrom(sockfd,buffer,256,0,NULL,NULL);
	   	 if (n < 0)
	   		 error("ERROR reading from socket");
		 printf("%s\n",buffer);//Выведение сообщения на экран
		 //Запись сообщения от пользователоя в массив
	  }
	  closesocket(sockfd);
	  WSACleanup();
	  return 0;
}

