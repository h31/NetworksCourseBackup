// ClientApp.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "MyClient.h"
#include "..//..//ServerApp/ServerApp/MyLogger.h"
#include "EchoRequestCommand.h"

int _tmain(int argc, _TCHAR* argv[]) //prilog - vipoln command v nugn poradke
{
	MyClient* client = new MyClient();

	bool isCreateSocketSuccess = client->CreateDefaultSocket();
	if(isCreateSocketSuccess == false)
	{
		MyLogger::WriteFailEvent("Create socket");
		exit(1);
	}
	else
	{
		MyLogger::WriteSuccessEvent("Create socket");
	}
	client->SetServerConnectionsParams();
	bool isConnectSuccess = client->Connect();
	if(isConnectSuccess == false)
	{
		MyLogger::WriteFailEvent("Connect");
	}
	else
	{
		MyLogger::WriteSuccessEvent("Connect");
	}
	EchoRequestCommand* echoRcmd = new EchoRequestCommand(); //client zapros na nalich servera
	echoRcmd->GenerateData(); //pust data
	Package* pack = new Package();
	Package  echopackage = pack->Generate(*echoRcmd); // preobr CMD (ID DATA) v PACKAGE

	client->SendTo(echopackage);
}

