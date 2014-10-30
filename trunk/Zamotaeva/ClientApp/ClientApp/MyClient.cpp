#include "StdAfx.h"
#include "MyClient.h"

#pragma comment(lib, "WS2_32.lib")



MyClient::MyClient(void)
{
	this->socketDescriptor = 0;
}


MyClient::~MyClient(void)
{
}


bool MyClient::CreateDefaultSocket(void)
{
	this->socketDescriptor = socket(AF_INET,SOCK_STREAM,0);
	//return (socketDescriptor != INVALID_SOCKET);
	return true;
}


void MyClient::SetServerConnectionsParams(void)
{
	servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons( DEFAULT_CONNECTION_PORT );
}


bool MyClient::Connect(void)
{
	connect(socketDescriptor, (struct sockaddr *)&servaddr, sizeof(servaddr));
	return true;
}

void MyClient::SendTo(Package pack) // otpr pack
{
	sendto(socketDescriptor, (const char*)pack.data, sizeof(pack.data), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
	MyLogger::WriteSuccessEvent((char*)pack.data);
}