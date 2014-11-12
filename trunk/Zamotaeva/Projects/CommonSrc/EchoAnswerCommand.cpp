#include "stdafx.h"
#include "EchoAnswerCommand.h"


EchoAnswerCommand::EchoAnswerCommand(void): Command(CommandsIDs::ECHO_ANSWER)
{
	
}


EchoAnswerCommand::~EchoAnswerCommand(void)
{
}


char* EchoAnswerCommand::GenerateData()
{
    char * answer = new char[1];
    char ans = '1';
	answer[0] = ans;
	this->data = answer;
	this->data_len = 1;
	return answer;
}


