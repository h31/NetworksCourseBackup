
#include <stdlib.h> 
#include <string.h>

#include "stdafx.h"
#include "CommandFactory.h"
#include "CommandsIDs.h"
#include "MyCreator.h"


CommandStructure GenerateEchoRequestCommand()
{
	CommandStructure echoreqcmd;
	echoreqcmd.CommandId = ECHO_REQUEST;
	char * data = getCleanBuffer(1);
	data[0] = '1';
	echoreqcmd.data = data;
	echoreqcmd.data_len = 1; 

	return echoreqcmd;
};

CommandStructure GenerateEchoAnswerCommand()
{
	CommandStructure echoanscmd;
	echoanscmd.CommandId = ECHO_ANSWER;
	char * data = getCleanBuffer(1);
	data[0] = '1';
	echoanscmd.data = data;
	echoanscmd.data_len = 1; 

	return echoanscmd;
}

CommandStructure GenerateLoginRequestCommand(char *login_name)
{
	int login_len = strlen(login_name);
	char * data = getCleanBuffer(login_len+2);
	sprintf(data,"%2d%s",login_len,login_name);
	for(int i=0;i<2;i++)
	{
		if(data[i]==' ')
		{
			data[i] = '0';
		}
	}
	CommandStructure loginreqcmd;
	loginreqcmd.CommandId = LOGIN_REQUEST;
	loginreqcmd.data = data;
	loginreqcmd.data_len = strlen(login_name)+2;
	return loginreqcmd;
};

CommandStructure GenerateMessageSendCommand(MessageStructure msg_str)
{
	CommandStructure messndcmd;
	messndcmd.CommandId = MESSAGE_SEND;
	char * data =  ConvertMessageToData(msg_str);
	messndcmd.data =data;

	messndcmd.data_len = strlen(data);

	return messndcmd;
};

CommandStructure GenerateUsersRequestCommand()
{
	CommandStructure usersreqcmd;
	usersreqcmd.CommandId = USERS_REQUEST;
	char * data = getCleanBuffer(1);
	data[0] = '1';
	usersreqcmd.data = data;
	usersreqcmd.data_len = 1; 
	return usersreqcmd;
}

CommandStructure GenerateQuitRequest()
{
	CommandStructure quitreqcmd;
	quitreqcmd.CommandId = QUIT_REQUEST;
	char * data;
	data = getCleanBuffer(1);
	data[0] = '1';
	quitreqcmd.data = data;
	quitreqcmd.data_len = 1; 
	return quitreqcmd;
}
