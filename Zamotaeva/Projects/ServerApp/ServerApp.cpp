// ServerApp.cpp:

#include "stdafx.h"
#include "MyServer.h"

#include "../CommonSrc/ApplicationOptions.h"

int main(int argc, _TCHAR* argv[])
{
	ApplicationOptions::setServerAppOptions();
	MyServer* server = new MyServer();
	server->SetupLibrary();
	server->SetServerConnectionsParams();
	bool isCreateSocketSuccess = server->CreateDefaultSocket();
	if(isCreateSocketSuccess == false)
	{
		MyLogger::WriteFailNetworkEvent("Create socket");
        return 1;
	}
	else
	{
		MyLogger::WriteSuccessNetworkEvent("Create socket");
	}
	
	bool isBindSuccess = server->BindSocket();
	if(isBindSuccess == false)
	{
		MyLogger::WriteFailNetworkEvent("Bind socket");
	}
	else
	{
		MyLogger::WriteSuccessNetworkEvent("Bind socket");
	}

	server->Listen();
	server->CleanUpLibrary();
}

