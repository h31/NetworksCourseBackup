
#include <stdlib.h> 
#include <string.h>

#include "stdafx.h"
#include "CommandFactory.h"
#include "CommandsIDs.h"
CommandStructure GenarateEchoRequestCommand()
{
	CommandStructure echoreqcmd;
	echoreqcmd.CommandId = ECHO_REQUEST;
	char * data = (char*)malloc(sizeof(char)*1);
	data[0] = '1';
	echoreqcmd.data = data;
	echoreqcmd.data_len = 1; 

	return echoreqcmd;
};

CommandStructure GenarateEchoAnswerCommand()
{
	CommandStructure echoanscmd;
	echoanscmd.CommandId = ECHO_ANSWER;
	return echoanscmd;

};
CommandStructure GenarateLoginRequestCommand(char *login_name)
{
	CommandStructure loginreqcmd;
	loginreqcmd.CommandId = LOGIN_REQUEST;
	loginreqcmd.data = login_name;
	loginreqcmd.data_len = strlen(login_name);
	return loginreqcmd;
};
CommandStructure GenarateLoginAnswerCommand(bool answer)
{
	CommandStructure loginanscmd;
	loginanscmd.CommandId = LOGIN_ANSWER;
	return loginanscmd;
};

CommandStructure GenarateAdvCommand(char *adv_message)
{
	CommandStructure advcmd;
    advcmd.CommandId = ADVERT_MESSAGE;
	return advcmd;
};
CommandStructure GenarateMessageSendCommand(MessageStructure msg_str)
{
	CommandStructure messndcmd;
	messndcmd.CommandId = MESSAGE_SEND;
	char * data =  ConvertMessageToData(msg_str);
	messndcmd.data =data;

	messndcmd.data_len = strlen(data);

	return messndcmd;
};
CommandStructure GenarateMessageDeliverCommand()
{
	CommandStructure mesdelcmd;
    mesdelcmd.CommandId = MESSAGE_DELIVERY;
	return mesdelcmd;
};
