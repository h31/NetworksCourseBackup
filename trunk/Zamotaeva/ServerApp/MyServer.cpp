#include "StdAfx.h"
#include "MyServer.h"

#pragma comment(lib, "WS2_32.lib")



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


BYTE*  MyServer::getCleanBuffer()
{
	BYTE * buffer = new BYTE[256];
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
	int clilen  =  sizeof(client_addr);
	listen(socketDescriptor , SOMAXCONN);
	//accept(socketDescriptor,)
	newsocketDesc = accept(socketDescriptor,(struct sockaddr *)&client_addr, &clilen);


	int read_size = -1;
	BYTE* buffer = new BYTE[255];
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


void MyServer::ParceIncomingPackage(BYTE * packagedata)
{
	Package pack;
	Command cmd = pack.Parse(packagedata);
	cmd.OutCommand();

}

bool MyServer::SetupLibrary()
{
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
}

void MyServer::CleanUpLibrary()
{
	WSACleanup();
}

void MyServer::CloseSocket(SOCKET s)
{
	closesocket(s);
}
