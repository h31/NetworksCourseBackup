#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;


class User
{
public:
	User(void);
	~User(void);

    float getLiveTime();
    void DecreaseLiveTime(float value);//decrem
    void RestoreLiveTime();//restart
private:
	string userName;
    //unsigned int userID;
	string curIP;
    float LifeTime; //user life time before disconnect
};

