/**
    Simple TCP client to fetch a web page
    Silver Moon (m00n.silv3r@gmail.com)
*/
 
/*
    Create a TCP socket
*/
 
#include<stdio.h>
#include<winsock2.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
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
/*
int main(int argc , char *argv[])
{
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;
    char *message , server_reply[2000];
    int recv_size;
 
    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return 1;
    }
     
    printf("Initialised.\n");
     
    //Create a socket
    if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d" , WSAGetLastError());
    }
 
    printf("Socket created.\n");
     
     
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);
 
    //Connect to remote server
    if (connect(s , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("connect error");
        return 1;
    }
     
    puts("Connected");
     
    //Send some data
    message = "GET / HTTP/1.1\r\n\r\n";
    if( send(s , message , strlen(message) , 0) < 0)
    {
        puts("Send failed");
        return 1;
    }
    puts("Data Send\n");
     
    //Receive a reply from the server
    if((recv_size = recv(s , server_reply , 2000 , 0)) == SOCKET_ERROR)
    {
        puts("recv failed");
    }
     
    puts("Reply received\n");
 
    //Add a NULL terminating character to make it a proper string before printing
    server_reply[recv_size] = '\0';
    puts(server_reply);
 
    return 0;
}*/


int main(int argc, char *argv[])
{
	 WSADATA wsa;
	 SOCKET sockfd;
     int portno, n;//Дескриптор сокета
     struct sockaddr_in serv_addr;//Адрес сервера
     struct hostent *server;//IP-address
	 
	printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return 1;
    }
      
    printf("Initialised.\n");
     char buffer[256];//Полученное сообщение
     portno = 50000;//Получение номера порта сервера
     sockfd = socket(AF_INET, SOCK_STREAM, 0);//Получение дескриптора сокета
     if (sockfd < 0)//Если получить дескриптор не удалось
         error("ERROR opening socket");
	  
	memset((char*) &serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(50000);

     /*if (*/connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr));/* < 0);*///Подсоединение к серверу
          //error("ERROR connecting");

     char passbuf[256];
     int is_auntentity_ok = 0;
         memset(buffer,0,256);
         n = recv(sockfd,buffer,255,0);//Чтение полученного сообщения
         printf("%s\n",buffer);
         printf("Please enter the message: ");//Введите сообщение
         memset(buffer,0,256);
         memset(passbuf,0,256);
         fgets(buffer,256,stdin);//Запись сообщения от пользователоя в массив
         n = send(sockfd,buffer,strlen(buffer)-1,0);//Отправка сообщения на сервер
         strncpy(passbuf,buffer,strlen(buffer)-1);
   
         memset(buffer,0,256);
         n = recv(sockfd,buffer,255,0);//Чтение полученного сообщения
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
         sprintf_s(buffer, "%d", res);
         n = send(sockfd,buffer,256,0);//Отправка сообщения на сервер

         memset(buffer,0,256);
         n = recv(sockfd,buffer,255,0);//Чтение полученного сообщения

         printf("%s\n",buffer);//Выведение сообщения на экран
         if(strcmp(buffer,"Hello!") == 0)
             is_auntentity_ok = 1;

     int i = 0;
     for(i = 0; i < 10; i++)
     {
         printf("Please enter the message: ");//Введите сообщение
         memset(buffer,0,256);
         fgets(buffer,256,stdin);//Запись сообщения от пользователоя в массив
         n = send(sockfd,buffer,strlen(buffer),0);//Отправка сообщения на сервер
   
         char tmp[256];
         memset(tmp,0,256);
         strncpy_s(tmp,buffer,strlen(buffer)-1);
         if(strcmp(tmp,"logout") == 0)
         {
             printf("sovpalo\n");
			closesocket(sockfd);
         }
         memset(buffer,0,256);
         n = recv(sockfd,buffer,255,0);//Чтение полученного сообщения

		 printf("%s\n",buffer);//Выведение сообщения на экран
      }
      closesocket(sockfd);//Закрытие сокета
      WSACleanup();
      return 0;
}