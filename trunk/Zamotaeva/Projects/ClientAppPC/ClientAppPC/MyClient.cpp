
#include <stdio.h>
#include <string.h>

#include "stdafx.h"
#include "MyClient.h"
#include "CommandFactory.h"
#include "CommandsIDs.h"

#ifdef _WIN32
	#pragma comment(lib, "WS2_32.lib")
#endif







sockaddr_in createListenAddr()
{
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( DEFAULT_PORT );
    return address;
}

sockaddr_in createSendAddr()
{
	sockaddr_in address;

	address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1"); //server adress
    address.sin_port = htons( DEFAULT_PORT );
	return address;
}




bool SetUpLib()
{
	#ifdef _WIN32
		WSADATA wsaData;
		int errorcode = WSAStartup(MAKEWORD(2,2),&wsaData);
        if (errorcode)
           {
               printf("fail setup lib\n");
               return false;
           }
           else
           {
               printf("setup lib\n");
               return true;
           }
	#endif
}

void CleanLib()
{
#ifdef _WIN32
    WSACleanup();
#endif
}


mysocket createSocket()
{
    return socket(AF_INET,SOCK_STREAM,0);
}

char*  getCleanBuffer()
{
    char * buffer = new char[MAX_PACKAGE_SIZE];
    for(int i=0;i<MAX_PACKAGE_SIZE;i++)
    {
        buffer[i] = ' ';
    }
    return buffer;
}


bool runTCP()
{
    mysocket s = createSocket();
    if(s == INVALID_SOCKET)
    {
        printf("bad socket\n");
        return false;
    }
    sockaddr_in adr = createSendAddr();
    addr_size addr_len = sizeof(adr);
    int connectionResult = connect(s,(struct sockaddr *)&adr, addr_len); //connect to server
    if(connectionResult < 0)
    {
        printf("bad connection\n");
        //return false;
    }

    sockaddr_in incomingAddr;
    int send_size = 0;
	int rec_size = 0;
    bool isOnWork = true; //client works - listen from server
    char * receivebuffer = (char*)malloc(sizeof(char)*MAX_PACKAGE_SIZE);

    CommandStructure EchoRequestCmd = GenarateEchoRequestCommand(); //client sends echo request
    PackageStructure package_echo_req = CreatePackage(EchoRequestCmd);
    char *senddata = GeneratePackageData(package_echo_req);
    int len = strlen(senddata);
    send_size = send(s,senddata,len,0);
	if(send_size < 0)
	{
		printf("fail to send echo request\n");
	}
	if(send_size > 0)
	{
		printf("send echo request\n");
	}
	printf("wait server answers\n");
    while(isOnWork) // client listen 
    {
		
        while (recv (s,receivebuffer,MAX_PACKAGE_SIZE,0) > 0)
        {
            //printf("receive: %s",receivebuffer);
			CommandStructure cmd = Parse(receivebuffer);
			if(cmd.CommandId == IDs::ECHO_ANSWER)
			{
				printf("receive echo answer. server online\n");
				char *login = inputLogin(); //client inputs login
				CommandStructure LoginRequestCmd = GenarateLoginRequestCommand(login); //command = type+data
				PackageStructure package_login_req = CreatePackage(LoginRequestCmd);
				char *sendlogindata = GeneratePackageData(package_login_req); //convert (6 0 1 -> 006001)
				len = strlen(sendlogindata);
				send_size = send(s,sendlogindata,len,0);
				if(send_size < 0)
				{
					printf("fail to send login request\n");
				}
				if(send_size > 0)
				{
					printf("send login request\n");
				}
			}
			if(cmd.CommandId == IDs::LOGIN_ANSWER)
			{

			}
        }
    }

	return true;
}


char * inputLogin()
{
	char * login = (char*) malloc(sizeof(char)*256);


	printf("input your name\n");
	scanf("%255s",login);
	return login;
}
char * inputMessage()
{
	char * message = (char*) malloc(sizeof(char)*256);
	printf("input message:\n");
    scanf("%255s",message);
	return message;
}