#ifndef _MYCLIENT_H_
#define _MYCLIENT_H_



#include "PackageStructure.h"
#include "CrossPlatformDefines.h"
#include "MessageCollection.h"
#include "User.h"

//********************************************
const int TCPregime = 1;					//
const int UDPregime = 2;					//
const int CurrentRegime = TCPregime;		//
//********************************************

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
	typedef int socklen_t;
	
#else

#endif

#include "MyLogger.h"




struct SendMetaInfo //for IO thread
{
	char * login;
	UserList * userlist;
	mysocket *s;
	MessageCollection * collection;
	int CurRegime;
};
enum ActionID
{
	ACTION_SHOW_MESSAGE  =1,
	ACTION_SEND_MESSAGE  =2,
	ACTION_SHOW_CONTACTS = 3,
	ACTION_QUIT = 4,
};


static const int DEFAULT_PORT = 34343;

static const int MAX_PACKAGE_SIZE = 512;

bool SetUpLib();
void CleanLib();

sockaddr_in createSendAddr();
mysocket createSocket();
bool runTCP();
bool runUDP();
void Listen(void);
void * Answer( void * arg);
static void closeSocket(mysocket socket);
int sendData(mysocket s,char *data);
int sendDataUDP(mysocket s,sockaddr_in addr,char *data);

#ifdef _WIN32
	static returnType WINAPI InputText(void  * info_param);
#elif __linux
	static returnType InputText(void  * info_param);
#endif

#endif

char * inputLogin();
int  getAction(SendMetaInfo info);
char * inputMessage();
User GetUser(UserList* list);
void closeSocket(mysocket socket);
