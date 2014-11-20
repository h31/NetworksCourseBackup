// MiniServer.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "MyMiniServer.h"


int _tmain(int argc, _TCHAR* argv[])
{
	MyMiniServer server;
	server.setupLib();
	server.runTCP();
	server.cleanLib();
	return 0;
}

