#include "MyMiniServer.h"
#include <stdio.h>


#pragma comment(lib, "WS2_32.lib")

MyMiniServer::MyMiniServer(void)
{
}


MyMiniServer::~MyMiniServer(void)
{
}

bool MyMiniServer::setupLib()
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
bool MyMiniServer::cleanLib()
{
	WSACleanup();
	return true;
}


SOCKET MyMiniServer::createSocket()
{
	return socket(AF_INET,SOCK_STREAM,0);
}



sockaddr_in MyMiniServer::createListenAddr()
{
	sockaddr_in serv;
	serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons( 12345 );
	return serv;

}
bool  MyMiniServer::runTCP()
{
	SOCKET s = socket(AF_INET,SOCK_STREAM,0);
	if(s == INVALID_SOCKET)
	{
		return false;
	}
	sockaddr_in server_addr = createListenAddr();
	int servaddr_len = sizeof(server_addr);
	bind(s,(struct sockaddr * )&server_addr,servaddr_len);
	
	sockaddr_in in_addr;	
	int addr_len = sizeof(in_addr);
	listen(s , SOMAXCONN);
	int read_size = 0;
	
	
	SOCKET s_in;
	printf("wait connections\n");
	while( s_in = accept(s , (struct sockaddr * )&in_addr,&addr_len))
	{
		CreateThread(NULL,0,Communicate, &s_in,0,NULL);
	}
#ifdef _WIN32
	shutdown(s,SD_BOTH);
	closesocket(s);
#elif __linux
	shutdown(s,SHUT_RDWR);
	close(s);
#endif
	return true;
}


DWORD  MyMiniServer::Communicate(void * arg)
{
	printf("start thread!\n");
	char* buffer_in = new char[512];
	char  empty = {' '};
	memccpy(buffer_in,&empty,0,512);
	int read_size = 0;
	SOCKET s_in = *(SOCKET*)arg;
	while(read_size = recv(s_in,buffer_in,512,0) > 0)
	{
		printf("<-%s\n",buffer_in);
		send(s_in,buffer_in,read_size,0);
		printf("->%s\n",buffer_in);
	}

	if(read_size== 0)
	{
		printf("client disconnected\n");
	}
	if(read_size < 0)
	{
		printf("receive error\n");
	}
	//memccpy(buffer_in,&empty,0,512);

	free(arg);
	return NULL;
}