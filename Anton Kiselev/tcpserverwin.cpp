/*
    Bind socket to port 8888 on localhost
*/
#include<io.h>
#include<stdio.h>
#include<winsock2.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <locale.h>


#include <sys/types.h>
#include <limits.h>

  
#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define N 80

//��������� ��� �������� ���� �������������, �������, ������, ������� ����������
struct UserFields{
    char username[256];
    char password[256];
    char commands[256];
    char currentDir[256];
};
//������ �������� �������������
struct ArrFields{
    int size;
    struct UserFields* arr;
};
//���������� ������� �������
char* retCurrentDirectory()
{
    char *PathName = NULL;
    size_t t = FILENAME_MAX;
    PathName = _getcwd (PathName,t);
    if (PathName == NULL)
        printf ("������ ����������� ����");
    else
        printf ("������� ����������: %s\n",PathName);
    return PathName;
}

//������� ������� ����������
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
            memset(fields.arr[i].currentDir,0,256);
               strncpy_s(fields.arr[i].currentDir,newdir,strlen(newdir));
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

//������� ��������� �� ������ ���� ������������� � ����������
char* createWhoMessage(struct ArrFields fields)
{
    char whomessage[1024];
    memset(whomessage,0,1024);
    int i;
    printf("%i\n",fields.size);
    for(i = 0; i < fields.size; i++)
    {
        strcat_s(whomessage,fields.arr[i].username);
        strcat_s(whomessage,"\n");
        strcat_s(whomessage,fields.arr[i].currentDir);
        strcat_s(whomessage,"\n");
    }
    return whomessage;
}
//���-�������, ������������ ��� �������� ������ ������������ � ������
unsigned int HashH37(const char * str)
{
    unsigned int hash = 0;
    for( ; *str; str++)
        hash = (hash * 1664525) + (unsigned char)(*str) + 1013904223;
    return hash;
}
//����� ��������� �� ������
void error(const char *msg)
{
    perror(msg);
    exit(1);
}
//������������ �������
void killclient()
{
    system("killall client");
    exit(1);
}

//�������� ������ ������������
void addNewUser(char* username, char* password)
{
    char command[1024];
    memset(command,0,1024);
    strcat_s(command,"echo '");
    strcat_s(command,username);
    strcat_s(command,"\n");
    strcat_s(command,password);
    strcat_s(command,"\n");
    strcat_s(command,"cd ls who kill logout\n");
    strcat_s(command,retCurrentDirectory());
    strcat_s(command,"' >> usersandpasswords.txt");
	   FILE   *pPipe;

        /* Run DIR so that it writes its output to a pipe. Open this
         * pipe with read text attribute so that we can read it 
         * like a text file. 
         */
	if( (pPipe = _popen( command, "rt" )) == NULL )
		exit( 1 );
   
	/* Close pipe and print return value of pPipe. */
	if (feof( pPipe))
	{
		printf( "\nProcess returned %d\n", _pclose( pPipe ) );
	}
	else
	{
		printf( "Error: Failed to read the pipe to the end.\n");
	}
}
//���������� ����������� ����� ������������� � �������
struct ArrFields readFile()
{
    struct UserFields usr;
    struct UserFields arr[10];
    struct ArrFields arrfields;
    // ����������, � ������� ����� ������� ��������� �� ���������
    // ����� ������
    FILE *mf;
    // ����������, � ������� ���������� ����� ���������� ����������� ������
    char str[50];
    //���������, � ������� ����� ������� ����� �������, � ������� �������
    // ������, ��� NULL ���� ��������� ����� ����� ��� ��������� ������
    char *estr;

    // �������� ����� � ������� ������� ������� ������ � �������� � ����
    // ������ ������
    mf = fopen("usersandpasswords.txt","r");

    // �������� �������� �����
    if (mf == NULL)
    {
        printf ("������\n");
    }
    int strcount = 0;
    int size = 0;
    //������ (���������) ������ �� ����� � ����������� �����
    while (1)
    {
       // ������ ����� ������  �� �����
       estr = fgets (str,sizeof(str),mf);
       //�������� �� ����� ����� ��� ������ ������
       if (estr == NULL)
       {
          // ���������, ��� ������ ���������: �������� ����
          // ��� ��� ������ ������
          if ( feof (mf) != 0)
          {
             //���� ���� ����������, ������� ��������� � ����������
             //������ � ������� �� ������������ �����
             //printf ("\n������ ����� ���������\n");
             break;
          }
          else
          {
             //���� ��� ������ ��������� ������, ������� ���������
             //�� ������ � ������� �� ������������ �����
            // printf ("\n������ ������ �� �����\n");
             break;
          }
       }
       if(strcount == 0){
                 strncpy_s(usr.username,str,strlen(str));
                 usr.username[strlen(str)-1] = '\0';
             }
       else if(strcount == 1){
                 strncpy_s(usr.password,str,strlen(str));
                 usr.password[strlen(str)-1] = '\0';
             }
       else if(strcount == 2){
                 strncpy_s(usr.commands,str,strlen(str));
                 usr.commands[strlen(str)-1] = '\0';
             }
       else if(strcount == 3){
                 strncpy_s(usr.currentDir,str,strlen(str));
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
    // ��������� ����
    if ( fclose (mf) == EOF)
        printf ("������\n");
    else printf ("���������\n");
    return arrfields;
}

DWORD WINAPI myThread1(LPVOID lpParameter)
{
	SOCKET new_socket = *((SOCKET*)lpParameter);
    char hashsend[256] = "1q2w3e4r";//��������� ������������������

    char answer[1024];
    char buffer[256];
    int n;
    //�������������� ������������
    int is_auntentity_ok = 0;
    char savebuf[256];
     //do
    // {
         n = send(new_socket,"Please, give me your username and password via blank:",255,0);
       //  if (n < 0)
       //      error("ERROR writing to socket");

         memset(buffer,0,256);//������ ������ �� ������������
         n = recv(new_socket,buffer,256,0);
         Sleep(5);
        // if (n < 0)
        //     error("ERROR reading from socket");
         printf("Here is the message: %s\n",buffer);
         char temp[256];
         memset(temp,0,256);
         memset(savebuf,0,256);
         strncpy_s(temp,buffer,strlen(buffer));
         struct ArrFields arrfields = readFile();
         int i = 0;
         int yes = 0;
         for(i = 0; i < arrfields.size; i++)
         {
             char usrpassw [256];
             memset(usrpassw,0,256);
             strcat_s(usrpassw,arrfields.arr[i].username);
             strcat_s(usrpassw," ");
             strcat_s(usrpassw,arrfields.arr[i].password);
             if(strcmp(temp,usrpassw) == 0)
             {
                 yes = 1;
                 strncpy_s(savebuf,temp,strlen(temp));
             }
         }
         printf("%i\n",yes);
         if(yes == 1)
         {
             is_auntentity_ok = 1;
             printf("%s\n",hashsend);
             printf("%i\n",strlen(hashsend));
             n = send(new_socket,hashsend,strlen(hashsend),0);
       //      if (n < 0)
       //          error("ERROR writing to socket");
         }
         else
         {
             n = send(new_socket,"Polzovatel' and Parol' neverny!",255,0);
      //       if (n < 0)
      //           error("ERROR writing to socket");
             //close(newsockfd);
             //killclient();
         }
   //  } while(is_auntentity_ok != 1);
     is_auntentity_ok = 0;
    // do
    // {
          char hashbuf[256];
          memset(hashbuf,0,256);
          strcat_s(hashbuf,savebuf);
          strcat_s(hashbuf,hashsend);
         printf("%s\n",hashbuf);
         printf("%i\n",strlen(hashbuf));
         unsigned int res = HashH37(hashbuf);//���������� ���-�������
         printf("%i\n",res);
         char ans [256];
         memset(ans,0,256);
         memset(buffer,0,256);
         sprintf_s(buffer, "%d", res);
         strcpy_s(ans,buffer);
         printf("������������ ���: %s\n",ans);
         printf("������������ ��� �����: %i\n",strlen(ans));
         memset(buffer,0,256);
         n = recv(new_socket,buffer,256,0);
         printf("���������� ���: %s\n",buffer);
         printf("���������� ��� �����: %i\n",strlen(buffer));
      //   if (n < 0)
      //       error("ERROR reading from socket");
         if (strcmp(ans,buffer) == 0)
         {
             printf("Hash ok\n");
             n = send(new_socket,"Hello!",255,0);
      //       if (n < 0)
      //           error("ERROR writing to socket");
             is_auntentity_ok = 1;
            }
         else
         {
             printf("Hash not ok\n");
             n = send(new_socket,"Hash neveren",255,0);
       //      if (n < 0)
        //         error("ERROR writing to socket");
             killclient();
            }
    // } while(is_auntentity_ok != 1);
     while(1)
     {
         memset(buffer,0,256);
         memset(answer,0,256);
         n = recv(new_socket,buffer,256,0);
       //  if (n < 0)
       //      error("ERROR reading from socket");
         printf("Here is the message: %s\n",buffer);
         char tempbuf[256];
         memset(tempbuf,0,256);
         strncpy_s(tempbuf,buffer,strlen(buffer)-1);
         printf("%s\n",tempbuf);
         if(strcmp(tempbuf,"logout") == 0)
         {
             printf("logout yes\n");
             n = send(new_socket,"Good Buy!",255,0);
    //         if (n < 0)
    //             error("ERROR writing to socket");
             char path[256];
             memset(path,0,256);
             closesocket(new_socket);
         }
         else if(strcmp(tempbuf,"who") == 0)
         {
             printf("who yes\n");
             char whomessage[1024];
             memset(whomessage,0,1024);
             chDirectory(arrfields, "/home/anton/workspace/tcpproj", "Anton");
             strcpy_s(whomessage,createWhoMessage(arrfields));
             n = send(new_socket,whomessage,255,0);
    //         if (n < 0)
     //            error("ERROR writing to socket");
         }

         else
         {
             printf("%s\n",buffer);


             FILE *fp;
             int status;
             char path[256];
             memset(path,0,256);
             fp = _popen(buffer, "r");
             if (fp == NULL)
                 error("Failed to execute a command in the terminal\n");
             char prov[256];
             memset(prov,0,256);
             strncpy_s(prov,buffer,strlen(buffer)-1);
             if((strcmp(prov,"cd") == 0) || (strcmp(prov,"cd ..") == 0))
             {
                 printf("cdok");
                 strcpy_s(answer,"cdok");
             }
             else {
                 while (fgets(path, 256, fp) != NULL)
                 {
                     printf("%s", path);
                     memset(buffer,0,256);
                     strncpy_s(buffer,path,strlen(path));
                     strcat_s(answer,buffer);
                 }
             }
             n = send(new_socket,answer,strlen(answer),0);
    //         if (n < 0)
    //             error("ERROR writing to socket");
             status = _pclose(fp);
             if (status == -1) {
                 error("Error with executing of command\n");
             }
         }
     }
     closesocket(new_socket);
	return 0;
}

int main(int argc , char *argv[])
{  
	setlocale(LC_ALL,"Russian");
	
	DWORD myThreadID;
	WSADATA wsa;
    SOCKET sockfd, new_socket;
	int portno;
    char hashsend[256] = "1q2w3e4r";
    int clilen;
    struct sockaddr_in serv_addr, cli_addr;

	printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return 1;
    }
      
    printf("Initialised.\n");
      
    //Create a socket
    if((sockfd = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d" , WSAGetLastError());
    }
  
    printf("Socket created.\n");
    
    memset((char *) &serv_addr,0, sizeof(serv_addr));
    portno = 8888;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
     //Bind
    if( bind(sockfd ,(struct sockaddr *)&serv_addr , sizeof(serv_addr)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d" , WSAGetLastError());
    }
      
    puts("Bind done");
  
    //Listen to incoming connections
    listen(sockfd , 5);
      
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
	clilen = sizeof(struct sockaddr_in);
		new_socket = accept(sockfd , (struct sockaddr *)&cli_addr, &clilen);
		if (new_socket == INVALID_SOCKET)
		{
			printf("accept failed with error code : %d" , WSAGetLastError());
		}

    char answer[1024];
    char buffer[256];
    int n;
    //�������������� ������������
    int is_auntentity_ok = 0;
    char savebuf[256];
     //do
    // {
         n = send(new_socket,"Please, give me your username and password via blank:",255,0);
       //  if (n < 0)
       //      error("ERROR writing to socket");

         memset(buffer,0,256);//������ ������ �� ������������
         n = recv(new_socket,buffer,256,0);
        // if (n < 0)
        //     error("ERROR reading from socket");
         printf("Here is the message: %s\n",buffer);
         char temp[256];
         memset(temp,0,256);
         memset(savebuf,0,256);
         strncpy_s(temp,buffer,strlen(buffer));
         struct ArrFields arrfields = readFile();
         int i = 0;
         int yes = 0;
         for(i = 0; i < arrfields.size; i++)
         {
             char usrpassw [256];
             memset(usrpassw,0,256);
             strcat_s(usrpassw,arrfields.arr[i].username);
             strcat_s(usrpassw," ");
             strcat_s(usrpassw,arrfields.arr[i].password);
             if(strcmp(temp,usrpassw) == 0)
             {
                 yes = 1;
                 strncpy_s(savebuf,temp,strlen(temp));
             }
         }
         printf("%i\n",yes);
         if(yes == 1)
         {
             is_auntentity_ok = 1;
             printf("%s\n",hashsend);
             printf("%i\n",strlen(hashsend));
             n = send(new_socket,hashsend,strlen(hashsend),0);
       //      if (n < 0)
       //          error("ERROR writing to socket");
         }
         else
         {
             n = send(new_socket,"Polzovatel' and Parol' neverny!",255,0);
      //       if (n < 0)
      //           error("ERROR writing to socket");
             //close(newsockfd);
             //killclient();
         }
   //  } while(is_auntentity_ok != 1);
     is_auntentity_ok = 0;
    // do
    // {
          char hashbuf[256];
          memset(hashbuf,0,256);
          strcat_s(hashbuf,savebuf);
          strcat_s(hashbuf,hashsend);
         printf("%s\n",hashbuf);
         printf("%i\n",strlen(hashbuf));
         unsigned int res = HashH37(hashbuf);//���������� ���-�������
         printf("%i\n",res);
         char ans [256];
         memset(ans,0,256);
         memset(buffer,0,256);
         sprintf_s(buffer, "%d", res);
         strcpy_s(ans,buffer);
         printf("������������ ���: %s\n",ans);
         printf("������������ ��� �����: %i\n",strlen(ans));
         memset(buffer,0,256);
         n = recv(new_socket,buffer,256,0);
         printf("���������� ���: %s\n",buffer);
         printf("���������� ��� �����: %i\n",strlen(buffer));
      //   if (n < 0)
      //       error("ERROR reading from socket");
         if (strcmp(ans,buffer) == 0)
         {
             printf("Hash ok\n");
             n = send(new_socket,"Hello!",255,0);
      //       if (n < 0)
      //           error("ERROR writing to socket");
             is_auntentity_ok = 1;
            }
         else
         {
             printf("Hash not ok\n");
             n = send(new_socket,"Hash neveren",255,0);
       //      if (n < 0)
        //         error("ERROR writing to socket");
             killclient();
            }
    // } while(is_auntentity_ok != 1);
     while(1)
     {
         memset(buffer,0,256);
         memset(answer,0,256);
         n = recv(new_socket,buffer,256,0);
       //  if (n < 0)
       //      error("ERROR reading from socket");
         printf("Here is the message: %s\n",buffer);
         char tempbuf[256];
         memset(tempbuf,0,256);
         strncpy_s(tempbuf,buffer,strlen(buffer)-1);
         printf("%s\n",tempbuf);
         if(strcmp(tempbuf,"logout") == 0)
         {
             printf("logout yes\n");
             n = send(new_socket,"Good Buy!",255,0);
    //         if (n < 0)
    //             error("ERROR writing to socket");
             char path[256];
             memset(path,0,256);
             closesocket(new_socket);
         }
         else if(strcmp(tempbuf,"who") == 0)
         {
             printf("who yes\n");
             char whomessage[1024];
             memset(whomessage,0,1024);
             chDirectory(arrfields, "/home/anton/workspace/tcpproj", "Anton");
             strcpy_s(whomessage,createWhoMessage(arrfields));
             n = send(new_socket,whomessage,255,0);
    //         if (n < 0)
     //            error("ERROR writing to socket");
         }

         else
         {
             printf("%s\n",buffer);


             FILE *fp;
             int status;
             char path[256];
             memset(path,0,256);
             fp = _popen(buffer, "r");
             if (fp == NULL)
                 error("Failed to execute a command in the terminal\n");
             char prov[256];
             memset(prov,0,256);
             strncpy_s(prov,buffer,strlen(buffer)-1);
             if((strcmp(prov,"cd") == 0) || (strcmp(prov,"cd ..") == 0))
             {
                 printf("cdok");
                 strcpy_s(answer,"cdok");
             }
             else {
                 while (fgets(path, 256, fp) != NULL)
                 {
                     printf("%s", path);
                     memset(buffer,0,256);
                     strncpy_s(buffer,path,strlen(path));
                     strcat_s(answer,buffer);
                 }
             }
             n = send(new_socket,answer,strlen(answer),0);
    //         if (n < 0)
    //             error("ERROR writing to socket");
             status = _pclose(fp);
             if (status == -1) {
                 error("Error with executing of command\n");
             }
         }
     }

  /*  while(1)
    {
		clilen = sizeof(struct sockaddr_in);
		printf("%i\n",clilen);
		newsockfd = accept(sockfd , (struct sockaddr *)&cli_addr, &clilen);
		if (newsockfd == INVALID_SOCKET)
		{
			printf("accept failed with error code : %d" , WSAGetLastError());
		}
		printf("Est' soedineniye\n");

		HANDLE myHandle1 = CreateThread(0, 0, myThread1,&newsockfd, 0, &myThreadID);
		//CloseHandle(myHandle1);
    }*/
    closesocket(sockfd);
    WSACleanup();
    return 0;
}
   /* WSADATA wsa;
    SOCKET s , new_socket;
    struct sockaddr_in server , client;
    int c;
    char *message;
  
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
      
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
      
    //Bind
    if( bind(s ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d" , WSAGetLastError());
    }
      
    puts("Bind done");
  
    //Listen to incoming connections
    listen(s , 5);
      
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    
  	
	while(1)
	{
		c = sizeof(struct sockaddr_in);
		new_socket = accept(s , (struct sockaddr *)&client, &c);
		if (new_socket == INVALID_SOCKET)
		{
			printf("accept failed with error code : %d" , WSAGetLastError());
		}
      
		puts("Connection accepted");
	DWORD myThreadID;
		
		HANDLE myHandle1 = CreateThread(0, 0, myThread1,&new_socket, 0, &myThreadID);
		CloseHandle(myHandle1);
	}*/
      