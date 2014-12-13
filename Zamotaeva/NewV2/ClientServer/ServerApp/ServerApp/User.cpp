#include "stdafx.h"
#include "User.h"
#include "MyServer.h"

User::User(void)
{
    RestoreLiveTime();
}

User::User(string userName,mysocket socket)
{
	this->userName = userName;
	this->userSocket = socket;
	RestoreLiveTime();
}

User::User(string userName,sockaddr_in addr)
{
	this->userName = userName;
	this->addr= addr;
	RestoreLiveTime();
}

User::User(mysocket socket)
{
	this->userName = "";
	this->userSocket = socket;
	RestoreLiveTime();
}


User::~User(void)
{
}


int User::getLiveTime()
{
    return this->LifeTime;
}

void User::DecreaseLiveTime(int value) //decrease livetime to value
{
    value = (value < 0)? 0: value;
	this->LifeTime -=value;
}

void User::RestoreLiveTime() //restart
{
    this->LifeTime = MyServer::USER_TIMEOUT; //max time 
	this->isSendWarning = false;
}


string User::getUserName()
{
    return this->userName;
}


mysocket  User::getUserSocket()
{
	return this->userSocket;
}
sockaddr_in User::getUserAddr()
{
	return this->addr;
}

string User::ParseName(string data)
{

	if(data.size() <=0)
	{
		return "";
	}
	string s_name_len = data.substr(0,2);
	int name_len = stoi(s_name_len);
	if(name_len <=0)
	{
		return "";
	}
	string s_name = data.substr(2,name_len);
	//s_name +='\0';
	return s_name;
}


	void User::setWarning(bool isWarning)
	{
		this->isSendWarning = isWarning;
	}

	bool User::getWarning()
	{
		return this->isSendWarning;
	}