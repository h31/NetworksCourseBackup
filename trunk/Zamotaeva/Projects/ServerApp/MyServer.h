#pragma once

#ifdef _WIN32
#include <WinSock2.h>
#include <windows.h>
typedef SOCKET mysocket;
#elif __linux
#include <sys/socket.h>
#include <arpa/inet.h>
typedef int mysocket;
#endif


#include "../CommonSrc/MyLogger.h"
#include "..//CommonSrc/Package.h"
#include "..//CommonSrc/Command.h"
#include "CrossPlatformDefines.h"
class MyServer
{
public:
	static const int DEFAULT_CONNECTION_PORT = 34343;

	MyServer(void);
	~MyServer(void);
    mysocket socketDescriptor;
    mysocket newsocketDesc;
	bool CreateDefaultSocket(void);
	struct sockaddr_in server, client_addr;
	void SetServerConnectionsParams(void);
	bool BindSocket(void);
	void Listen(void);
    Command ParceIncomingPackage(char * packagedata);
	void AnalyzeIncomingCommand(Command cmd);
	bool SetupLibrary();
	void CleanUpLibrary();
    void CloseSocket(mysocket s);
    char* getCleanBuffer();
};

