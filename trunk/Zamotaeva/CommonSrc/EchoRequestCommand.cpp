#include "stdafx.h"
#include "EchoRequestCommand.h"
#include "CommandsIDs.h"

EchoRequestCommand::EchoRequestCommand(void):Command(CommandsIDs::ECHO_REQUEST)
{
}


EchoRequestCommand::~EchoRequestCommand(void)
{
}


BYTE* EchoRequestCommand::GenerateData()
{
	BYTE * answer = new BYTE[1];
	BYTE ans = '1';
	answer[0] = ans;
	this->data = answer;
	this->data_len = 1;
	return answer;
}