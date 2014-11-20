#pragma once



#ifdef _WIN32
#include <WinSock2.h>
#include <windows.h>
typedef SOCKET mysocket;
typedef DWORD returnType; //win thread
typedef int sock_len;
#elif __linux
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
typedef int mysocket;
typedef void * returnType;
typedef socklen_t sock_len;
#endif




#define TCPregime 1
#define UDPregime 0

#ifndef CurrentRegime
#define CurrentRegime TCPregime
#endif


#include "../CommonSrc/MyLogger.h"
#include "..//CommonSrc/Package.h"
#include "UserList.h"
#include "CrossPlatformDefines.h"

struct SendMetaInfo
{
    mysocket socket;
};

class MyServer
{
public:

    static const int DEFAULT_PORT  = 34343;
    static const int USER_TIMEOUT= 3*60*1000;  //3min = 180s = 180000ms;
    static const int USER_UPDATE_TIME_S = 3;
    static const int USER_UPDATE_TIME_MS = USER_UPDATE_TIME_S*1000;



    //
	MyServer(void);
	~MyServer(void);

    //
    static UserList userlist;

    // methods
    //bool BindSocket(void);
#ifdef _WIN32
	static returnType WINAPI Communicate(void  * socket_param); //thread function
#elif __linux
	static returnType Communicate(void  * socket_param);
#endif
    sockaddr_in createListenAddr();
    bool runTCP();
	bool runUDP();
    bool SetupLibrary();
    void CleanUpLibrary();


    Command ParceIncomingPackage(char * packagedata);


    static void * TimerUserTimeOut(void * user_list_arg);
    char* getCleanBuffer();
};

