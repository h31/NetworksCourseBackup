#include "stdafx.h"
#include <stdlib.h>

#include "MyClient.h"
#include "PackageStructure.h"
#include "MessageStructure.h"
#include "GetCurrentTime.h"
#include "CommandFactory.h"
#include "CrossPlatformDefines.h"




static char * getMessageReceiver(char** allusers, int nUsers)
{
    for(int i=0;i<nUsers;i++)
    {
        printf("%d)%s\n",i,allusers[i]);
    }
    printf("input user number:");
    int ch = 0;
    scanf("%d",&ch);
    if(ch)
    {

    }
    return allusers[0];
}

int main(int argc, _TCHAR* argv[])
{
    char * login = (char*) malloc(sizeof(char)*256);
    char * message = (char*) malloc(sizeof(char)*256);

	/*
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
	*/


    SetUpLib();
    runTCP();
    CleanLib();
    return 0;
}
