#pragma once



//********************************************
const int TCPregime = 1;					//
const int UDPregime = 2;					//
const int CurrentRegime = TCPregime;		//
//********************************************


#include "CrossPlatformDefines.h"
#include "MyLogger.h"
#include "Package.h"
#include "UserList.h"


struct SendMetaInfo //tcp, common userlist
{
    mysocket socket;
	UserList *userlist;
};

class MyServer
{
public:
	//constants
    static const int DEFAULT_PORT  = 34343;
	
    static const int USER_TIMEOUT= (1*60 + 20)*1000;  //inactive time to disconnect users=  1 min 20 sec
	static const int USER_WARNING_TIME = (0*60 + 20)*1000;	//time remains to warning message = 0 min 20 sec
    static const int USER_UPDATE_TIME_S = 3;	//update users every 3 sec  (linux) 
    static const int USER_UPDATE_TIME_MS = USER_UPDATE_TIME_S*1000; // update users every 3000 msec (win)

	static const int ADVERT_SEND_PAUSE_S = 60;	//send advert message every 60 sec (linux)
	static const int ADVERT_SEND_PAUSE_MS = ADVERT_SEND_PAUSE_S*1000; //send advert message every 1000 ms (windows)



    //constr /destr
	MyServer(void);
	~MyServer(void);

    //
    UserList userlist;

	//crossplatform thread functions
#ifdef _WIN32
	static returnType WINAPI Communicate(void  * socket_param); //send/receive data function
	static returnType WINAPI TimerUserTimeOut(void * user_list_arg);	//delete timeout users
	static returnType WINAPI AdvertMessage(void * user_list_arg);	//send all active users advert`s
#elif __linux
	static returnType Communicate(void  * socket_param); //send/receive data function
	static returnType TimerUserTimeOut(void * user_list_arg); 	//delete timeout users
	static returnType AdvertMessage(void * user_list_arg); //send all active users advert`s
#endif
	//functions
    sockaddr_in createListenAddr();
    bool runTCP();
	bool runUDP();
    bool SetupLibrary();
    void CleanUpLibrary();
    Command ParceIncomingPackage(char * packagedata);

	static void SendData(Package package, mysocket socket,string info_message);
	static void SendDataUDP(Package package,sockaddr_in addr, mysocket socket,string info_message);
	static void closeSocket(mysocket socket);

	
};

