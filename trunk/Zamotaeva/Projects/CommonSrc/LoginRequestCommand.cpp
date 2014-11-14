#include "stdafx.h"
#include "LoginRequestCommand.h"

LoginRequestCommand::LoginRequestCommand(std::string Login):Command(CommandsIDs::LOGIN_REQUEST)
{
	this->Login = Login;
}

char* LoginRequestCommand::GenerateData()
{
    char * answer = new char[Login.size()];
    for(unsigned int i= 0;i<Login.size();i++)
	{
		answer[i] = Login[i];
	}
	this->data = answer;
	this->data_len = Login.size();
	return answer;
}
