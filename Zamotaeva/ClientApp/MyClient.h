#pragma once
#include <WinSock2.h>
#include <windows.h>
#include "..//..//CommonSrc//Package.h"
#include "..//..//CommonSrc//MyLogger.h"
class MyClient
{
public:
	static const int DEFAULT_CONNECTION_PORT = 34343;
	//char DEFAULT_SERVER_IP[16];

	MyClient(void);
	~MyClient(void);
	SOCKET socketDescriptor;
	bool CreateDefaultSocket(void);
	struct sockaddr_in servaddr;
	void SetServerConnectionsParams(void);
	bool SetupLibrary();
	void CleanUpLibrary();
	bool Connect(void);
	void Listen(void);
	void SendTo(Package pack);
	void CloseSocket(SOCKET s);
};

