// ClientApp.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "MyClient.h"
#include "..//..//CommonSrc/MyLogger.h"
#include "..//..//CommonSrc/EchoRequestCommand.h"
#include "..//..//CommonSrc/LoginRequestCommand.h"
#include "..//..//CommonSrc/InitIpRequest.h"
#include "..//..//CommonSrc/GetMyIP.h"
#include "..//..//CommonSrc/ApplicationOptions.h"

int _tmain(int argc, _TCHAR* argv[]) //prilog - vipoln command v nugn poradke
{


	ApplicationOptions::setClientAppOptions();
	
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
	
	bool isConnectSuccess = client->Connect();
	if(isConnectSuccess == false)
	{
		MyLogger::WriteFailNetworkEvent("Connect");
	}
	else
	{
		MyLogger::WriteSuccessNetworkEvent("Connect");
	}


	Package* pack = new Package();


	EchoRequestCommand* echoRcmd = new EchoRequestCommand(); //client zapros na nalich servera
	echoRcmd->GenerateData(); //pust data
	Package  echopackage = pack->Generate(*echoRcmd); // preobr CMD (ID DATA) v PACKAGE
	client->SendTo(echopackage);

	pack = new Package();
	InitIpRequest * ipreq = new InitIpRequest();
	ipreq->GenerateData();
	Package ippack = pack->Generate(*ipreq);
	client->SendTo(ippack);

	pack = new Package();
	LoginRequestCommand * loginreq = new LoginRequestCommand("Yulia");
	loginreq->GenerateData();
	Package loginpack = pack->Generate(*loginreq);
	client->SendTo(loginpack);





	client->CloseSocket(client->socketDescriptor);
	client->CleanUpLibrary();
	
}

