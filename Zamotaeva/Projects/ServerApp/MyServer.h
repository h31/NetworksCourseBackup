#pragma once

#ifdef _WIN32
#include <WinSock2.h>
#include <windows.h>
typedef SOCKET mysocket;
#elif __linux
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
typedef int mysocket;
#endif


#include "../CommonSrc/MyLogger.h"
#include "..//CommonSrc/Package.h"
#include "..//CommonSrc/Command.h"
#include "UserList.h"
#include "CrossPlatformDefines.h"

class MyServer
{
public:
	static const int DEFAULT_CONNECTION_PORT = 34343;
    static const int USER_TIMEOUT= 3*60*1000;  //3min = 180s = 180000ms;


    //
	MyServer(void);
	~MyServer(void);

    //
    mysocket listenSocketDescriptor;
    mysocket newsocketDesc;
    mysocket sendsocketDescriptor;
    UserList userlist;

    // methods
    bool CreateListenSocket(void);
    static mysocket CreateWriteSocket(void);
    static void SendPackage(mysocket s,Package package,sockaddr_in servaddr);
    struct sockaddr_in server,servclient ,client_addr;
    void SetDefaultServerConnectionsParams(void);
    static sockaddr_in SetClientConnectionsParams(char * ipaddr);
	bool BindSocket(void);
	void Listen(void);
    Command ParceIncomingPackage(char * packagedata);
	void AnalyzeIncomingCommand(Command cmd);
	bool SetupLibrary();
	void CleanUpLibrary();
    void CloseSocket(mysocket s);
    static void * Answer(void * cmd_arg);
    char* getCleanBuffer();
};

