#pragma once

#include <WinSock2.h>
#include <windows.h>


class MyMiniClient
{
public:
	MyMiniClient(void);
	~MyMiniClient(void);
	bool runTCP();
	bool runUDP();
	bool setupLib();
	bool cleanLib();
	SOCKET createSocket();
	sockaddr_in createSendAddr();
	sockaddr_in createListenAddr();

};

