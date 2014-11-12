#include "CommandFactory.h"
#include "CommandsIDs.h"
#include <stdlib.h> 
#include <string.h>

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
	advcmd.CommandId = ADV_MESSAGE;
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
	mesdelcmd.CommandId = MESSAGE_DELIVER;
	return mesdelcmd;
};
