#include "stdafx.h"
#include "MyServer.h"
#include <unistd.h>
#include "../CommonSrc/EchoAnswerCommand.h"

#ifdef _WIN32
#pragma comment(lib, "WS2_32.lib")
#endif



MyServer::MyServer(void) //klass server konstructor
{
     listenSocketDescriptor  = 0; //ident socketa
}


MyServer::~MyServer(void)
{
}


bool MyServer::CreateListenSocket(void)
{
    this->listenSocketDescriptor = socket(AF_INET,SOCK_STREAM,0);
    if(listenSocketDescriptor == INVALID_SOCKET)
	{
        return false;
	}
	return true;
}


mysocket MyServer::CreateWriteSocket(void)
{
    mysocket sendsocketDescriptor = socket(AF_INET,SOCK_STREAM,0);
    if(sendsocketDescriptor == INVALID_SOCKET)
    {
        return INVALID_SOCKET;
    }
    return sendsocketDescriptor;
}


char*  MyServer::getCleanBuffer()
{
    char * buffer = new char[256];
	for(int i=0;i<256;i++)
	{
		buffer[i] = '\0';
	}
	return buffer;
}


void MyServer::SetDefaultServerConnectionsParams(void)
{
	server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( DEFAULT_CONNECTION_PORT );
}

sockaddr_in MyServer::SetClientConnectionsParams(char * ipaddr)
{
    sockaddr_in newaddr;
    newaddr.sin_family = AF_INET;
    newaddr.sin_addr.s_addr = inet_addr(ipaddr);
    newaddr.sin_port = htons( DEFAULT_CONNECTION_PORT );
    return newaddr;
}


bool MyServer::BindSocket(void)
{
    return  ( bind(this->listenSocketDescriptor,(struct sockaddr *)&server , sizeof(server)) >= 0);
}


void MyServer::Listen(void)
{
	
	//Accept and incoming connection
    puts("Waiting for incoming connections...");
    unsigned int clilen  =  sizeof(client_addr);
    listen(listenSocketDescriptor , SOMAXCONN);
	//accept(socketDescriptor,)
    newsocketDesc = accept(listenSocketDescriptor,(struct sockaddr *)&client_addr, &clilen);


	int read_size = -1;
    char* buffer = new char[257];
	for(int i=0;i<255;i++)
	{
		buffer[i] = '\0';
	}
    for(;;)
	{
		
        read_size = recv(newsocketDesc , (char*)buffer , 256 , 0);
		if(read_size > 0)
		{
			MyLogger::WriteMessage(buffer);
            ParceIncomingPackage(buffer); // parse message and create thread
			
		}
		buffer = getCleanBuffer();
	}
	this->CloseSocket(newsocketDesc);


}


Command MyServer::ParceIncomingPackage(char * packagedata) // parse message and create thread
{
	Package pack;
	Command cmd = pack.Parse(packagedata);
    pthread_t sendthread;
    pthread_create(&sendthread,NULL,Answer,reinterpret_cast<void*>(&cmd));
    pthread_join(sendthread,NULL);
	cmd.OutCommand();
	return cmd;
}

void MyServer::AnalyzeIncomingCommand(Command cmd)
{

}

bool MyServer::SetupLibrary()
{
    #ifdef _WIN32
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

void MyServer::CloseSocket(mysocket s)
{
#ifdef _WIN32
	closesocket(s);
#elif __linux
    close(s);
#endif
}


 void * MyServer::Answer(void * cmd_arg) //thread function, cmd arg - parametr command from user
 {
    Command cmd = *(reinterpret_cast<Command*>(cmd_arg)); // convert type  - void - command
    if(cmd.commandID == CommandsIDs::ECHO_REQUEST) //opred type command
    {
        EchoAnswerCommand echoAnswer; //type of command
        echoAnswer.GenerateData(); //data


        Package pack;
        Package EchoPack;


        EchoPack = pack.Generate(echoAnswer); //command

        //SetClientConnectionsParams();
        mysocket writesocket = CreateWriteSocket();
        sockaddr_in addr = MyServer::SetClientConnectionsParams("127.0.0.1");
        SendPackage(writesocket,EchoPack,addr);
    }
    if(cmd.commandID == CommandsIDs::LOGIN_REQUEST)
    {
        //check login
        //answer
    }
    if(cmd.commandID == CommandsIDs::MESSAGE_SEND)
    {
        //get name
        //get ip by name
        //resend package
    }


 }

 void MyServer::SendPackage(mysocket s,Package package,sockaddr_in servaddr)
 {
     int len = package.packagesize;
     char * data =package.data;
     int sendsize = sendto(s, data ,len , 0, (struct sockaddr *) &servaddr, sizeof(servaddr));
     if(sendsize > 0)
     {

     }
 }

