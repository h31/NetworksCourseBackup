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
      printf("Please enter the message: ");//Введите сообщение
      bzero(buffer,256);
      fgets(buffer,256,stdin);//Запись сообщения от пользователоя в массив
      n = write(sockfd,buffer,strlen(buffer));//Отправка сообщения на сервер
      if (n < 0)
          error("ERROR writing to socket");
      //while(1)//
      //{
    	  bzero(buffer,256);
    	  n = read(sockfd,buffer,255);//Чтение полученного сообщения
    	  if (n < 0)
    		  error("ERROR reading from socket");
    	  printf("%s\n",buffer);//Выведение сообщения на экран
      //}
      close(sockfd);//Закрытие сокета
	  return 0;
}
