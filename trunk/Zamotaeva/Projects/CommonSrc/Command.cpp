#include "stdafx.h"
#include "Command.h"

Command::Command(int commandID)
{
	this->commandID = commandID;
}

Command::~Command(void)
{
}


void Command::OutCommand()
{
	printf("type:\t[%d]",commandID);
	switch(this->commandID)
	{
	case CommandsIDs::BAD_PACKAGE_ANSWER:
		printf("bad package");
		break;
	case CommandsIDs::ECHO_REQUEST:
		printf("echo request");
		break;
	case CommandsIDs::ECHO_ANSWER:
		printf("echo answer");
		break;

	case CommandsIDs::INIT_IP_REQUEST:
		printf("init ip request");
		break;
	case CommandsIDs::LOGIN_REQUEST:
		printf("login request");
		break;
	case CommandsIDs::MESSAGE_SEND:
		printf("message send");
		break;
	case CommandsIDs::MESSAGE_REC:
		printf("message deliver");
		break;

	default:
		printf("unknown");
		break;
	}
	printf("\n");
	printf("data:\t");
	if(data != NULL)
	{
		printf("%s\n",this->data);
	}
}
