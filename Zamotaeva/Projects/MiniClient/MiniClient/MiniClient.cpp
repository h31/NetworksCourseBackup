// MiniClient.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "MyMiniClient.h"

int _tmain(int argc, _TCHAR* argv[])
{
	MyMiniClient client;
	client.setupLib();
	client.runTCP();
	client.cleanLib();
	return 0;
}

