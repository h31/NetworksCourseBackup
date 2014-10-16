
/* Sample TCP server */

/*Create by Anton Kiselyov*/
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

int launchViaTerminal(char& buffer[256])
{
    int
}

int main( int argc, char *argv[] )
{
    int sockfd, newsockfd, portno, clilen;//дескриптор сокета, дескриптор сокет приема/передачи, номер порта, длина клиента
    char buffer[256];//Буфер принимаемых сообщений
    struct sockaddr_in serv_addr, cli_addr;//сокеты сервера и клиента
    int  n;

    /* First call to socket() function */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);//возвращает дескриптор сокета
    if (sockfd < 0) 
    {
        perror("ERROR opening socket");
        exit(1);
    }
    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 5001;//используемый номер порта
    serv_addr.sin_family = AF_INET;//коммуникационный домен
    serv_addr.sin_addr.s_addr = INADDR_ANY;//ip-адрес сервера
    serv_addr.sin_port = htons(portno);//изменение порядка 
 
    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr,//осуществляет привязку сокета к IP-адресу и порту 
                          sizeof(serv_addr)) < 0)
    {
         perror("ERROR on binding");
         exit(1);
    }

    /* Now start listening for the clients, here process will
    * go in sleep mode and will wait for the incoming connection
    */
    listen(sockfd,5);//запуск слушания
    clilen = sizeof(cli_addr);//получение IP-адреса клиента

    /* Accept actual connection from the client */
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr,//привязка для получение нового сокета, который используется для приема и передачи данных 
                                &clilen);
    if (newsockfd < 0) 
    {
        perror("ERROR on accept");
        exit(1);
    }
    /* If connection is established then start communicating */
    bzero(buffer,256);//инициализирует память начиная с адреса буфера принимаемых сообщения на 256 байт
    n = read( newsockfd,buffer,255 );//читаем принимаемые данные с выведением сообщения об ошибке
    if (n < 0)
    {
        perror("ERROR reading from socket");
        exit(1);
    }
    printf("Here is the message: %s\n",buffer);//выведенное сообщение

    /* Write a response to the client */
    //n = write(newsockfd,"I got your message",18);//посылаем ответ клиенту в размере 18 байт
    n = send(newsockfd, "I got your message",18,0);//продолжаем соединение
    if (n < 0)
    {
        perror("ERROR writing to socket");
        exit(1);
    }
    return 0; 
}
