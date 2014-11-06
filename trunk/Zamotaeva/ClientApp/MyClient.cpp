#include "StdAfx.h"
#include "MyClient.h"
#include <string.h>

#pragma comment(lib, "WS2_32.lib")



MyClient::MyClient(void)
{
	this->socketDescriptor = 0;
	//strcpy(DEFAULT_SERVER_IP, "127.000.000.001");
}


MyClient::~MyClient(void)
{
}


bool MyClient::CreateDefaultSocket(void)
{
	this->socketDescriptor = socket(AF_INET,SOCK_STREAM,0);
	if(socketDescriptor == INVALID_SOCKET)
	{
		return false;
	}
	return true;
}


void MyClient::SetServerConnectionsParams(void)
{
	servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//servaddr.sin_addr.s_addr = inet_addr("192.168.0.11");
    servaddr.sin_port = htons( DEFAULT_CONNECTION_PORT );
}

bool MyClient::SetupLibrary()
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


bool MyClient::Connect(void)
{
	connect(socketDescriptor, (struct sockaddr *)&servaddr, sizeof(servaddr));
	return true;
}

void MyClient::SendTo(Package pack) // otpr pack
{
	int len = pack.packagesize;
	int sendsize = sendto(socketDescriptor, (const char*)pack.data,len , 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
	if(sendsize > 0)
	{
		char* text = new char[30];
		//memccpy(text,0,0,30);
		sprintf(text,"send:%i",sendsize);
		MyLogger::WriteSuccessNetworkEvent(text);
	}
	else
	{
		MyLogger::WriteFailNetworkEvent("send data");
	}
	pack.OutPackage();
}

void MyClient::CleanUpLibrary()
{
	WSACleanup();
}

void MyClient::CloseSocket(SOCKET s)
{
	closesocket(s);
}