#ifndef _MYCLIENT_H_
#define _MYCLIENT_H_



#include "PackageStructure.h"
#include "CrossPlatformDefines.h"

#ifdef __linux__ 
    #include <sys/socket.h>
    #include <arpa/inet.h>
    typedef int mysocket;
	typedef unsigned int addr_size;
    typedef void * returnType;

#elif _WIN32
	#include <WinSock2.h>
	#include <windows.h>
	typedef SOCKET mysocket;
	typedef int addr_size;
	typedef DWORD  returnType;
	
#else

#endif



static const int DEFAULT_PORT = 34343;

static const int MAX_PACKAGE_SIZE = 512;

bool SetUpLib();
void CleanLib();

sockaddr_in createSendAddr();
sockaddr_in createListenAddr();
mysocket createSocket();
char*  getCleanBuffer();
bool runTCP();
void Listen(void);
void * Answer( void * arg);


#ifdef _WIN32
	//static returnType WINAPI InputText(void  * text_ref_param);
#elif __linux
	static returnType Communicate(void  * text_ref_param);
#endif

#endif

char * inputLogin();
char * chooseReceiver();
char * chooseAction();
char * inputMessage();

