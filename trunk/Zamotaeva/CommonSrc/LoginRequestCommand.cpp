#include "StdAfx.h"
#include "LoginRequestCommand.h"

LoginRequestCommand::LoginRequestCommand(std::string Login):Command(CommandsIDs::LOGIN_REQUEST)
{
	this->Login = Login;
}

BYTE* LoginRequestCommand::GenerateData()
{
	BYTE * answer = new BYTE[Login.size()];
	for(int i= 0;i<Login.size();i++)
	{
		answer[i] = Login[i];
	}
	this->data = answer;
	this->data_len = Login.size();
	return answer;
}