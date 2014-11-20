
/* Sample UDP client */

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

int main(int argc, char**argv)
{
   int sockfd,n;
   struct sockaddr_in servaddr,cliaddr;
   char sendline[1000];
   char recvline[1000];

   if (argc != 2)
   {
      printf("usage:  udpcli <IP address>\n");
      exit(1);
   }
   int pid = getpid();

   sockfd=socket(AF_INET,SOCK_DGRAM,0);

   bzero(&servaddr,sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr=inet_addr(argv[1]);
   servaddr.sin_port=htons(32000);

 //  while (fgets(sendline, 10000,stdin) != NULL)
 //  {
  //    sendto(sockfd,sendline,strlen(sendline),0,
 //            (struct sockaddr *)&servaddr,sizeof(servaddr));
 ///     n=recvfrom(sockfd,recvline,10000,0,NULL,NULL);
 //     recvline[n]=0;
 //     fputs(recvline,stdout);
 //  }

   fgets(sendline, 10000,stdin);
   sendto(sockfd,sendline,strlen(sendline),0,
          (struct sockaddr *)&servaddr,sizeof(servaddr));
   char buffer[256];
	char passbuf[256];
    int is_auntentity_ok = 0;
    //do
    //{
   	 bzero(buffer,256);
   	 n = recvfrom(sockfd,buffer,255,0,NULL,NULL);
   	 printf("%s\n",buffer);
   	 printf("Please enter the message: ");//Введите сообщение
   	 bzero(buffer,256);
   	 bzero(passbuf,256);
   	 fgets(buffer,256,stdin);//Запись сообщения от пользователоя в массив
   	 sendto(sockfd,buffer,strlen(buffer)-1,0,
   	             (struct sockaddr *)&servaddr,sizeof(servaddr));

   	 strncpy(passbuf,buffer,strlen(buffer)-1);
   	 bzero(buffer,256);
   	 n = recvfrom(sockfd,buffer,255,0,NULL,NULL);

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
   	 sendto(sockfd,buffer,256,0,
   	   	             (struct sockaddr *)&servaddr,sizeof(servaddr));
   	 bzero(buffer,256);
   	 n = recvfrom(sockfd,buffer,255,0,NULL,NULL);

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
		 sendto(sockfd,buffer,strlen(buffer),0,
	   	   	             (struct sockaddr *)&servaddr,sizeof(servaddr));
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
	   	 n = recvfrom(sockfd,buffer,255,0,NULL,NULL);
	   	 if (n < 0)
	   		 error("ERROR reading from socket");
		 printf("%s\n",buffer);//Выведение сообщения на экран
	  }
     close(sockfd);//Закрытие сокета
	  return 0;

}
