#include "MyMiniClient.h"
#include <stdio.h>
#pragma comment(lib, "WS2_32.lib")

MyMiniClient::MyMiniClient(void)
{
}


MyMiniClient::~MyMiniClient(void)
{
}


bool MyMiniClient::setupLib()
{
	WSADATA wsaData;
    int errorcode = WSAStartup(MAKEWORD(2,2),&wsaData);
    if (errorcode)
	{
		printf("fail setup lib\n");
		return false;
	}
	else
	{
		printf("setup lib\n");
		return true;
	}
}

bool MyMiniClient::cleanLib()
{
	WSACleanup();
	return true;
}

SOCKET MyMiniClient::createSocket()
{
	return socket(AF_INET,SOCK_STREAM,0);
}

sockaddr_in MyMiniClient::createListenAddr()
{
	sockaddr_in addr;
	addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons( 12345 );
	return addr;
}

sockaddr_in  MyMiniClient::createSendAddr()
{
	sockaddr_in addr;
	addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons( 12345 );
	return addr;
}

bool MyMiniClient::runTCP()
{
	SOCKET s = createSocket();
	if(s == INVALID_SOCKET)
	{
		printf("bad socket\n");
		return false;
	}
	sockaddr_in adr = createSendAddr();
	int addr_len = sizeof(adr);
	int connectionResult =  connect(s, (struct sockaddr *)&adr, addr_len);
	if(connectionResult < 0)
	{
		printf("bad connection\n");
		return false;
	}
	else
	{
		printf("good connection\n");
	}

	sockaddr_in incomingAddr;
	//SOCKET s_in =  accept(s, (struct sockaddr *)&incomingAddr, &addr_len);
	int send_size = 0;
	int k = 1;
	char * buffer = new char[256];
	for (int i=1;i<100;i+=2, k+=2)
	{
		if(k >= 10)
		{
			k -=10;
		}
		char* data = new char[2];
		sprintf_s(data,2,"%1d",k);
		send_size = send(s,data,1,0);
		if(send_size > 0)
		{
			printf("->%s\n",data);
		}
		else
		{
			printf("sending error\n");
		}
		if (recv (s,buffer,256,0) < 0)
		{
			printf("receive error\n");
		}
		printf("<-%s\n",buffer);
		Sleep(1000);
		delete data;

	}
	shutdown(s,SD_BOTH);
	closesocket(s);
}
