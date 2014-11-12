#include "stdafx.h"

#include "MyClient.h"
#include "PackageStructure.h"
#include "MessageStructure.h"
#include "GetLocalIP.h"
#include "GetCurrentTime.h"
#include "CommandFactory.h"
#include "CrossPlatformDefines.h"
#include <stdlib.h>


int main(int argc, _TCHAR* argv[])
{
	char * login = (char*) malloc(sizeof(char)*256);
	char * message = (char*) malloc(sizeof(char)*256);
	char * ip = (char*) malloc(sizeof(char)*16);


	printf("input your name\n");
	scanf("%255s",login);
	
	printf("input message:\n");
	scanf("%255s",message);

	char* time = getCurrentTime();

	MessageStructure messtr;
	messtr.message = message;
	messtr.message_len = strlen(message);
	messtr.sender_name = login;
	messtr.sender_len = strlen(login);
	messtr.dest_name = "server";
	messtr.dest_len  = 6;
	messtr.time_stamp = time;
	messtr.time_len = strlen(time);


	SetUpLib();
    char * ipaddr = getIPAddress();
    printf("ip = %s\n",ipaddr);
	mysocket sock =  createConnection();


	//char* msgdata = ConvertMessageToData(messtr);
	//parseData(msgdata);
    printf("sending echo request");
	CommandStructure EchoRequestCmd = GenarateEchoRequestCommand();
	PackageStructure package_echo_req = CreatePackage(EchoRequestCmd,ipaddr);
	SendPackage(sock,package_echo_req);
    printf("sending message");
	CommandStructure MessageSendCmd = GenarateMessageSendCommand(messtr);
	PackageStructure package_mes = CreatePackage(MessageSendCmd,ipaddr);
	SendPackage(sock,package_mes);
	return 0;
}
