#include "StdAfx.h"
#include "NetWorkController.h"
#include "..//..//CommonSrc/GetMyIP.h"
#include "..//..//CommonSrc/EchoRequestCommand.h"
#include "..//..//CommonSrc/InitIpRequest.h"
#include "..//..//CommonSrc/LoginRequestCommand.h"


NetWorkController::NetWorkController(void)
{
	InitController();
}


NetWorkController::~NetWorkController(void)
{
}


void NetWorkController::InitController()
{
	MyClient* client = new MyClient();
	client->SetupLibrary();
	client->SetServerConnectionsParams();
	bool isCreateSocketSuccess = client->CreateDefaultSocket();
	if(isCreateSocketSuccess == false)
	{
		MyLogger::WriteFailNetworkEvent("Create socket");
		exit(1);
	}
	else
	{
		MyLogger::WriteSuccessNetworkEvent("Create socket");
	}
	char* ipaddr = GetMyIP::getIPAddress();
	if(ipaddr != NULL && ipaddr != " " && ipaddr != "")
	{
		MyLogger::WriteObjectInfo("ip",ipaddr);
		
	}
	else
	{
		MyLogger::WriteFailNetworkEvent("Get IP address");
	}
}


void NetWorkController::ConnectToServer()
{
	bool isConnectSuccess = client->Connect();
	if(isConnectSuccess == false)
	{
		MyLogger::WriteFailNetworkEvent("Connect");
	}
	else
	{
		MyLogger::WriteSuccessNetworkEvent("Connect");
	}
}
void NetWorkController::CloseConnection()
{

	client->CloseSocket(client->socketDescriptor);
#ifdef _WINDOWS_
	client->CleanUpLibrary();
#endif
}

void NetWorkController::SendEchoRequest()
{
	EchoRequestCommand* echoRcmd = new EchoRequestCommand(); //client zapros na nalich servera
	echoRcmd->GenerateData(); //pust data
	Package* pack = new Package();
	Package  echopackage = pack->Generate(*echoRcmd); // preobr CMD (ID DATA) v PACKAGE
	client->SendTo(echopackage);
}


void NetWorkController::SendInitIPrequest()
{
	InitIpRequest *ipRcmd = new InitIpRequest();
	ipRcmd->GenerateData();
	Package* pack = new Package();
	Package ippackage = pack->Generate(*ipRcmd);
	client->SendTo(ippackage);
}

void NetWorkController::SendLoginRequest()
{

}