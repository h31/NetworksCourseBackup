#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "write.h"

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

   //Start connect
   	strcpy(recvline, "ERROR");
   	while (strcmp(recvline, "ERROR") == 0) {
   		printf("Please enter '!' to connect :");
   		if(fgets(sendline, 10000,stdin) != NULL)
   		 sendto(sockfd,sendline,strlen(sendline),0,
   		             (struct sockaddr *)&servaddr,sizeof(servaddr));
   		bzero(recvline,1000);
   		n=recvfrom(sockfd,recvline,10000,0,NULL,NULL);
   	}
   	//Registration
   	printf("Enter login: ");
   	if(fgets(sendline, 10000,stdin) != NULL)
   	sendto(sockfd,sendline,strlen(sendline),0,
   	  		             (struct sockaddr *)&servaddr,sizeof(servaddr));
	bzero(recvline,1000);
   	n=recvfrom(sockfd,recvline,10000,0,NULL,NULL);
   	printf("%s\n", recvline);

   	strcpy(recvline, "ERROR");
   	while (strcmp(recvline, "ERROR") == 0) {
   		printf("Please enter '!' to receive list of test :");
   		if(fgets(sendline, 10000,stdin) != NULL)
   		   	sendto(sockfd,sendline,strlen(sendline),0,
   		   	   		             (struct sockaddr *)&servaddr,sizeof(servaddr));
   			bzero(recvline,1000);
   		   	n=recvfrom(sockfd,recvline,10000,0,NULL,NULL);
   	}
   	int listOfTest[50];
   	int size, number;
   	int true=0;
   	char res[50];
   	int i;
   	size=writeListTest(listOfTest,recvline,res);
   	printf("%s\n", res);
   	while(true!=1){
   		printf("Choose test: ");
   		if(fgets(sendline, 10000,stdin) != NULL)
   		   	sendto(sockfd,sendline,strlen(sendline),0,
   		   	   		             (struct sockaddr *)&servaddr,sizeof(servaddr));
   		number=toInt(sendline);
   		for(i=0;i<size;i++){
   		if(number==listOfTest[i]){
   			true=1;
   			break;
   		}}
   	}
   	struct Line x;
   	//LOOP
   	while (1) {
   		bzero(recvline, 256);
   		n=recvfrom(sockfd,recvline,10000,0,NULL,NULL);
   				if (recvline[strlen(recvline) - 1] == '!') {
   					printf("%s\n", recvline);
   					close(sockfd);
   					break;
   				}
   		strcpy(recvline, "ERROR");
   		while (strcmp(recvline, "ERROR") == 0) {
   			printf("Enter ! to receive question: ");
   			if(fgets(sendline, 10000,stdin) != NULL)
   			   	sendto(sockfd,sendline,strlen(sendline),0,
   			   	   		             (struct sockaddr *)&servaddr,sizeof(servaddr));
   			 bzero(recvline,1000);
   			   	n=recvfrom(sockfd,recvline,10000,0,NULL,NULL);
   		}
   			writeSize(&x,recvline);
   			char str[sizeStr(&x)];
   			char* stringOut = (char*) malloc(50 * sizeof(char));
   			writeToClient(&x,str,stringOut);
   			printf("%s\n", stringOut);

   		freeLine(&x);
   		while(1){
   			if(fgets(sendline, 10000,stdin) != NULL)
   			   	sendto(sockfd,sendline,strlen(sendline),0,
   			   	   		             (struct sockaddr *)&servaddr,sizeof(servaddr));
   		if(sendline[0]=='1'  || sendline[0]=='2' || sendline[0]=='3' || sendline[0]=='4')
   			break;
   		else
   			printf("Enter your answer(1,2,3,4):\n");
   		}
   		bzero(recvline, 256);
   		n=recvfrom(sockfd,recvline,10000,0,NULL,NULL);
   		printf("%s\n", recvline);
   	}
   return 0;
}
