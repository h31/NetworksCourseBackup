/* Sample UDP client */

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

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

   sockfd=socket(AF_INET,SOCK_DGRAM,0);

   bzero(&servaddr,sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr=inet_addr(argv[1]);
   servaddr.sin_port=htons(32000);
   char buffer[256];
   scanf("%s", buffer);
      sendto(sockfd,buffer,strlen(buffer),0,
             (struct sockaddr *)&servaddr,sizeof(servaddr));
  //    getchar ();
      n=recvfrom(sockfd,buffer,10000,0,NULL,NULL);
  //    buffer[n]='\0';
      printf("%s\n", buffer);
      bzero(buffer, 256);
      n=recvfrom(sockfd,buffer,10000,0,NULL,NULL);
       //    buffer[n]='\0';
           printf("%s\n", buffer);

           scanf("%s", buffer);
                sendto(sockfd,buffer,strlen(buffer),0,
                       (struct sockaddr *)&servaddr,sizeof(servaddr));
                bzero(buffer, 256);
                     n=recvfrom(sockfd,buffer,10000,0,NULL,NULL);
                      //    buffer[n]='\0';
                          printf("%s\n", buffer);
		bzero(buffer, 256);
	 scanf("%s", buffer);
	 sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *) &servaddr,
	 sizeof(servaddr));
	 //start from croup

	 bzero(buffer, 256);
	 n = recvfrom(sockfd, buffer, 256, 0, NULL, NULL);

	 printf("%s\n", buffer);
/*	 bzero(buffer, 256);
	 scanf("%s", buffer);
	 sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *) &servaddr,
	 sizeof(servaddr));
	 bzero(buffer, 256);
	 n = recvfrom(sockfd, buffer, 256, 0, NULL, NULL);
	 printf("%s\n", buffer);

	 bzero(buffer, 256);
	 scanf("%s", buffer);
	 sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *) &servaddr,
	 sizeof(servaddr));
	 bzero(buffer, 256);
	 n = recvfrom(sockfd, buffer, 256, 0, NULL, NULL);
	 printf("%s\n", buffer);*/

}
