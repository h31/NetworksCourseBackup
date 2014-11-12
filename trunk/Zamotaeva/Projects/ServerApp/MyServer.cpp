#include "stdafx.h"
#include "MyServer.h"
#include <unistd.h>

#ifdef _WIN32
#pragma comment(lib, "WS2_32.lib")
#endif



MyServer::MyServer(void) //klass server konstructor
{
	 socketDescriptor  = 0; //ident socketa
}


MyServer::~MyServer(void)
{
}


bool MyServer::CreateDefaultSocket(void)
{
	this->socketDescriptor = socket(AF_INET,SOCK_STREAM,0);
	if(socketDescriptor == INVALID_SOCKET)
	{
		return true;
	}
	return true;
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


void MyServer::SetServerConnectionsParams(void)
{
	server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( DEFAULT_CONNECTION_PORT );
}


bool MyServer::BindSocket(void)
{
	return  ( bind(this->socketDescriptor,(struct sockaddr *)&server , sizeof(server)) >= 0);
}


void MyServer::Listen(void)
{
	
	//Accept and incoming connection
    puts("Waiting for incoming connections...");
    unsigned int clilen  =  sizeof(client_addr);
	listen(socketDescriptor , SOMAXCONN);
	//accept(socketDescriptor,)
	newsocketDesc = accept(socketDescriptor,(struct sockaddr *)&client_addr, &clilen);


	int read_size = -1;
    char* buffer = new char[255];
	for(int i=0;i<255;i++)
	{
		buffer[i] = '\0';
	}
	do
	{
		
		read_size = recv(newsocketDesc , (char*)buffer , 256 , 0); //poluhaem
		if(read_size > 0)
		{
			MyLogger::WriteMessage(buffer);
			ParceIncomingPackage(buffer);
			
		}
		else
		{
			MyLogger::WriteFailNetworkEvent("receive message");
		}
		buffer = getCleanBuffer();
	}
	while(read_size > 0);
	this->CloseSocket(newsocketDesc);


}


Command MyServer::ParceIncomingPackage(char * packagedata)
{
	Package pack;
	Command cmd = pack.Parse(packagedata);
	cmd.OutCommand();
	return cmd;

}


void MyServer::AnalyzeIncomingCommand(Command cmd)
{
	if(cmd.commandID == CommandsIDs::ECHO_ANSWER)
	{
		//send echo_request
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

