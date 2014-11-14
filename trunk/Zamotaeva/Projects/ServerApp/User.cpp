#include "stdafx.h"
#include "User.h"
#include "MyServer.h"

User::User(void)
{
    RestoreLiveTime();
}


User::~User(void)
{
}


float User::getLiveTime()
{
    return this->LifeTime;
}

void User::DecreaseLiveTime(float value)
{
    value = (value < 0)? 0: value;
    this->LifeTime -=value;
}

void User::RestoreLiveTime() //restart
{
    this->LifeTime = MyServer::USER_TIMEOUT;
}
