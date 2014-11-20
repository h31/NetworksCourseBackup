#pragma once

#include <WinSock2.h>
#include <windows.h>

class MyMiniServer
{
public:
	MyMiniServer(void);
	~MyMiniServer(void);
	void Listen();
#ifdef _WIN32
	static DWORD WINAPI Communicate( void *  lpParam );
#elif __linux
	static void * Communicate(void * arg);
#endif
	SOCKET createSocket();
	sockaddr_in createListenAddr();
	bool setupLib();
	bool cleanLib();
	bool runTCP();
};

