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
	//return (socketDescriptor != INVALID_SOCKET);
	return true;
}



void MyServer::SetServerConnectionsParams(void)
{
	server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( DEFAULT_CONNECTION_PORT );
}


bool MyServer::BindSocket(void)
{
	return  ( bind(this->socketDescriptor,(struct sockaddr *)&server , sizeof(server)) < 0);
}


void MyServer::Listen(void)
{
	
	//Accept and incoming connection
    puts("Waiting for incoming connections...");
	int clilen  =  sizeof(client_addr);
	listen(socketDescriptor , SOMAXCONN);
	//accept(socketDescriptor,)
	newsocketDesc = accept(socketDescriptor,(struct sockaddr *)&client_addr, &clilen);
	BYTE* buffer = new BYTE[255];
	int read_size = recv(newsocketDesc , (char*)buffer , sizeof(buffer) , 0); //poluhaem
	MyLogger::WriteSuccessEvent((char*)buffer);



}


void MyServer::ParceIncomingPackage(BYTE * package)
{
	
}
