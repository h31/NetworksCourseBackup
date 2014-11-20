#include "stdafx.h"
#include "MyServer.h"

#include "../CommonSrc/commandFactory.h"

#ifdef _WIN32
#pragma comment(lib, "WS2_32.lib")
#elif __linux
#include <unistd.h>
#endif



MyServer::MyServer(void) 
{
}


MyServer::~MyServer(void)
{
}

char*  MyServer::getCleanBuffer()
{
    char * buffer = new char[Package::MAX_PACKAGE_SIZE];
    for(int i=0;i<Package::MAX_PACKAGE_SIZE;i++)
	{
		buffer[i] = '\0';
	}
	return buffer;
}

bool MyServer::SetupLibrary()
{
    #ifdef _WIN32 //Windows
	
	WSADATA wsaData;
    int errorcode = WSAStartup(MAKEWORD(2,2),&wsaData);
    if (errorcode)
	{
		MyLogger::WriteFailNetworkEvent("setup lib");
		return false;
	}
	else
	{
		MyLogger::WriteSuccessNetworkEvent("setub lib");
		return true;
	}
    #endif
    return true;
}

void MyServer::CleanUpLibrary()
{
    #ifdef _WIN32
	WSACleanup();
    #endif
}

 sockaddr_in MyServer::createListenAddr()
 {
     sockaddr_in serv;
     serv.sin_family = AF_INET;
     serv.sin_addr.s_addr = INADDR_ANY;
     serv.sin_port = htons( DEFAULT_PORT );
     return serv;

 }
 bool MyServer::runTCP()
 {
     mysocket s = socket(AF_INET,SOCK_STREAM,0);
     if(s == INVALID_SOCKET)
     {
         return false;
     }
     
     sockaddr_in server_addr = createListenAddr();
     int servaddr_len = sizeof(server_addr);
     if (bind(s,(struct sockaddr * )&server_addr,servaddr_len) < 0)
	 {
		 printf("bind error\n");
		 return false;
	 }
     sockaddr_in in_addr;
     sock_len addr_len = sizeof(in_addr);
     listen(s , SOMAXCONN);
     int read_size = 0;
     mysocket s_in;
     printf("wait connections\n");
     while( s_in = accept(s , (struct sockaddr * )&in_addr,&addr_len))
     {
		 printf(" connection accepted\n");
#ifdef _WIN32
         CreateThread(NULL,0,&Communicate, &s_in,0,NULL); 
#elif __linux
         pthread_t sendthread;
         pthread_create(&sendthread,NULL,Communicate,reinterpret_cast<void*>(&s_in));
         pthread_join(sendthread,NULL);
#endif
     }

#ifdef _WIN32
     shutdown(s,SD_BOTH);
     closesocket(s);
#elif __linux
     shutdown(s,SHUT_RDWR);
     close(s);
#endif
 }


void * MyServer::TimerUserTimeOut(void * user_list_arg)
{
    UserList userlist = *(reinterpret_cast<UserList*>(user_list_arg));
    userlist.DecreaseTimeUsers(MyServer::USER_UPDATE_TIME_MS);
    #ifdef __linux
    sleep(MyServer::USER_UPDATE_TIME_S);
    #elif _WIN32
    Sleep(MyServer::USER_UPDATE_TIME_MS);
    #endif
	return NULL;
}


returnType MyServer::Communicate(void  * socket_param)
{
    mysocket s_in = *(mysocket*)socket_param;

    char* buffer_in = new char[Package::MAX_PACKAGE_SIZE];
    int read_size = 0;
	int send_size = 0;
    while(read_size = recv(s_in,buffer_in,Package::MAX_PACKAGE_SIZE,0) > 0)
    {
            Command cmd = Package::Parse(buffer_in);
            if(cmd.commandID == CommandsIDs::ECHO_REQUEST)
            {
                printf("receive echo request\n");
                EchoAnswerCommand echoAnswer;
                echoAnswer.GenerateData();
                Package EchoPack = Package::Generate(echoAnswer);
                send_size = send(s_in,EchoPack.data,EchoPack.packagesize,0);
				if(send_size > 0)
				{
					printf("send echo answer[ %d bytes]\n",EchoPack.packagesize);
				}
            }
            if(cmd.commandID == CommandsIDs::LOGIN_REQUEST)
            {
				printf("receive login request\n");
            }
    }
	return NULL;
}
