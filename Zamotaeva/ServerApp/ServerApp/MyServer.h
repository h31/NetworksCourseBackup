#pragma once
#include <WinSock2.h>
#include <windows.h>
#include "MyLogger.h"
class MyServer
{
public:
	static const int DEFAULT_CONNECTION_PORT = 34343;

	MyServer(void);
	~MyServer(void);
	SOCKET socketDescriptor;
	SOCKET newsocketDesc;
	bool CreateDefaultSocket(void);
	struct sockaddr_in server, client_addr;
	void SetServerConnectionsParams(void);
	bool BindSocket(void);
	void Listen(void);
	void ParceIncomingPackage(BYTE * package);
};

