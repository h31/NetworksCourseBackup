#include "commandFactory.h"
#include "../ServerApp/StringsHelper.h"
CommandFactory::CommandFactory()
{
}
//---------------------------------------------
//----------------EchoAnswerCommand------------

EchoAnswerCommand::EchoAnswerCommand(void): Command(CommandsIDs::ECHO_ANSWER) //command type
{
	GenerateData();
}

void EchoAnswerCommand::GenerateData() //data
{
	this->data = "1";
	this->data_len = data.size();
}






//---------------------------------------------
//-----------------LoginAnswerCommand----------

LoginAnswerCommand::LoginAnswerCommand(bool isGoodLogin) :Command(CommandsIDs::LOGIN_ANSWER)
{
    this->isGoodLogin = isGoodLogin;
	GenerateData();
}

void LoginAnswerCommand::GenerateData()
{
	if(this->isGoodLogin == true)
	{
			this->data = "1";
			this->data_len = data.size();
	}
	else
	{
		  	this->data = "0";
			this->data_len = data.size();
	}
}


//---------------------------------------------
//---------------------------------------------

UsersAnswerCommand::UsersAnswerCommand(UserList * userlist) : Command(CommandsIDs::USERS_ANSWER)
{
	this->userlist = userlist;
	GenerateData();
}

void UsersAnswerCommand::GenerateData()
{
	string s_usertext = this->userlist->getUserList();
	this->data = s_usertext;
	this->data_len = s_usertext.size();
}


//---------------------------------------------
//---------------------------------------------
UnAuthoririzedActionCommand::UnAuthoririzedActionCommand(int id):Command(CommandsIDs::UNAUTHORIZED_ACTION)
{
	this->action_id = id;
	GenerateData();
}
void UnAuthoririzedActionCommand::GenerateData()
{
	this->data = ""+action_id;
	this->data_len = data.size();
}

//---------------------------------------------
//---------------------------------------------
BadPackageCommand::BadPackageCommand(void) :Command(CommandsIDs::BAD_PACKAGE_ANSWER)
{
	this->data = "1";
	this->data_len = data.size();
}

//---------------------------------------------
//---------------------------------------------
EchoRequestCommand::EchoRequestCommand() :Command(CommandsIDs::ECHO_REQUEST)
{
}
void EchoRequestCommand::GenerateData()
{
	this->data = "1";
	this->data_len = data.size();
}


//---------------------------------------------
//---------------------------------------------
QuitCommand::QuitCommand() :Command(CommandsIDs::QUIT_REQUEST)
{

}
void QuitCommand::GenerateData()
{
	this->data = "1";
	this->data_len = data.size();
}

//---------------------------------------------
//---------------------------------------------
AdvertCommand::AdvertCommand(string advert_text): Command(CommandsIDs::ADVERT_MESSAGE)
{
	this->advert_text = advert_text;
}
void AdvertCommand::GenerateData()
{
	int advert_len = advert_text.size();
	string data = "" + advert_len+advert_text;
	this->data_len = data.size();
	this->data = data;
}