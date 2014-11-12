#ifndef _MYCLIENT_H_
#define _MYCLIENT_H_


#include "PackageStructure.h"
#include "CrossPlatformDefines.h"

#ifdef __linux__ 
    #include <sys/socket.h>
    #include <arpa/inet.h>
    typedef int mysocket;
#elif _WIN32
	#include <WinSock2.h>
	#include <windows.h>
	typedef SOCKET mysocket;
	
#else

#endif

//#define struct sockaddr_in SockAddress


static const int DEFAULT_CONNECTION_PORT = 34343;
void SetUpLib();
void SendPackage(mysocket s,PackageStructure package,sockaddr_in servaddr);
void SendPackage(mysocket s,PackageStructure package);
sockaddr_in getDefaultAddressParam();
sockaddr_in getListenAddressParam();
mysocket createConnection();


#endif

