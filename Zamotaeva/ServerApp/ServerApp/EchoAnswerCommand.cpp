#include "StdAfx.h"
#include "EchoAnswerCommand.h"


EchoAnswerCommand::EchoAnswerCommand(void): Command(CommandsIDs::ECHO_ANSWER)
{
	
}


EchoAnswerCommand::~EchoAnswerCommand(void)
{
}


BYTE* EchoAnswerCommand::GenerateData()
{
	BYTE * answer = new BYTE[1];
	BYTE ans = '1';
	answer[0] = ans;
	this->data = answer;
	return answer;
}


