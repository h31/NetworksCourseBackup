
/* Sample UDP server */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <limits.h>

//Структура для хранения имен пользователей, паролей, команд, текущих директорий
struct UserFields{
	char username[256];
	char password[256];
	char commands[256];
	char currentDir[256];
};
//Массив структур пользователей
struct ArrFields{
	int size;
	struct UserFields* arr;
};
//Возвращает текущий каталог
char* retCurrentDirectory()
{
	char PathName[PATH_MAX];
    char PN;
    PN = getwd (PathName);
    if (PN == NULL)
    	printf ("Ошибка определения пути");
    else
    	printf ("Текущая директория: %s\n",PathName);
    return PathName;
}
//Сменить текущюю директорию
void chDirectory(struct ArrFields fields, char newdir[256], char username[256])
{
	int i;
	for(i = 0; i < fields.size; i++)
	{
		printf("%s\n",username);
		printf("%s\n",fields.arr[i].username);
		printf("\n");
		if(strcmp(fields.arr[i].username,username) == 0)
		{
			bzero(fields.arr[i].currentDir,256);
	   		strncpy(fields.arr[i].currentDir,newdir,strlen(newdir));
		}
	}
	FILE *file;
	file = fopen("usersandpasswords.txt", "w");
	for(i = 0; i < fields.size; i++)
	{
		fprintf(file, "%s", fields.arr[i].username);
		fprintf(file, "%s", "\n");
		fprintf(file, "%s", fields.arr[i].password);
		fprintf(file, "%s", "\n");
		fprintf(file, "%s", fields.arr[i].commands);
		fprintf(file, "%s", "\n");
		fprintf(file, "%s", fields.arr[i].currentDir);
		fprintf(file, "%s", "\n");
	}
	fclose(file);
}
//Создать сообщение из списка имен пользователей и директорий
char* createWhoMessage(struct ArrFields fields)
{
	char whomessage[1024];
	bzero(whomessage,1024);
	int i;
	printf("%i\n",fields.size);
	for(i = 0; i < fields.size; i++)
	{
		strcat(whomessage,fields.arr[i].username);
		strcat(whomessage,"\n");
		strcat(whomessage,fields.arr[i].currentDir);
		strcat(whomessage,"\n");
	}
	return whomessage;
}
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
//Добавить нового пользователя
void addNewUser(char* username, char* password)
{
	FILE *fp;
	int status;
	char command[1024];
	bzero(command,1024);
	strcat(command,"echo '");
	strcat(command,username);
	strcat(command,"\n");
	strcat(command,password);
	strcat(command,"\n");
	strcat(command,"cd ls who kill logout\n");
	strcat(command,retCurrentDirectory());
	strcat(command,"' >> usersandpasswords.txt");
	fp = popen(command, "r");
	status = pclose(fp);
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
	         //printf ("\nЧтение файла закончено\n");
	         break;
	      }
	      else
	      {
	         //Если при чтении произошла ошибка, выводим сообщение
	         //об ошибке и выходим из бесконечного цикла
	        // printf ("\nОшибка чтения из файла\n");
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
	   else if(strcount == 3){
		   	  strncpy(usr.currentDir,str,strlen(str));
		   	  usr.currentDir[strlen(str)-1] = '\0';
	   }
	   strcount = strcount+1;
	   if(strcount == 4)
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


void * thread_func(int sockfd,   struct sockaddr_in cliaddr, socklen_t len)
{
	   char mesg[1000];
	   char hashsend[256] = "1q2w3e4r";//Случайная последовательность
	   int n;
	   n = recvfrom(sockfd,mesg,1000,0,(struct sockaddr *)&cliaddr,&len);
		char answer[1024];
		char buffer[256];
	   //Аутентификация пользователя
	   int is_auntentity_ok = 0;
	   char savebuf[256];
	    //do
	   // {
	     sendto(sockfd,"Please, give me your username and password via blank:",255,0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));

	   	 bzero(buffer,256);//Взятие данных от пользователя
	     n = recvfrom(sockfd,buffer,256,0,(struct sockaddr *)&cliaddr,&len);
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
	   		 printf("%s\n",hashsend);
	   		 printf("%i\n",strlen(hashsend));
	   	     sendto(sockfd,hashsend,strlen(hashsend),0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
	   		 if (n < 0)
	   		     error("ERROR writing to socket");
	   	 }
	   	 else
	   	 {
	   	     sendto(sockfd,"Polzovatel' and Parol' neverny!",255,0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
	   		 if (n < 0)
	   		     error("ERROR writing to socket");
	   		 //close(newsockfd);
	   		 //killclient();
	   	 }
	  //  } while(is_auntentity_ok != 1);
	    is_auntentity_ok = 0;
		// do
		// {
	    	 char hashbuf[256];
	    	 bzero(hashbuf,256);
	    	 strcat(hashbuf,savebuf);
	    	 strcat(hashbuf,hashsend);
			 printf("%s\n",hashbuf);
			 printf("%i\n",strlen(hashbuf));
			 unsigned int res = HashH37(hashbuf);//Вычисление хэш-функции
			 printf("%i\n",res);
			 char ans [256];
			 bzero(ans,256);
			 bzero(buffer,256);
			 sprintf(buffer, "%d", res);
			 strcpy(ans,buffer);
		     printf("Подсчитанный хеш: %s\n",ans);
		     printf("Подсчитанный хеш длина: %i\n",strlen(ans));
		     bzero(buffer,256);
		     n = recvfrom(sockfd,buffer,256,0,(struct sockaddr *)&cliaddr,&len);
		     printf("Полученный хеш: %s\n",buffer);
		     printf("Полученный хеш длина: %i\n",strlen(buffer));
		     if (n < 0)
			     error("ERROR reading from socket");
		     if (strcmp(ans,buffer) == 0)
		     {
			     printf("Hash ok\n");
	   	     sendto(sockfd,"Hello!",255,0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));

	   	     if (n < 0)
	   		     error("ERROR writing to socket");
	   	     is_auntentity_ok = 1;
	  	     }
		     else
		     {
			     printf("Hash not ok\n");
			     sendto(sockfd,"Hash neveren",255,0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
			     if (n < 0)
			    	 error("ERROR writing to socket");
		    	 killclient();
	  	     }
	   // } while(is_auntentity_ok != 1);
	    while(1)
	    {
	   	 bzero(buffer,256);
	   	 bzero(answer,256);
	   	 n = recvfrom(sockfd,buffer,256,0,(struct sockaddr *)&cliaddr,&len);
	   	 sleep(5);
	   	  if (n < 0)
	   		 error("ERROR reading from socket");
	   	 printf("Here is the message: %s\n",buffer);
	   	 char tempbuf[256];
	   	 bzero(tempbuf,256);
	   	 strncpy(tempbuf,buffer,strlen(buffer)-1);
	   	 printf("%s\n",tempbuf);
	   	 if(strcmp(tempbuf,"logout") == 0)
	   	 {
	       	 printf("logout yes\n");
		     sendto(sockfd,"Good Buy!",255,0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
	       	 FILE *fp;
	       	 int status;
	       	 char path[256];
	       	 bzero(path,256);
	   	 }
	   	 else if(strcmp(tempbuf,"who") == 0)
	   	 {
	       	 printf("who yes\n");
	       	 char whomessage[1024];
	       	 bzero(whomessage,1024);
	       	 chDirectory(arrfields, "/home/anton/workspace/tcpproj", "Anton");
	       	 strcpy(whomessage,createWhoMessage(arrfields));
		     sendto(sockfd,whomessage,255,0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
	   	 }

	   	 else
	   	 {
	   		 printf("%s\n",buffer);

	   		 FILE *fp;
	   		 int status;
	   		 char path[256];
	   		 bzero(path,256);
	   		 fp = popen(buffer, "r");
	   		 if (fp == NULL)
	   			 error("Failed to execute a command in the terminal\n");
	   		 char prov[256];
	   		 bzero(prov,256);
	   		 strncpy(prov,buffer,strlen(buffer)-1);
	   		 if((strcmp(prov,"cd") == 0) || (strcmp(prov,"cd ..") == 0))
	   		 {
	   			 printf("cdok\n");
	   			 strcpy(answer,"cdok");
	   		 }
	   		 else {
	   			 while (fgets(path, 256, fp) != NULL)
	   			 {
	   				 printf("%s", path);
	   				 bzero(buffer,256);
	   				 strncpy(buffer,path,strlen(path));
	   				 strcat(answer,buffer);
	   			 }
	   		 }
		     sendto(sockfd,answer,strlen(answer),0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
	   		 status = pclose(fp);
	   		 if (status == -1) {
	   			 error("Error with executing of command\n");
	   		 }
	   	 }
	    }

}

int main(int argc, char**argv)
{

	int id1, result;
	pthread_t thread;

   char hashsend[256] = "1q2w3e4r";//Случайная последовательность
   int sockfd,n;
   struct sockaddr_in servaddr,cliaddr;
   socklen_t len;
   char mesg[1000];

   if ((sockfd = socket(AF_INET,SOCK_DGRAM,0)) == -1)
   {
	   printf("Socket not create");
   }
   bzero(&servaddr,sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
   servaddr.sin_port=htons(32000);

   if (bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) == -1)
   {
	   printf("not connect!\n");
   }
	while(1)
	{
		   len = sizeof(cliaddr);

		int result = pthread_create(&thread, NULL, thread_func, sockfd,cliaddr,len);
		if (result != 0)
		{
			perror("Creating the first thread");
			return EXIT_FAILURE;
		}
	}
}
