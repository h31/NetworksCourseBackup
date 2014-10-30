// ServerApp.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "MyServer.h"
#include "MyLogger.h"

int _tmain(int argc, _TCHAR* argv[])
{
	MyServer* server = new MyServer();

	bool isCreateSocketSuccess = server->CreateDefaultSocket();
	if(isCreateSocketSuccess == false)
	{
		MyLogger::WriteFailEvent("Create socket");
		exit(1);
	}
	else
	{
		MyLogger::WriteSuccessEvent("Create socket");
	}
	server->SetServerConnectionsParams();
	bool isBindSuccess = server->BindSocket();
	if(isBindSuccess == false)
	{
		MyLogger::WriteFailEvent("Bind socket");
	}
	else
	{
		MyLogger::WriteSuccessEvent("Bind socket");
	}

	server->Listen();
}

