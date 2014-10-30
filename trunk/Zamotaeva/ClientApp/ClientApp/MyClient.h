#pragma once
#include <WinSock2.h>
#include <windows.h>
#include "..//..//ServerApp/ServerApp/Package.h"
#include "..//..//ServerApp/ServerApp/MyLogger.h"
class MyClient
{
public:
	static const int DEFAULT_CONNECTION_PORT = 34343;

	MyClient(void);
	~MyClient(void);
	SOCKET socketDescriptor;
	bool CreateDefaultSocket(void);
	struct sockaddr_in servaddr;
	void SetServerConnectionsParams(void);
	bool Connect(void);
	void Listen(void);
	void SendTo(Package pack);
};

