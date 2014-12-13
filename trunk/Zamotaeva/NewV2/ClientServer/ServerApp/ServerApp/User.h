#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "CrossPlatformDefines.h"


using namespace std;


class User
{
public:
	User(void);
	User(string userName,mysocket socket);
	User(string userName,sockaddr_in addr);
	User(mysocket socket);
	~User(void);

    int getLiveTime();
    string getUserName();
	mysocket  getUserSocket();
	sockaddr_in getUserAddr();
    void DecreaseLiveTime(int value);//decrem
    void RestoreLiveTime();//restart
	void setWarning(bool isWarning);
	bool getWarning();

	static string ParseName(string data);
private:
	string userName;
	sockaddr_in addr;
    //unsigned int userID;
	mysocket  userSocket;
    int LifeTime; //user life time before disconnect
	bool isSendWarning;
};

